#!/usr/bin/env python3
"""odata.py ADDR NWORDS [f|x|d]: dump image B words (build/ovl/b/image.bin) at vaddr"""
import struct, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
img = (ROOT/'build/ovl/b/image.bin').read_bytes()
a = int(sys.argv[1], 16); n = int(sys.argv[2]); mode = sys.argv[3] if len(sys.argv) > 3 else 'x'
for i in range(n):
    o = a + 4*i - 0x8038A400
    w = struct.unpack('>I', img[o:o+4])[0]
    f = struct.unpack('>f', img[o:o+4])[0]
    print(f'{a+4*i:08x}: {w:08x}  {f:.9g}  {struct.unpack(">i", img[o:o+4])[0]}')
