""".data map (0x8010FD7C..0x80123870): objects delimited by referenced addresses
(splat-style), byte classes, referencing functions, TU-contiguity test."""
import json, collections, struct, bisect
from common import *
RO = 0x80123870
refs = json.load(open(OUT / "refs.json"))
cls = json.load(open(OUT / "data_classes.json"))
faddr = {n: v for v, s, n in FUNCS}; fidx = {n: i for i, (v, s, n) in enumerate(FUNCS)}
# refine classes inside .data
def refine(i):
    c = cls[i]; w = word(DATA_START + 4 * i)
    if c in ("other", "fltlike"):
        e = (w >> 23) & 0xFF
        if w < 0x10000 or w >= 0xFFFF0000: return "smallint"
        if 0x60 <= e <= 0xA0 and (w >> 28) in (3, 4, 0xB, 0xC): return "fltlike"
        return "other"
    return c
ND = (RO - DATA_START) // 4
dc = [refine(i) for i in range(ND)]
tot = collections.Counter(dc)
print(".data bytes", 4 * ND)
for k, v in tot.most_common(): print(f"  {k:9s} {4*v:6d} {100*v/ND:5.1f}%")
by = collections.defaultdict(list)
for r in refs:
    if DATA_START <= r["addr"] < RO: by[r["addr"]].append(r)
starts = sorted(by)
# pointer targets inside data also delimit objects
ptr_targets = {word(DATA_START + 4 * i) for i in range(ND) if dc[i] == "dptr"}
bounds = sorted(set(starts) | {t for t in ptr_targets if DATA_START <= t < RO} | {DATA_START})
objs = []
for i, a in enumerate(bounds):
    e = bounds[i + 1] if i + 1 < len(bounds) else RO
    fs = sorted({r["f"] for r in by.get(a, [])}, key=faddr.get)
    lo, hi = (a - DATA_START) // 4, (e - DATA_START + 3) // 4
    comp = collections.Counter(dc[lo:hi])
    objs.append(dict(addr=a, size=e - a, funcs=fs, comp={k: 4 * v for k, v in comp.items()},
                     mns=sorted({r["mn"] for r in by.get(a, [])}), indexed=any(r.get("indexed") for r in by.get(a, []))))
print("objects (ref- or pointer-delimited):", len(objs), " directly code-referenced:", len(starts))
sz = sorted(o["size"] for o in objs)
print("size quantiles:", [sz[int(len(sz) * q)] for q in (0.25, 0.5, 0.75, 0.9, 0.99)], "max", sz[-1])
print("bytes in objects >=256:", sum(o["size"] for o in objs if o["size"] >= 256), " count", sum(1 for o in objs if o["size"] >= 256))
owners = collections.Counter(len(o["funcs"]) for o in objs)
print("objects by #referencing functions:", {k: owners[k] for k in sorted(owners)[:6]}, ">5:", sum(v for k, v in owners.items() if k > 5))
print("bytes in objects never referenced by code (pointer-only):", sum(o["size"] for o in objs if not o["funcs"]))
um = sum(1 for o in objs if o["funcs"] and all(f in MATCHED for f in o["funcs"]))
print("code-referenced objects whose referrers are all matched:", um, "of", sum(1 for o in objs if o["funcs"]))
# TU contiguity: consecutive single-owner objects -> |delta function index|
single = [(o["addr"], fidx[o["funcs"][0]]) for o in objs if len(o["funcs"]) == 1]
d = sorted(abs(b[1] - a[1]) for a, b in zip(single, single[1:]))
print("single-owner objects:", len(single), " |delta func index| between address-adjacent owners: median",
      d[len(d) // 2], "p75", d[int(len(d) * .75)], " share<=15:", round(sum(1 for x in d if x <= 15) / len(d), 3),
      " monotonic(nondecreasing) share:", round(sum(1 for a, b in zip(single, single[1:]) if b[1] >= a[1]) / len(d), 3))
# segment into TU-like clusters: runs where owner function index stays within +-40 of running median
clusters = []; cur = None
for a, fi in single:
    if cur and abs(fi - cur["last"]) <= 40:
        cur["hi"] = a; cur["fmin"] = min(cur["fmin"], fi); cur["fmax"] = max(cur["fmax"], fi); cur["n"] += 1; cur["last"] = fi
    else:
        cur = dict(lo=a, hi=a, fmin=fi, fmax=fi, n=1, last=fi); clusters.append(cur)
big = [c for c in clusters if c["n"] >= 3]
print("TU-like clusters (>=3 adjacent single-owner objects from functions within 40 indices):", len(big),
      "covering", sum(c["n"] for c in big), "of", len(single), "objects")
print("\n### address-ordered data clusters (code range of owners)")
print("| data range | bytes | objs | owner functions (text range) |")
for c in big:
    print(f"| {c['lo']:08X}..{c['hi']:08X} | {c['hi']-c['lo']:5d} | {c['n']:3d} | {FUNCS[c['fmin']][2]} .. {FUNCS[c['fmax']][2]} ({FUNCS[c['fmin']][0]:08X}..{FUNCS[c['fmax']][0]:08X}) |")
print("\n### objects >= 512 bytes")
print("| addr | size | dominant classes | #funcs (unmatched) | access | first referrers |")
for o in objs:
    if o["size"] >= 512:
        comp = " ".join(f"{k}:{v}" for k, v in sorted(o["comp"].items(), key=lambda kv: -kv[1])[:3])
        print(f"| {o['addr']:08X} | {o['size']} | {comp} | {len(o['funcs'])} ({sum(1 for f in o['funcs'] if f not in MATCHED)}) | {','.join(o['mns'])}{' idx' if o['indexed'] else ''} | {', '.join(o['funcs'][:3])} |")
json.dump(objs, open(OUT / "data_objects.json", "w"))
# 0x1000 windows summary
print("\n### 0x1000-byte windows")
for s in range(0, ND, 1024):
    c = collections.Counter(dc[s:s + 1024]); a = DATA_START + 4 * s
    fs = {r["f"] for k in range(a, a + 4096) for r in by.get(k, [])}
    print(f"| {a:08X} | " + " ".join(f"{k}:{4*v}" for k, v in c.most_common(4)) + f" | {len(fs)} funcs ({sum(1 for f in fs if f not in MATCHED)} unmatched) |")
