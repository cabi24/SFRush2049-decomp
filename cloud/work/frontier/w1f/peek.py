#!/usr/bin/env python3
"""peek.py ADDR [NWORDS]: dump words of build/game_code.bin (base 0x80086A50) as hex/float."""
import sys, struct
BASE = 0x80086A50
d = open('/home/cburnes/projects/rush2049-decomp/build/game_code.bin','rb').read()
a = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 4
for i in range(n):
    o = a - BASE + 4*i
    if o + 4 > len(d): print('%08X: beyond image (bss)' % (a+4*i)); continue
    w = struct.unpack('>I', d[o:o+4])[0]
    print('%08X: %08X  %g' % (a + 4*i, w, struct.unpack('>f', d[o:o+4])[0]))
