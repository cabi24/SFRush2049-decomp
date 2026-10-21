#!/usr/bin/env python3
"""mk.py BASEDIR OUTDIR FILE 'regex==>repl' ... : copy BASEDIR to OUTDIR and apply regex substitutions (MULTILINE) to FILE; prints counts."""
import sys, re, shutil, os
base, out, fn = sys.argv[1:4]
shutil.copytree(base, out, dirs_exist_ok=True)
p = os.path.join(out, fn); s = open(p).read()
for spec in sys.argv[4:]:
    rx, rp = spec.split('==>', 1)
    s, n = re.subn(rx, rp, s, flags=re.M)
    print(n, 'x', rx[:70])
open(p, 'w').write(s)
