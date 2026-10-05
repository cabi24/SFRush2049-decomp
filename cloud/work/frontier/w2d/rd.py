#!/usr/bin/env python3
"""rd.py ADDR NWORDS: print retail image words from asm/us/blob_data/opaque.hex"""
import sys
a=int(sys.argv[1],16); n=int(sys.argv[2])
runs=[]
for l in open('asm/us/blob_data/opaque.hex'):
    if l.startswith('#') or not l.strip(): continue
    ad,h=l.split(); runs.append((int(ad,16),bytes.fromhex(h)))
out=[]
for i in range(n):
    x=a+4*i
    for ad,b in runs:
        if ad<=x<ad+len(b)-3 or (ad<=x and x+4<=ad+len(b)):
            out.append(b[x-ad:x-ad+4].hex()); break
    else: out.append('????????')
for i in range(0,n,8): print('%08X: %s'%(a+4*i,' '.join(out[i:i+8])))
