"""Best arcade type per N64 entity by single-shift agreement above chance."""
import json, io, contextlib, collections
import arcade_compare as ac
from common import *
ents = ["80152818","8014A250","80144030","8012E700","8014A114","80151FC8","80150B7C","80151CF4","80139320","8017A510","801569B8","80156D38","ptr:8017A4E4","ptr:8017A4EC","ptr:801749C4"]
for e in ents:
    obs = {o: ac.klass(m) for o, m in ac.observed(e).items()}; obs = {o: k for o, k in obs.items() if k}
    offs = sorted(obs); n = len(offs); res = []
    for t, info in ac.arc.items():
        if info["size"] < 0x18: continue
        leaves = {l[0]: l[2] for l in info["leaves"]}
        deltas = range(-0x200, 0x204, 4)
        sc = {d: sum(ac.compat(obs[o], leaves.get(o + d)) for o in offs) for d in deltas}
        best = max(deltas, key=lambda d: (sc[d], -abs(d))); base = sum(sc.values()) / len(sc)
        res.append((sc[best] / n, base / n, t, best, sc[0] / n, info["size"]))
    res.sort(reverse=True)
    print(f"{e}: {n} typed offsets, span {min(offs):#x}..{max(offs):#x}, classes {dict(collections.Counter(obs.values()))}")
    for s, b, t, d, s0, size in res[:4]:
        print(f"    {t:14s} size {size:#6x} best shift {d:+#06x} agree {s:.2f} (shift0 {s0:.2f}, chance {b:.2f})")
