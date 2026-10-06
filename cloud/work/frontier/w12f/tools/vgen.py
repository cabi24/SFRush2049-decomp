#!/usr/bin/env python3
"""vgen.py BASE.c OUTPREFIX VARIANTS.py : VARIANTS.py defines V = {name: [(old,new),...]}; writes OUTPREFIX_name.c"""
import sys
base, pre, vf = sys.argv[1:4]
s = open(base).read()
g = {}; exec(open(vf).read(), g)
for name, reps in g['V'].items():
    t = s
    for o, n in reps:
        if o not in t: print('MISSING in', name, ':', o[:60]); break
        t = t.replace(o, n, 1)
    else:
        open('%s_%s.c' % (pre, name), 'w').write(t); print(pre + '_' + name)
