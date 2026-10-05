"""Compare N64 observed typed field offsets with arcade struct layouts.

usage: arcade_compare.py <n64 entity> <ArcadeType> [-v]
  entity: array base hex (from bases.json), or ptr:<hex> (pointer global from ptr_refs.json)
Observed class per offset from access mnemonics; arcade leaf kinds from
arcade_layouts.json (mips-linux-gnu-gcc -mabi=32 layout of the arcade headers).
Reports agreement for the best single shift and best two-piece shift, against
the mean agreement over all shifts (chance baseline)."""
import json, sys, collections
from common import *
arc = json.load(open(OUT / "arcade_layouts.json"))
def observed(entity):
    obs = collections.defaultdict(set)
    if entity.startswith("ptr:"):
        a = int(entity[4:], 16)
        for r in json.load(open(OUT / "ptr_refs.json")):
            if r["ptr"] == a: obs[r["off"]].add(r["mn"])
    else:
        a = int(entity, 16)
        row = next(r for r in json.load(open(OUT / "bases.json")) if r["base"] == a)
        for o, m in row["types"].items(): obs[int(o, 16)] |= set(m)
    return obs
def klass(mns):
    if mns & {"lwc1", "swc1"}: return "f4"
    if "lh" in mns: return "s2"
    if "lhu" in mns: return "u2"
    if "sh" in mns: return "x2"
    if "lb" in mns: return "s1"
    if "lbu" in mns: return "u1"
    if "sb" in mns: return "x1"
    if mns & {"lw", "sw"}: return "w4"
    return None           # addiu-only (address taken), lwl/lwr
def compat(o, a):
    if a is None: return 0
    if o == "f4": return a == "f4"
    if o == "w4": return a in ("s4", "u4", "p4")
    if o[0] == "x": return a[1] == o[1] and a[0] in "su"
    return o == a
def run(entity, tname, verbose=False):
    obs = {o: klass(m) for o, m in observed(entity).items()}
    addr_only = sum(1 for k in obs.values() if k is None)
    obs = {o: k for o, k in obs.items() if k}
    leaves = {l[0]: l[2] for l in arc[tname]["leaves"]}
    lname = {l[0]: l[3] for l in arc[tname]["leaves"]}
    offs = sorted(obs)
    deltas = range(-0x200, 0x204, 4)
    hit = {d: [compat(obs[o], leaves.get(o + d)) for o in offs] for d in deltas}
    n = len(offs)
    best1 = max(deltas, key=lambda d: (sum(hit[d]), -abs(d)))
    base = sum(sum(h) for h in hit.values()) / len(hit) / n
    best2 = (0, None)
    for i in range(1, n):
        l = max(deltas, key=lambda d: (sum(hit[d][:i]), -abs(d))); r = max(deltas, key=lambda d: (sum(hit[d][i:]), -abs(d)))
        s = sum(hit[l][:i]) + sum(hit[r][i:])
        if s > best2[0]: best2 = (s, (offs[i], l, r))
    print(f"{entity} vs {tname} (arcade size {arc[tname]['size']:#x}): {n} typed offsets (+{addr_only} address-only), "
          f"classes {dict(collections.Counter(obs.values()))}")
    print(f"  chance baseline (mean over shifts): {base:.2f}")
    print(f"  best single shift {best1:+#x}: {sum(hit[best1])}/{n} = {sum(hit[best1])/n:.2f};  shift 0: {sum(hit[0])}/{n}")
    if best2[1]:
        s, (split, l, r) = best2
        print(f"  best two-piece: n64 off < {split:#x} shift {l:+#x}, >= {split:#x} shift {r:+#x}: {s}/{n} = {s/n:.2f}")
        if verbose:
            for o in offs:
                d = l if o < split else r
                a = leaves.get(o + d)
                print(f"    n64 +{o:#05x} {obs[o]:3s} | arcade +{o+d:#05x} {a or '-':3s} {lname.get(o+d, '(no field start)'):28s} {'ok' if compat(obs[o], a) else 'MISMATCH'}")
    return best2
if __name__ == "__main__":
    run(sys.argv[1], sys.argv[2], "-v" in sys.argv)
