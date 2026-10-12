#!/usr/bin/env python3
"""w27s builder: uscan.py DIR FN OPSFILE [JOBS] -> for each line of OPSFILE (uedit ops), edit DIR/r.O, run urun.sh,
print 'rows first_diff ops' sorted (first_diff = first differing row index vs tgt)."""
import sys, os, subprocess, tempfile, concurrent.futures as cf
S = os.path.expanduser('~/rush2049/scratch/frontier/w27s')
D, FN, OPS = sys.argv[1], sys.argv[2], sys.argv[3]
J = int(sys.argv[4]) if len(sys.argv) > 4 else 2
env = dict(os.environ, PYTHONPATH=S + '/third_party/n64-decomp-workbench/src')
def one(a):
    n, ops = a
    E = f'{D}/s{n}'; os.makedirs(E, exist_ok=True)
    subprocess.run(['cp', f'{D}/r.T', E])
    r = subprocess.run(['python3', S + '/lane/tools/uedit.py', f'{D}/r.O', f'{E}/a.O'] + ops.split(), env=env, capture_output=True)
    if r.returncode: return (999, 999, ops)
    out = subprocess.run(['sh', S + '/lane/tools/urun.sh', E, f'{E}/a.O', FN], capture_output=True, text=True).stdout
    try:
        rows = int(out.strip().split()[-1])
        t = open(f'{E}/t.rows').read().split('\n'); g = open(f'{E}/a.rows').read().split('\n')
        fd = next((i for i in range(max(len(t), len(g))) if (t[i] if i < len(t) else None) != (g[i] if i < len(g) else None)), -1)
    except Exception:
        rows, fd = 998, 0
    subprocess.run(['rm', '-rf', E])
    return (rows, fd, ops)
lines = [l.strip() for l in open(OPS) if l.strip()]
with cf.ThreadPoolExecutor(J) as ex:
    res = list(ex.map(one, enumerate(lines)))
for r in sorted(res, key=lambda x: (x[0], -x[1])): print(r[0], r[1], r[2])
