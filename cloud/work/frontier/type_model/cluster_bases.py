"""Cluster absolute references into struct arrays / struct singletons / scalars.

Entities:
  array  : indexed refs sharing a stride S>=0xC whose addresses lie within S of
           the cluster minimum (element 0 fields); count N inferred from formed
           end pointers base+k*S, else assumed (see ASSUME).
  struct : a formed base (lui+addiu) with >=3 distinct register offsets.
  Refs (any kind) falling inside an entity's extent are attributed to it and
  folded modulo the stride.
Output: bases.json + printed table of the top entities by distinct functions.
"""
import json, collections, re, bisect
from common import *
refs = [r for r in json.load(open(OUT / "refs.json")) if 0x80000400 <= r["addr"] < 0x80800000
        and not (BASE <= r["addr"] < CODE_END)]
fsz = {n: s for v, s, n in FUNCS}
# ---- names
names = {}
for line in (REPO / "symbol_addrs.us.txt").read_text().splitlines():
    m = re.match(r"\s*(?://\s*dup-addr\([^)]*\):\s*)?(\w+)\s*=\s*0x([0-9A-Fa-f]{8})\s*;", line)
    if m and not line.strip().startswith("// ") or (m and "dup-addr" in line):
        names.setdefault(int(m.group(2), 16), []).append(m.group(1))
src = (REPO / "tools/conveyor/pipeline/disasm.py").read_text()
for m in re.finditer(r"0x([0-9A-Fa-f]{8}):\s*\"(\w+)\"", src):
    n = m.group(2)
    if not n.startswith("D_"): names.setdefault(int(m.group(1), 16), []).append(n + "[GAME_SYMBOLS]")
gt = (REPO / "include/game_types.h").read_text()
# ---- arrays by stride
by_stride = collections.defaultdict(list)
for r in refs:
    if r.get("indexed") and r.get("stride", 0) >= 0xC:
        by_stride[r["stride"]].append(r)
formed = collections.Counter(r["addr"] for r in refs if r["kind"] == "form")
entities = []
for S, rs in by_stride.items():
    addrs = sorted({r["addr"] for r in rs})
    groups = [[addrs[0]]]
    for a in addrs[1:]:
        if a - groups[-1][0] < S: groups[-1].append(a)
        else: groups.append([a])
    for g in groups:
        fs = {r["f"] for r in rs if r["addr"] in g}
        if len(fs) < 2: continue
        entities.append(dict(kind="array", base=g[0], stride=S, idx_funcs=len(fs)))
# merge same-stride groups that are contiguous (constant element index / index bias folded into the address)
entities.sort(key=lambda e: (e["stride"], e["base"]))
m = []
for e in entities:
    if m and m[-1]["stride"] == e["stride"] and (e["base"] - m[-1]["base"]) % e["stride"] < e["stride"] \
            and e["base"] < m[-1]["hi"] + 2 * e["stride"]:
        m[-1]["hi"] = e["base"]; m[-1]["idx_funcs"] += e["idx_funcs"]; m[-1].setdefault("biased_bases", []).append(e["base"])
    else:
        e["hi"] = e["base"]; m.append(e)
entities = m
all_addrs = sorted({r["addr"] for r in refs})
idx_offs = {}
for e in entities:
    S = e["stride"]
    idx_offs[e["base"]] = {(r["addr"] - e["base"]) % S for r in by_stride[S] if e["base"] <= r["addr"] < e["hi"] + S}
other_bases = sorted(e["base"] for e in entities)
# extent: walk referenced addresses upward; the array ends at the first referenced address whose
# offset modulo S is not one of the indexed field offsets, or at another array's base. Cap 64 elements.
for e in entities:
    b, S = e["base"], e["stride"]; offs = idx_offs[b]
    end = b + 64 * S
    for a in all_addrs[bisect.bisect_left(all_addrs, b + S):]:
        if a >= end: break
        if (a - b) % S not in offs or (a in other_bases and a != b and a not in e.get("biased_bases", [])):
            end = b + ((a - b) // S) * S if (a - b) % S not in offs else a
            break
    end = max(end, e["hi"] + S)
    ks = sorted({(a - b) // S for a in formed if a > b and (a - b) % S == 0 and (a - b) // S <= 64})
    e["end_ptr_k"] = ks
    # element counts settled by array_extent.py evidence (end pointer formed AND the next element's
    # offsets stop matching the indexed field set)
    OVERRIDE = {0x80152818: 6, 0x8014A250: 6, 0x80144030: 4, 0x8017A510: 4}
    if b in OVERRIDE: end = b + OVERRIDE[b] * S
    e["count"] = (end - b + S - 1) // S
    e["end"] = end
entities.sort(key=lambda e: e["base"])
# ---- structs from formed bases with register offsets
per_base = collections.defaultdict(lambda: [set(), set()])
for r in refs:
    if "base" in r and not r.get("indexed"):
        per_base[r["base"]][0].add(r["off"]); per_base[r["base"]][1].add(r["f"])
def in_array(a):
    for e in entities:
        if e["base"] <= a < e["end"]: return e
    return None
structs = []
for b, (offs, fs) in per_base.items():
    if len(offs) >= 3 and not in_array(b):
        structs.append(dict(kind="struct", base=b, stride=None, end=b + max(offs) + 4, lo=b + min(min(offs), 0)))
structs.sort(key=lambda s: s["base"])
merged = []
for s in structs:
    if merged and s["base"] < merged[-1]["end"]:
        merged[-1]["end"] = max(merged[-1]["end"], s["end"]); merged[-1].setdefault("alt_bases", []).append(s["base"])
    else: merged.append(s)
entities += merged
entities.sort(key=lambda e: e["base"])
# ---- attribute refs
for e in entities: e.update(funcs=set(), offs=set(), loads=collections.Counter())
starts = [e["base"] for e in entities]
loose = collections.defaultdict(set)
for r in refs:
    hit = None
    for e in entities:
        if e["base"] <= r["addr"] < e["end"]:
            if hit is None or e["base"] > hit["base"]: hit = e
    if hit:
        off = (r["addr"] - hit["base"]) % hit["stride"] if hit["stride"] else r["addr"] - hit["base"]
        hit["funcs"].add(r["f"]); hit["offs"].add(off); hit["loads"][(off, r["mn"])] += 1
    else:
        loose[r["addr"]].add(r["f"])
def name_of(a):
    return ", ".join(names.get(a, [])) or ("(in game_types.h)" if f"{a:08X}" in gt else "")
rows = []
for e in entities:
    fs = e["funcs"]; um = [f for f in fs if f not in MATCHED]
    rows.append(dict(base=e["base"], kind=e["kind"], stride=e["stride"], count=e.get("count"),
                     end=e["end"], nfuncs=len(fs), nunmatched=len(um), unmatched_bytes=sum(fsz[f] for f in um),
                     noffs=len(e["offs"]), maxoff=max(e["offs"]) if e["offs"] else 0, name=name_of(e["base"]),
                     offs=sorted(e["offs"]), types={f"{o:#x}": sorted({m for (oo, m) in e["loads"] if oo == o}) for o in sorted(e["offs"])},
                     funcs=sorted(fs), where="data" if e["base"] < END else "bss"))
rows.sort(key=lambda r: -r["nfuncs"])
json.dump(rows, open(OUT / "bases.json", "w"), indent=0)
print("| base | where | kind | stride | N | funcs | unmatched | unmatched bytes | offsets | max off | current name |")
print("|---|---|---|---|---|---|---|---|---|---|---|")
for r in rows[:45]:
    print(f"| {r['base']:08X} | {r['where']} | {r['kind']} | {('%#x' % r['stride']) if r['stride'] else '-'} | {r['count'] or '?'} | {r['nfuncs']} | {r['nunmatched']} | {r['unmatched_bytes']} | {r['noffs']} | {r['maxoff']:#x} | {r['name']} |")
# loose scalars
lr = sorted(loose.items(), key=lambda kv: -len(kv[1]))
print("\nloose scalar addresses:", len(loose), " entities:", len(entities))
print("top loose scalars:")
for a, fs in lr[:40]:
    print(f"  {a:08X} funcs={len(fs):3d} unmatched={sum(1 for f in fs if f not in MATCHED):3d} {name_of(a)}")
# dense blocks of loose scalars (gap <= 0x10): candidate singleton structs or one TU's adjacent globals
la = sorted(loose); blocks = [[la[0]]]
for a in la[1:]:
    if a - blocks[-1][-1] <= 0x10: blocks[-1].append(a)
    else: blocks.append([a])
brow = []
for b in blocks:
    fs = set().union(*(loose[a] for a in b)); um = [f for f in fs if f not in MATCHED]
    brow.append((len(fs), b[0], b[-1], len(b), len(um), sum(fsz[f] for f in um)))
brow.sort(reverse=True)
print("\ndense loose-scalar blocks (gap<=0x10), top 30: | start | end | span | addrs | funcs | unmatched | unmatched bytes | names |")
for n, lo, hi, k, u, ub in brow[:30]:
    nm = "; ".join(f"{a:08X}={name_of(a)}" for a in sorted(loose) if lo <= a <= hi and name_of(a))[:110]
    print(f"| {lo:08X} | {hi:08X} | {hi-lo+4:#x} | {k} | {n} | {u} | {ub} | {nm} |")
json.dump([dict(lo=lo, hi=hi, addrs=k, funcs=n, unmatched=u, unmatched_bytes=ub) for n, lo, hi, k, u, ub in brow], open(OUT / "scalar_blocks.json", "w"))
json.dump({f"{a:08X}": sorted(fs) for a, fs in loose.items()}, open(OUT / "loose_scalars.json", "w"))
