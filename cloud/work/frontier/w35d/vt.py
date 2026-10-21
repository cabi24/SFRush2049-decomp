#!/usr/bin/env python3
"""vt.py BASE_GROUP NAME OLD NEW [OLD NEW ...] : make groups/BASE-NAME with substitutions, print counts.
OLD/NEW may be '@file' to read a file. Use --keep to keep dir."""
import sys, os, shutil, subprocess
base = sys.argv[1]; name = sys.argv[2]; pairs = sys.argv[3:]
src = f'groups/{base}'; dst = f'groups/_v_{name}'
shutil.rmtree(dst, ignore_errors=True); shutil.copytree(src, dst)
s = open(f'{dst}/group.c').read()
def rd(x): return open(x[1:]).read() if x.startswith('@') else x
for i in range(0, len(pairs), 2):
    o, n = rd(pairs[i]), rd(pairs[i+1])
    if o not in s: print('NOT FOUND:', o[:60]); sys.exit(1)
    s = s.replace(o, n, 1)
open(f'{dst}/group.c', 'w').write(s)
import os
fn = os.environ.get('FN')
if fn:
    print(name, subprocess.run(['./m.sh', dst, fn], capture_output=True, text=True).stdout.strip())
else:
    print(name, subprocess.run(['./cnt.sh', dst], capture_output=True, text=True).stdout.strip())
