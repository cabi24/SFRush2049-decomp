#!/usr/bin/env python3
"""rd.py ADDR [N] [s|w|f]: read the game image (build/game_code.bin, base 0x80086A50):
s = C string, w = N words, f = N floats. Read-only."""
import struct, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
img = open(ROOT / 'build/game_code.bin', 'rb').read(); base = 0x80086A50
a = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 1
mode = sys.argv[3] if len(sys.argv) > 3 else 's'
off = a - base
if mode == 's':
    for i in range(n):
        e = img.index(b'\0', off); print(hex(base + off), repr(img[off:e].decode('latin1'))); off = (e + 4) & ~3
elif mode == 'w':
    print(' '.join(f'{w:08X}' for w in struct.unpack(f'>{n}I', img[off:off + 4 * n])))
else:
    print(' '.join(f'{w:g}' for w in struct.unpack(f'>{n}f', img[off:off + 4 * n])))
