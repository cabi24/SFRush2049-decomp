#!/usr/bin/env python3
"""addrs.py FILE.dis: list absolute data addresses formed by lui + (addiu|load|store offset)"""
import re, sys, collections
lui = {}
found = collections.OrderedDict()
for line in open(sys.argv[1]):
    m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(\S+)\s*(.*)', line)
    if not m: continue
    pc, w, op, args = m.groups()
    args = args.split('   <')[0]
    mm = re.match(r'lui\s+(\w+),0x([0-9a-f]+)', op + ' ' + args)
    if op == 'lui':
        r, v = args.split(',')
        lui[r] = int(v, 16) << 16
        continue
    m2 = re.match(r'(\w+),(\w+),(-?\d+)$', args)
    if op == 'addiu' and m2 and m2.group(2) in lui:
        a = (lui[m2.group(2)] + int(m2.group(3))) & 0xffffffff
        found.setdefault(a, []).append((pc, op)); 
        if m2.group(1) != m2.group(2): pass
        continue
    m3 = re.match(r'(\$?\w+),(-?\d+)\((\w+)\)$', args)
    if m3 and m3.group(3) in lui:
        a = (lui[m3.group(3)] + int(m3.group(2))) & 0xffffffff
        found.setdefault(a, []).append((pc, op))
for a in sorted(found):
    print(f'{a:08x}  ' + ' '.join(f'{pc[-5:]}:{op}' for pc, op in found[a][:6]) + (f' (+{len(found[a])-6})' if len(found[a]) > 6 else ''))
