#!/usr/bin/env python3
# usage: offs.py NAME OBJ  -- sp offsets used by retail vs ours
import sys, re, subprocess
name, obj = sys.argv[1], sys.argv[2]
out = subprocess.run(['python3', '/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace/udiff.py', name, '--obj', obj, '--all'], capture_output=True, text=True).stdout
W=set(); G=set()
for l in out.splitlines():
    if len(l) < 40: continue
    a = l[8:40]; b = l[40:]
    for m in re.finditer(r'(-?\d+)\(sp\)|sp,(-?\d+)', a): W.add(int(m.group(1) or m.group(2)))
    for m in re.finditer(r'(-?\d+)\(sp\)|sp,(-?\d+)', b): G.add(int(m.group(1) or m.group(2)))
print('want', sorted(W)); print('got ', sorted(G))
