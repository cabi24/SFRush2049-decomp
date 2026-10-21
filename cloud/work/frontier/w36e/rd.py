#!/usr/bin/env python3
"""rd.py ADDR [NWORDS] : print words/floats/strings from image A."""
import sys, struct
img = open('/home/cburnes/projects/rush2049-decomp/build/ovl/a/image.bin','rb').read()
base = 0x8038A400
a = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 4
for i in range(n):
    o = a - base + 4*i
    w = struct.unpack('>I', img[o:o+4])[0]
    f = struct.unpack('>f', img[o:o+4])[0]
    print(f'{a+4*i:08X}: {w:08X}  {f:.9g}  {w-0x100000000 if w&0x80000000 else w}')
