#!/usr/bin/env python3
"""sim.py SPILL_LOG [--x POS CONFLICTS]: replay uopt f_spilltemps home assignment (first fit in bit order, conflicts
from the CONFL lines of tools/trace/spill.sh) and report homes; with --x add a hypothetical web X before bit POS
conflicting with the comma-separated bits."""
import re, sys
L = open(sys.argv[1]).read().splitlines()
order = []; conf = {}
for l in L:
    m = re.match(r'CONFL proc=\S+ bit=(\d+) other=(\d+)', l)
    if m:
        a, b = int(m[1]), int(m[2]); conf.setdefault(a, set()).add(b); conf.setdefault(b, set()).add(a)
    m = re.match(r'HOME proc=\S+ bit=(\d+) home=(\d+)', l)
    if m: order.append(int(m[1]))
if '--x' in sys.argv:
    i = sys.argv.index('--x'); pos = int(sys.argv[i + 1]); S = [int(x) for x in sys.argv[i + 2].split(',')]
    order.insert(order.index(pos), 'X'); conf['X'] = set(S)
    for s in S: conf.setdefault(s, set()).add('X')
occ = []; h = {}
for w in order:
    for k, o in enumerate(occ):
        if not (o & conf.get(w, set())): o.add(w); h[w] = k; break
    else: occ.append({w}); h[w] = len(occ) - 1
for k, o in enumerate(occ): print('t%d' % k, sorted(o, key=str))
