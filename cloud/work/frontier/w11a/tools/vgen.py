#!/usr/bin/env python3
"""vgen.py BASE.c OUTDIR VARIANTS.py -- VARIANTS.py defines OLD (string in BASE) and V = {name: replacement}."""
import sys, os
base, out, vp = sys.argv[1:4]
src = open(base).read()
ns = {}
exec(open(vp).read(), ns)
old = ns['OLD']
assert src.count(old) == 1, 'OLD not unique/found'
os.makedirs(out, exist_ok=True)
for k, v in ns['V'].items():
    open(os.path.join(out, k + '.c'), 'w').write(src.replace(old, v))
print(len(ns['V']), 'variants')
