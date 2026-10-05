"""Pointer-based (register+offset, base not a known absolute, not sp) access profile
across functions, from regoff.json.  Large offsets are distinctive struct hallmarks."""
import json, collections
from common import *
ro = json.load(open(OUT / "regoff.json"))
fsz = {n: s for v, s, n in FUNCS}
um = [f for f in ro if f not in MATCHED]
tot = sum(n for f in ro for _, _, n in ro[f]); totu = sum(n for f in um for _, _, n in ro[f])
print("reg+offset accesses (non-sp, non-absolute base): all", tot, " in unmatched", totu)
absr = collections.Counter(r["f"] for r in json.load(open(OUT / "refs.json")))
print("absolute refs: all", sum(absr.values()), " in unmatched", sum(n for f, n in absr.items() if f not in MATCHED))
by = collections.defaultdict(set)
for f in um:
    for mn, off, n in ro[f]: by[(mn, off)].add(f)
big = sorted(((len(fs), mn, off) for (mn, off), fs in by.items() if off >= 0x100), reverse=True)[:40]
print("top distinctive (off>=0x100) pointer accesses by #unmatched functions:")
print("  " + ", ".join(f"{mn} {off:#x}:{n}" for n, mn, off in big))
for lo, hi, label in ((0x400, 0x810, "MODELDAT-sized (max off 0x400..0x80f)"), (0x100, 0x3b8, "CAR-sized (max off 0x100..0x3b7)"), (0x810, 1 << 30, ">=0x810")):
    fs = [f for f in um if ro[f] and lo <= max(o for _, o, _ in ro[f]) < hi]
    print(f"unmatched functions whose largest pointer offset is {label}: {len(fs)}, bytes {sum(fsz[f] for f in fs)}")
h7c6 = [f for f in ro if any(mn == "lh" and off == 0x7c6 for mn, off, _ in ro[f])]
print("functions reading lh 0x7C6(ptr) [MODELDAT.net_node hallmark]:", len(h7c6), "unmatched", sum(1 for f in h7c6 if f not in MATCHED),
      "bytes", sum(fsz[f] for f in h7c6 if f not in MATCHED))
# offsets used through pointers by the 0x7C6 family: the N64 MODELDAT field census
fam = collections.Counter()
for f in h7c6:
    for mn, off, n in ro[f]:
        if off >= 0x40: fam[(off, mn)] += 1
print("  offsets (>=0x40) seen in >=4 of those functions:", ", ".join(f"{o:#x}/{m}:{n}" for (o, m), n in sorted(fam.items()) if n >= 4))
