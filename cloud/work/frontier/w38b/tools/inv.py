#!/usr/bin/env python3
"""Inventory of overlay functions: address, size, locked group, stub flag."""
import json, sys
lock = json.load(open('ovl_matched.lock.json'))
for img in 'ab':
    e = json.load(open(f'asm/us/ovl_{img}/extents.json'))
    L = lock[img]
    fns = e['functions']
    for i, f in enumerate(fns):
        n = f['name']; sz = f['size']
        st = L.get(n)
        g = st['group'] if st and 'group' in st else ('single' if st else '-')
        prev = fns[i-1]['name'] if i else ''
        nxt = fns[i+1]['name'] if i+1 < len(fns) else ''
        mark = 'STUB' if sz == 8 else ''
        print(img, n, sz, 'LOCK' if st else 'open', g, mark, sep='\t')
