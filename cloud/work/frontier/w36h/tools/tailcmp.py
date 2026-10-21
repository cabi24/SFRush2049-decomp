#!/usr/bin/env python3
"""tailcmp.py WANT.dis GOT.txt N: compare the last N rows (aligned from the end)."""
import re, sys
want=[]
for l in open(sys.argv[1]):
    m=re.match(r'\s*[0-9a-f]{8}:\s+[0-9a-f]{8}\s+(.*)',l)
    if m: want.append(re.sub(r'\s+',' ',re.sub(r'<[^>]*>','',m.group(1))).strip())
got=[re.sub(r'\s+',' ',re.match(r'\s*\d+ (.*)',l).group(1)).strip() for l in open(sys.argv[2]) if re.match(r'\s*\d+ [a-z]',l)]
n=int(sys.argv[3]); off=len(got)-len(want)
for i in range(len(want)-n, len(want)):
    j=i+off; a=want[i]; b=got[j] if 0<=j<len(got) else ''
    a2=re.sub(r'0x8038[0-9a-f]+','J',a); b2=re.sub(r'0x[0-9a-f]+','J',b) if b.split()[0].startswith(('b','j')) else b
    if a.split()[0].startswith(('b','j')): a2=re.sub(r'0x[0-9a-f]+','J',a)
    print(('  ' if a2==b2 else '!!'), f'{i:5d} {a:38s} | {j:5d} {b}')
