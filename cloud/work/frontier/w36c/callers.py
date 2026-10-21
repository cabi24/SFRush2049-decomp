#!/usr/bin/env python3
"""callers.py IMG ADDR...: list functions in overlay IMG that jal ADDR (also scans .data refs as hi/lo pairs roughly)."""
import json, re, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
img = sys.argv[1]
d = ROOT / f'asm/us/ovl_{img}'
words = {}; cur = None
for line in open(d / f'ovl_{img}_8038a400.s'):
    m = re.match(r'\.section \.text\.(\S+?),', line.strip())
    if m: cur = words.setdefault(m.group(1), []); continue
    if line.strip().startswith('.section'): cur = None; continue
    m = re.match(r'\s*\.word\s+(0x[0-9A-Fa-f]+)', line)
    if m and cur is not None: cur.append(int(m.group(1), 16))
ov = {k: int(v, 16) for k, v in json.load(open(d / 'symbols.json'))['symbols'].items()}
for a in sys.argv[2:]:
    t = int(a.replace('func_', ''), 16)
    want = 0x0C000000 | ((t >> 2) & 0x3FFFFFF)
    hits = [(n, i) for n, w in words.items() for i, x in enumerate(w) if x == want]
    print(a, '<-', ', '.join(f'{n}+0x{i*4:x}' for n, i in hits) or '(no jal)')
