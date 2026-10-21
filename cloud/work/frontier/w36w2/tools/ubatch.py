#!/usr/bin/env python3
"""ubatch.py FN DIR [extra blob_unit args...]: score every DIR/*.c in the whole-program unit (tag w36w2),
one at a time; print 'file: verdict' sorted by differing words."""
import subprocess, sys, re, glob, os
fn, d = sys.argv[1], sys.argv[2]
extra = sys.argv[3:]
res = []
for f in sorted(glob.glob(os.path.join(d, '*.c'))):
    p = subprocess.run(['python3', '-m', 'tools.conveyor.pipeline.blob_unit', '--tag', 'w36w2', 'score', fn,
                        '--with', f] + extra, capture_output=True, text=True,
                       cwd='/home/cburnes/projects/rush2049-decomp')
    out = p.stdout + p.stderr
    m = re.search(r'(EQUAL %s: .*|FAIL %s: .*)' % (fn, fn), out)
    line = m.group(1) if m else ('ERROR ' + out.strip().splitlines()[-1] if out.strip() else 'ERROR')
    if os.environ.get('ROWS'):
        q = subprocess.run(['python3', 'cloud/work/frontier/tools/trace/udiff.py', fn, '--summary'], capture_output=True,
                           text=True, cwd='/home/cburnes/projects/rush2049-decomp', env=dict(os.environ, TAG='w36w2'))
        q2 = subprocess.run(['python3', 'cloud/work/frontier/tools/trace/udiff.py', fn, '--summary', '--ops'], capture_output=True,
                           text=True, cwd='/home/cburnes/projects/rush2049-decomp', env=dict(os.environ, TAG='w36w2'))
        m1 = re.search(r'differing rows (\d+).*frame (\d+/\d+)', q.stdout)
        m2 = re.search(r'differing rows (\d+)', q2.stdout)
        if m1:
            line = f"rows {m1.group(1)} ops {m2.group(1) if m2 else '?'} frame {m1.group(2)} | " + line
    n = 0 if 'EQUAL' in line else (int(re.search(r'(\d+) of', line).group(1)) if re.search(r'(\d+) of', line) else 9999)
    mr = re.match(r'rows (\d+)', line)
    if mr: n = int(mr.group(1))
    res.append((n, os.path.basename(f), line))
    print(f"{os.path.basename(f)}: {line}", flush=True)
print('--- ranked')
for n, f, l in sorted(res):
    print(n, f, l[:150])
