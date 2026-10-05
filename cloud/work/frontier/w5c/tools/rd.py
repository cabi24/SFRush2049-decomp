#!/usr/bin/env python3
"""rd.py ADDR [N]: print N retail words (hex, float) at game vaddr ADDR."""
import struct, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
d = (ROOT / 'build/game_code.bin').read_bytes()
a = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 1
for i in range(n):
    o = a - 0x80086A50 + 4 * i
    w = struct.unpack('>I', d[o:o+4])[0]
    print(f'{a+4*i:08X}: {w:08X} {struct.unpack(">f", d[o:o+4])[0]!r}')
