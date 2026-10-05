#!/usr/bin/env python3
"""callers.py NAME...: retail functions containing a jal to NAME (and lock state)."""
from pathlib import Path
import json, sys
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
syms = {k: int(v, 16) for k, v in json.load(open(ROOT / 'asm/us/blob/symbols.json'))['symbols'].items()}
T = score.targets()
try:
    lock = json.load(open(ROOT / 'blob_matched.lock.json'))
    locked = set(lock.get('functions', lock).keys()) if isinstance(lock, dict) else set()
except Exception:
    locked = set()
for name in sys.argv[1:]:
    va = syms[name]; jal = 0x0C000000 | ((va >> 2) & 0x3FFFFFF)
    for fn, words in T.items():
        n = sum(1 for w in words if w == jal)
        if n: print(f'{name} <- {fn} @0x{syms.get(fn,0):08X} x{n} size={len(words)*4} {"locked" if fn in locked else "open"}')
