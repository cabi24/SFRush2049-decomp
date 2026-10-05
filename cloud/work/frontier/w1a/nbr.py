#!/usr/bin/env python3
"""nbr.py ADDR [radius]: list game functions around an address (addr, bytes, name, locked?)."""
import json, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
syms = {k: int(v, 16) for k, v in json.load(open(ROOT / 'asm/us/blob/symbols.json'))['symbols'].items()}
lock = json.load(open(ROOT / 'blob_matched.lock.json'))
lk = lock.get('functions', lock)
T = score.targets()
s = sorted(((syms[n], len(w) * 4, n) for n, w in T.items() if n in syms))
t = int(sys.argv[1], 16); r = int(sys.argv[2]) if len(sys.argv) > 2 else 4
i = min(range(len(s)), key=lambda i: abs(s[i][0] - t))
for a, sz, n in s[max(0, i - r):i + r + 1]:
    e = lk.get(n)
    print('%08x %5d %-28s %s' % (a, sz, n, ('locked ' + e.get('source', '')) if e else 'UNMATCHED'))
