#!/usr/bin/env python3
"""vt.py BASEDIR OUTDIR 'old==>new' ['old2==>new2' ...] : copy group dir, apply replacements (each must match once unless prefixed with '*'), print"""
import sys, shutil, os
base, out = sys.argv[1], sys.argv[2]
if os.path.exists(out): shutil.rmtree(out)
shutil.copytree(base, out)
p = os.path.join(out, 'group.c')
s = open(p).read()
for r in sys.argv[3:]:
    allr = r.startswith('*')
    if allr: r = r[1:]
    a, b = r.split('==>', 1)
    a = a.replace('\\n', '\n'); b = b.replace('\\n', '\n')
    n = s.count(a)
    if n == 0 or (n > 1 and not allr):
        print(f'ERROR: {n} matches for {a!r}'); sys.exit(1)
    s = s.replace(a, b)
open(p, 'w').write(s)
