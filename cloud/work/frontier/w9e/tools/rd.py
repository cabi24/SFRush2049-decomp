#!/usr/bin/env python3
"""rd.py ADDR [N]: print N retail words (hex + float) from asm/us/blob_data/opaque.hex starting at ADDR."""
import sys, struct
mem = {}
for line in open('/home/cburnes/projects/rush2049-decomp/asm/us/blob_data/opaque.hex'):
    if line.startswith('#') or not line.strip(): continue
    a, h = line.split()
    a = int(a, 16); b = bytes.fromhex(h)
    for i, x in enumerate(b): mem[a + i] = x
addr = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 1
for k in range(n):
    a = addr + 4 * k
    try:
        w = bytes(mem[a + i] for i in range(4))
    except KeyError:
        print('%08X  (not in opaque runs)' % a); continue
    print('%08X  %s  %r' % (a, w.hex(), struct.unpack('>f', w)[0]))
