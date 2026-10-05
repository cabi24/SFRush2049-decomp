"""Greedy cover: which struct/array entities (bases.json), pointer-struct families and
dense scalar blocks touch the most unmatched code bytes; marginal gain per pick."""
import json, collections
from common import *
import sys
STRUCT_ONLY = "--structs" in sys.argv
fsz = {n: s for v, s, n in FUNCS}
UM = {n for v, s, n in FUNCS if n not in MATCHED}
total = sum(fsz[f] for f in UM)
sets = {}
for r in json.load(open(OUT / "bases.json")):
    if r["noffs"] >= 3:
        sets[f"{r['kind']} {r['base']:08X}" + (f" stride {r['stride']:#x}" if r["stride"] else "") + (f" [{r['name']}]" if r["name"] else "")] = set(r["funcs"]) & UM
loose = {int(a, 16): set(f) for a, f in json.load(open(OUT / "loose_scalars.json")).items()}
for b in ([] if STRUCT_ONLY else json.load(open(OUT / "scalar_blocks.json"))):
    if b["addrs"] >= 4 and b["lo"] < 0x80123870 or b["lo"] >= END and b["addrs"] >= 4:
        fs = set().union(*(f for a, f in loose.items() if b["lo"] <= a <= b["hi"]))
        sets[f"scalar-block {b['lo']:08X}..{b['hi']:08X}"] = fs & UM
ro = json.load(open(OUT / "regoff.json"))
sets["ptr-struct: MODELDAT-like via pointer (lh 0x7C6 hallmark)"] = {f for f in UM if any(m == "lh" and o == 0x7c6 for m, o, _ in ro[f])}
pr = collections.defaultdict(set)
for r in json.load(open(OUT / "ptr_refs.json")): pr[r["ptr"]].add(r["f"])
for a in (0x8017A4E4, 0x8017A4EC):
    sets[f"ptr-struct via *D_{a:08X}"] = pr[a] & UM
anyent = set().union(*sets.values())
print(f"unmatched functions {len(UM)}, bytes {total}; touching >=1 listed entity: {len(anyent)} funcs, {sum(fsz[f] for f in anyent)} bytes")
covered = set(); print("| pick | entity | unmatched funcs | unmatched bytes | new bytes | cumulative bytes | cum % of unmatched |")
for i in range(1, 16):
    best = max(sets, key=lambda k: sum(fsz[f] for f in sets[k] - covered))
    gain = sum(fsz[f] for f in sets[best] - covered); covered |= sets[best]
    cum = sum(fsz[f] for f in covered)
    print(f"| {i} | {best} | {len(sets[best])} | {sum(fsz[f] for f in sets[best])} | {gain} | {cum} | {100*cum/total:.1f} |")
    del sets[best]
