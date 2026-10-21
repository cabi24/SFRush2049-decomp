"""w36n probe: rank GC functions by ordered callee similarity to an N64 function (manual verification aid).
   python3 probe.py game:NAME [...]"""
import sys
from mapfn import Mapper
M = Mapper(); M.run(quiet=True)

def lcs(A, B):
    prev = [0] * (len(B) + 1)
    for a in A:
        cur = [0] * (len(B) + 1)
        for j, b in enumerate(B, 1):
            cur[j] = prev[j - 1] + 1 if a == b else max(prev[j], cur[j - 1])
        prev = cur
    return prev[-1]

for n in sys.argv[1:]:
    seq = M.calls.get(n, [])
    A = [M.m[t] for pc, t, k in seq if t in M.m]
    res = []
    for g, v in M.gc.items():
        if not v['gamesource'] or v['stripped']:
            continue
        for B0 in ([t for pc, t, k in v['calls']], M.gexp(g)):
            Bm = [t for t in B0 if t in M.inv]
            if len(Bm) < 2:
                continue
            l = lcs(A, Bm)
            res.append((round(l / max(len(A), len(Bm)), 2), l, len(Bm), g, v['size'], M.inv.get(g, '')))
    res.sort(reverse=True)
    seen = set(); out = []
    for r in res:
        if r[3] in seen: continue
        seen.add(r[3]); out.append(r)
        if len(out) == 4: break
    print(n, M.pop[n]['size'], 'mapped calls', len(A), '->', M.m.get(n), M.conf.get(n))
    for r in out: print('    sim %.2f lcs %d/%d  %s (%d B) mapped-to=%s' % r)
