#!/usr/bin/env python3
"""uvar.py BASE.c FUNC 'old-body-start' variants.py -- replace FUNC's definition in BASE with each variant and unit-score."""
import sys, re, subprocess, os
base, func, vfile = sys.argv[1], sys.argv[2], sys.argv[3]
extra = sys.argv[4:]
src = open(base).read()
# find definition of func: line starting with type and func( ... up to matching brace at col0 '}'
m = re.search(r'^[^\n;]*\b%s\([^;{]*\)\s*\{.*?^\}\n' % func, src, re.S | re.M)
assert m, 'no def'
ns = {}
exec(open(vfile).read(), ns)
outdir = os.path.dirname(os.path.abspath(vfile))
for name, body in ns['V'].items():
    s = src[:m.start()] + body.strip('\n') + '\n' + src[m.end():]
    p = os.path.join(outdir, 'u_%s.c' % name)
    open(p, 'w').write(s)
    r = subprocess.run(['python3', '-m', 'tools.conveyor.pipeline.blob_unit', '--tag', 'w10g', 'score', func, '--with', p] + extra,
                       cwd='/home/cburnes/projects/rush2049-decomp', capture_output=True, text=True)
    line = [l for l in r.stdout.splitlines() if (' %s:' % func) in l]
    print(name, '|', line[0].strip() if line else r.stdout[-400:] + r.stderr[-400:], flush=True)
