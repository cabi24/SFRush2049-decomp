#!/usr/bin/env python3
"""br2b.py FILE.dis|GOT.txt BASEHEX: list conditional branches whose target instruction is an unconditional b/j."""
import re, sys
rows=[]
for l in open(sys.argv[1]):
    m=re.match(r'\s*[0-9a-f]{8}:\s+[0-9a-f]{8}\s+(.*)',l) or re.match(r'\s*\d+ ([a-z].*)',l)
    if m: rows.append(re.sub(r'\s+',' ',re.sub(r'<[^>]*>','',m.group(1))).strip())
base=int(sys.argv[2],16)
for i,r in enumerate(rows):
    op=r.split()[0]
    if op.startswith('b') and op not in ('b','bal'):
        t=int(re.split(r'[ ,]',r)[-1],16)
        k=(t-base)//4
        if 0<=k<len(rows) and rows[k].split()[0] in ('b','j'):
            print(i, r, '->', k, rows[k])
