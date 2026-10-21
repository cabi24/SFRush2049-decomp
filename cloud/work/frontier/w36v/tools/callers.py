#!/usr/bin/env python3
# usage: callers.py HEXADDR...  -- find jal sites in asm/us/blob and name containing functions
import sys, re, json, glob, bisect
R='/home/cburnes/projects/rush2049-decomp/'
syms=json.load(open(R+'asm/us/blob/symbols.json'))
# symbols.json format guess
items=[]
syms=syms['symbols']
if isinstance(syms, dict):
    for k,v in syms.items():
        try:
            a=int(v,16) if isinstance(v,str) else (v if isinstance(v,int) else int(v.get('addr') or v.get('address'),16) if isinstance(v.get('addr') or v.get('address'),str) else v.get('addr') or v.get('address'))
            items.append((a,k))
        except Exception: pass
elif isinstance(syms, list):
    for e in syms:
        a=e.get('addr') or e.get('address'); n=e.get('name')
        if isinstance(a,str): a=int(a,16)
        items.append((a,n))
items.sort()
addrs=[a for a,_ in items]
targets={0x0C000000|((int(t,16)&0x0FFFFFFF)>>2): t for t in sys.argv[1:]}
for f in sorted(glob.glob(R+'asm/us/blob/blob_*.s')):
    base=int(re.search(r'blob_([0-9a-f]+)\.s',f).group(1),16)
    pc=base
    for line in open(f):
        m=re.match(r'\s*\.word 0x([0-9A-Fa-f]{8})',line)
        if not m: continue
        w=int(m.group(1),16)
        if w in targets:
            i=bisect.bisect_right(addrs,pc)-1
            print(targets[w], hex(pc), items[i][1] if i>=0 else '?')
        pc+=4
