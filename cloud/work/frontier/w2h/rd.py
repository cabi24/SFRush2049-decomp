#!/usr/bin/env python3
"""rd.py ADDR [N]: hex dump N bytes of build/game_code.bin at vaddr (base 0x80086A50)."""
import sys,struct
d=open('/home/cburnes/projects/rush2049-decomp/build/game_code.bin','rb').read()
a=int(sys.argv[1],16); n=int(sys.argv[2]) if len(sys.argv)>2 else 32
o=a-0x80086A50
b=d[o:o+n]
for i in range(0,n,16):
    c=b[i:i+16]
    print('%08X: %s  %s'%(a+i,' '.join('%02x'%x for x in c),''.join(chr(x) if 32<=x<127 else '.' for x in c)))
