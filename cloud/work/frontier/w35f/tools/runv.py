#!/usr/bin/env python3
"""runv.py BASEDIR NAMES NAME1 'old==>new' ... -- NAME2 'old==>new' ... : run named variants; prints ALIGNED/norm/frame"""
import sys, os, shutil, subprocess, re
W = '/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35f'
base, names = sys.argv[1], sys.argv[2]
args = sys.argv[3:]
variants = []
cur = None
for a in args:
    if a == '--': cur = None; continue
    if cur is None: cur = [a, []]; variants.append(cur)
    else: cur[1].append(a)
FILE = os.environ.get('VFILE', 'group.c')
src = open(os.path.join(base, FILE)).read()
for name, reps in variants:
    s = src; ok = True
    for r in reps:
        star = r.startswith('*')
        if star: r = r[1:]
        o, n = r.split('==>', 1)
        o = o.replace('\\n', '\n'); n = n.replace('\\n', '\n')
        c = s.count(o)
        if c == 0 or (c > 1 and not star):
            print(name, 'ERROR', c, 'matches for', repr(o[:60])); ok = False; break
        s = s.replace(o, n)
    if not ok: continue
    out = f'{W}/v/rv_{name}'
    if os.path.exists(out): shutil.rmtree(out)
    shutil.copytree(base, out)
    open(os.path.join(out, FILE), 'w').write(s)
    r = subprocess.run([f'{W}/tools/g.sh', out, '--names', names], capture_output=True, text=True).stdout
    for line in r.splitlines():
        m = re.match(r'(\S+): .*ALIGNED rows differ (\d+) \(norm (\d+)\).*frame (\S+)', line)
        if m: print(f'{name:12s} {m.group(1)} rows={m.group(2)} norm={m.group(3)} frame={m.group(4)}')
        elif ': MATCH' in line: print(f'{name:12s} {line[:60]}')
    if 'cc failed' in r or 'ERROR' in r: print(r[:400])
