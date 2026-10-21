#!/usr/bin/env python3
"""xref.py ADDR [ADDR...]: game functions whose code builds %hi/%lo of ADDR (lui+offset pattern, simple scan)."""
from pathlib import Path
import json, sys, struct
ROOT = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
T = score.targets()
want = [int(a, 16) for a in sys.argv[1:]]
for name, words in T.items():
    hi = {}
    hits = set()
    for w in words:
        op = w >> 26; rs = (w >> 21) & 31; rt = (w >> 16) & 31; imm = w & 0xffff
        simm = imm - 0x10000 if imm & 0x8000 else imm
        if op == 0x0f:  # lui
            hi[rt] = imm << 16
            continue
        if op in (0x09, 0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2b, 0x31, 0x39) and rs in hi:
            a = (hi[rs] + simm) & 0xffffffff
            for t in want:
                if a == t: hits.add(hex(t))
    if hits: print(name, sorted(hits))
