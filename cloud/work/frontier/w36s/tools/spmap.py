#!/usr/bin/env python3
"""spmap.py FN : map retail sp offsets to ours using the aligned --all diff (udiff.py, TAG env)."""
import subprocess, sys, re, collections, os
fn = sys.argv[1]
out = subprocess.run([sys.executable, '/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace/udiff.py', fn, '--all', '--nosp'] + sys.argv[2:],
                     capture_output=True, text=True, env=dict(os.environ)).stdout
pairs = collections.Counter()
for line in out.splitlines():
    m = re.match(r'^[| ]\s*\+([0-9a-f]+)\s+(.*?)\s{2,}(.*)$', line)
    if not m: continue
    w, g = m.group(2), m.group(3)
    a = re.search(r'(-?\d+)\(sp\)', w); b = re.search(r'(-?\d+)\(sp\)', g)
    if a and b:
        pairs[(int(a.group(1)), int(b.group(1)), w.split()[0])] += 1
for (a, b, op), n in sorted(pairs.items()):
    print('want %4d  got %4d  %-6s x%d' % (a, b, op, n))
