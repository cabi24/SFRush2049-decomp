#!/usr/bin/env python3
"""odata.py ADDR [LEN] [--str|--f|--w]: dump image-B bytes (build/ovl/b/image.bin, base 0x8038A400)."""
import sys, struct
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
img = (ROOT / 'build/ovl/b/image.bin').read_bytes()
a = int(sys.argv[1], 16); n = int(sys.argv[2], 0) if len(sys.argv) > 2 and not sys.argv[2].startswith('--') else 64
mode = [x for x in sys.argv if x.startswith('--')]
o = a - 0x8038A400
b = img[o:o+n]
if '--str' in mode:
    for i, s in enumerate(b.split(b'\0')):
        pass
    off = 0
    for s in b.split(b'\0'):
        print(f'{a+off:08X}: {s!r}'); off += len(s) + 1
elif '--f' in mode:
    for i in range(0, n, 4): w, = struct.unpack('>I', b[i:i+4]); f, = struct.unpack('>f', b[i:i+4]); print(f'{a+i:08X}: {w:08X} {f!r}')
else:
    for i in range(0, n, 16): print(f'{a+i:08X}: ' + ' '.join(f'{struct.unpack(">I", b[j:j+4])[0]:08X}' for j in range(i, min(i+16, n), 4)))
