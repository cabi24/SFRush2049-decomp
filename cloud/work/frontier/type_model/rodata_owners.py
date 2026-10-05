"""Ownership of .rodata (0x80123870..end) words by function, and how matched
sources currently spell their rodata references."""
import json, re, collections, struct
from common import *
RO = 0x80123870
refs = json.load(open(OUT / "refs.json"))
faddr = {n: v for v, s, n in FUNCS}
ref_at = collections.defaultdict(set); mn_at = collections.defaultdict(set)
for r in refs:
    if RO <= r["addr"] < END:
        ref_at[r["addr"] & ~3].add(r["f"]); mn_at[r["addr"] & ~3].add(r["mn"])
kinds = {}; owner = {}
a = RO
while a < END:
    w = word(a); f = func_at(w) if (BASE <= w < CODE_END and w % 4 == 0) else None
    if "ldc1" in mn_at.get(a, ()):
        kinds[a] = kinds[a + 4] = "double"; owner[a] = owner[a + 4] = min(ref_at[a], key=faddr.get); a += 8; continue
    if f and w != f[0]:
        kinds[a] = "jtbl"; owner[a] = f[2]
    elif a in ref_at:
        kinds[a] = "float" if mn_at[a] & {"lwc1"} else "ref-other:" + ",".join(sorted(mn_at[a]))
        owner[a] = min(ref_at[a], key=faddr.get)
    elif w == 0: kinds[a] = "zero-pad"
    else: kinds[a] = "unreferenced"
    a += 4
c = collections.Counter(k.split(":")[0] for k in kinds.values())
print("rodata", END - RO, "bytes:", {k: 4 * v for k, v in c.items()})
print("ref-other detail:", collections.Counter(k for k in kinds.values() if k.startswith("ref-other")))
# fill unowned words with previous owner; test monotonic order
prev = None; seq = []
for a in range(RO, END, 4):
    o = owner.get(a)
    if o: seq.append((a, o))
viol = [(a, o, p) for (pa, p), (a, o) in zip(seq, seq[1:]) if faddr[o] < faddr[p]]
print("owned words", len(seq), "order violations (owner addr decreases):", len(viol))
for v in viol[:10]: print("   %08X %s after %s" % v)
shared = [a for a in ref_at if len(ref_at[a]) > 1]
print("rodata words referenced by >1 function:", len(shared), [(hex(a), sorted(ref_at[a])) for a in shared][:5])
unref = [a for a, k in kinds.items() if k == "unreferenced"]
print("unreferenced nonzero words:", len(unref), [f"{a:08X}={word(a):08X}" for a in unref[:24]])
per = collections.defaultdict(lambda: collections.Counter())
for a, o in owner.items(): per[o][kinds[a].split(":")[0]] += 4
own_m = [f for f in per if f in MATCHED]; own_u = [f for f in per if f not in MATCHED]
print(f"functions owning rodata: {len(per)} (matched {len(own_m)}, unmatched {len(own_u)})")
for label, fs in (("matched", own_m), ("unmatched", own_u)):
    t = collections.Counter()
    for f in fs: t.update(per[f])
    print(f"  {label}: {dict(t)}  with jtbl: {sum(1 for f in fs if per[f]['jtbl'])}  with float/double: {sum(1 for f in fs if per[f]['float'] or per[f]['double'])}")
fsz = {n: s for v, s, n in FUNCS}
print("  unmatched code bytes in rodata-owning functions:", sum(fsz[f] for f in own_u),
      "of", sum(s for v, s, n in FUNCS if n not in MATCHED))
# how matched sources spell it
src_of = {}
for name, e in LOCK.items():
    src_of[name] = e["source"]
modes = collections.Counter(); ex = collections.defaultdict(list)
for f in own_m:
    e = LOCK[f]; p = REPO / e["source"]
    if "group" in e:
        gd = p.parent; text = "".join(q.read_text(errors="replace") for q in gd.glob("*.c"))
    else:
        text = p.read_text(errors="replace")
    fake = {int(x, 16) for x in re.findall(r"\bD_(8012[34][0-9A-Fa-f]{3})\b", text)}
    fake = {x for x in fake if RO <= x < END}
    mine = {a for a, o in owner.items() if o == f}
    has_jt = per[f]["jtbl"] > 0
    mode = ("group" if "group" in e else "single") + ("+jtbl" if has_jt else "") + \
           ("/extern-D_" if fake & mine else "/native-literal")
    modes[mode] += 1; ex[mode].append(f)
print("matched rodata-owning functions by build path / spelling:")
for m, n in modes.most_common(): print(f"  {m:28s} {n:4d}  e.g. {ex[m][:3]}")
json.dump({"kinds": {f"{a:08X}": k for a, k in kinds.items()}, "owner": {f"{a:08X}": o for a, o in owner.items()}},
          open(OUT / "rodata_owners.json", "w"))
