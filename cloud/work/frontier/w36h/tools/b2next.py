#!/usr/bin/env python3
"""b2next.py: scan all image-B functions for unconditional b whose target is pc+8 (jump over its own delay slot)
and conditional branches whose target holds an unconditional b."""
import json, re, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path('/home/cburnes/projects/rush2049-decomp')
img = sys.argv[1] if len(sys.argv) > 1 else 'ovl_b'
SYM = json.load(open(ROOT/f'asm/us/{img}/symbols.json'))['symbols']
txt = open(next((ROOT/f'asm/us/{img}').glob('*.s'))).read()
for m in re.finditer(r'\.section \.text\.(\w+), "ax", @progbits\n(?:\.globl \w+\n)?\w+:\n((?:\s+\.word 0x[0-9A-Fa-f]{8}\n)+)', txt):
    name = m.group(1); words = [int(x,16) for x in re.findall(r'0x([0-9A-Fa-f]{8})', m.group(2))]
    va = int(SYM[name],16); hits=[]; b2b=[]
    for i,w in enumerate(words):
        op = w >> 26
        if w >> 16 == 0x1000:  # b
            off = (w & 0xffff); off = off - 0x10000 if off & 0x8000 else off
            if off == 1: hits.append(i)
        if op in (4,5,6,7) and (w>>16) != 0x1000:
            off = (w & 0xffff); off = off - 0x10000 if off & 0x8000 else off
            t = i + 1 + off
            if 0 <= t < len(words) and words[t] >> 16 == 0x1000: b2b.append(i)
    if hits or b2b: print(f'{name} ({len(words)} w): b-to-next at {hits}  cond->b at {b2b}')
