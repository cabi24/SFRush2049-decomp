"""Is game data emitted in function (TU) order?  For each function, the span of
data-segment addresses it references; then test monotonicity per region."""
import json, collections, sys
from common import *
refs = json.load(open(OUT / "refs.json"))
cls = json.load(open(OUT / "data_classes.json"))
faddr = {n: v for v, s, n in FUNCS}
fsize = {n: s for v, s, n in FUNCS}
def region_report(lo, hi, label, verbose=False):
    per = collections.defaultdict(set)
    for r in refs:
        if lo <= r["addr"] < hi: per[r["f"]].add(r["addr"])
    order = sorted(per, key=lambda f: faddr[f])
    # monotonic test: count functions whose min address < running max of earlier functions' min
    inversions = 0; prev_max = lo; overlaps = 0; shared = collections.Counter()
    for f in order:
        for a in per[f]: shared[a] += 1
    excl = {a for a, n in shared.items() if n == 1}
    run_hi = lo; viol = []
    for f in order:
        mine = sorted(a for a in per[f] if a in excl)
        if not mine: continue
        if mine[0] < run_hi:
            inversions += 1; viol.append((f, mine[0], run_hi))
        run_hi = max(run_hi, mine[-1])
    nexcl = sum(1 for f in order if any(a in excl for a in per[f]))
    print(f"\n[{label}] {lo:08X}..{hi:08X}: {len(order)} referencing funcs "
          f"({sum(1 for f in order if f not in MATCHED)} unmatched), {len(shared)} distinct addrs, "
          f"{len(excl)} addrs single-owner; order violations {inversions}/{nexcl}")
    if verbose:
        for f in order:
            m = sorted(per[f])
            print(f"  {faddr[f]:08X} {f:34s} {'M' if f in MATCHED else 'u'} {m[0]:08X}..{m[-1]:08X} n={len(m)}")
    for v in viol[:12]: print(f"   violation {v[0]} first={v[1]:08X} < running max {v[2]:08X}")
    return per
if __name__ == "__main__":
    b = [int(x, 16) for x in sys.argv[1:3]]
    region_report(b[0], b[1], "arg", verbose=len(sys.argv) > 3)
