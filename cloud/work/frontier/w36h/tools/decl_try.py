#!/usr/bin/env python3
"""decl_try.py BASEDIR FUNC NAME 'decl block text (exact, from the group.c)' 'perm1' 'perm2' ... : for each permuted declaration
block write a variant dir and print ALIGNED/norm. Each perm is a '|'-separated list of declaration lines."""
import sys, subprocess, os, shutil, re
base, name = sys.argv[1], sys.argv[2]
blockfile = sys.argv[3]   # file with the original decl lines (one per line), in order
perms = sys.argv[4:]
orig = open(blockfile).read().rstrip('\n')
W = '/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36h'
src = open(os.path.join(base, 'group.c')).read()
assert orig in src, 'orig block not found'
res = []
for k, p in enumerate(perms):
    out = f'{W}/v/dt{k}'
    if os.path.exists(out): shutil.rmtree(out)
    shutil.copytree(base, out)
    new = '\n'.join(x for x in p.split('|'))
    open(os.path.join(out, 'group.c'), 'w').write(src.replace(orig, new))
    r = subprocess.run([f'{W}/tools/g.sh', out, '--names', name], capture_output=True, text=True).stdout
    m = re.search(r'ALIGNED rows differ (\d+) \(norm (\d+)\).*frame (\S+)', r)
    print(k, m.groups() if m else r[:200], p.replace('|', ' ; ')[:150])
