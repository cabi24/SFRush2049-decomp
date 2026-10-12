#!/usr/bin/env python3
"""p71rep.py TRACE [--at cm] BIT...|/regex/  -- per bit: its text and the nodes in each PRE vector (pre71 trace)."""
import re, sys
args = sys.argv[1:]
at = 'cm'
if '--at' in args:
    i = args.index('--at'); at = args[i + 1]; del args[i:i + 2]
path, sel = args[0], args[1:]
bits, vecs = {}, {}
for line in open(path):
    m = re.match(r'\[P71\] bit at=(\S+) bit=(\d+) .*?text=(\S*)', line)
    if m and m.group(1) == at:
        bits[int(m.group(2))] = m.group(3); continue
    m = re.match(r'\[P71\] vec at=(\S+) node=(\d+) (\w+)=(.*)$', line)
    if m and m.group(1) == at:
        s = set(int(x) for x in m.group(4).split(',') if x)
        vecs.setdefault(m.group(3), {})[int(m.group(2))] = s
want = []
for s in sel:
    if s.startswith('/'):
        want += [b for b, t in sorted(bits.items()) if re.search(s.strip('/'), t)]
    else:
        want.append(int(s))
for b in want:
    print('bit %d %s' % (b, bits.get(b)))
    for v in ('antlocs', 'avlocs', 'alters', 'absalters', 'antin', 'antout', 'iv', 'cand', 'ppin', 'ppout',
              'hoisted', 'insert', 'delete', 'subdelete', 'subinsert'):
        nodes = [n for n, s in sorted(vecs.get(v, {}).items()) if b in s]
        print('  %-10s %s' % (v, ' '.join(map(str, nodes))))
