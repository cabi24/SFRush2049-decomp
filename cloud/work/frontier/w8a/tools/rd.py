#!/usr/bin/env python3
"""rd.py ADDR... -- print image word(s) at game addresses as hex and float (build/game_code.bin, base 0x80086A50)"""
import sys,struct
d=open('/home/cburnes/projects/rush2049-decomp/build/game_code.bin','rb').read()
for a in sys.argv[1:]:
    a=int(a,16); o=a-0x80086A50; b=d[o:o+4]
    print(hex(a), b.hex(), repr(struct.unpack('>f',b)[0]), 'dbl' if 0 else '', repr(struct.unpack('>d',d[o:o+8])[0]))
