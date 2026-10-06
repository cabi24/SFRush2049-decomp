#!/usr/bin/env python3
"""vrun.py BASE.c FN SPEC.py [extra blob_unit args]: SPEC defines VARIANTS = {name: [(old, new), ...]};
writes BASE dir/vr_<name>.c, unit-scores each (tag w10d) and prints differing aligned rows."""
import sys, subprocess, re, os
base, fn, spec = sys.argv[1:4]; extra = sys.argv[4:]
ns = {}; exec(open(spec).read(), ns)
src = open(base).read(); d = os.path.dirname(base)
for name, reps in ns['VARIANTS'].items():
    s = src
    ok = True
    for old, new in reps:
        if old not in s: print(name, 'MISSING', old[:50]); ok = False; break
        s = s.replace(old, new)
    if not ok: continue
    p = os.path.join(d, 'vr_%s.c' % name); open(p, 'w').write(s)
    r = subprocess.run(['python3', '-m', 'tools.conveyor.pipeline.blob_unit', '--tag', 'w10d', 'score', fn, '--with', p] + extra, capture_output=True, text=True).stdout
    st = [l.strip() for l in r.splitlines() if 'EQUAL' in l or 'FAIL' in l or 'STAGEFAIL' in l][:1]
    u = subprocess.run(['python3', 'cloud/work/frontier/w10d/tools/udiff.py', fn], capture_output=True, text=True).stdout.splitlines()
    print('%-12s %s | %s' % (name, u[-1] if u else '', st[0][:90] if st else ''))
