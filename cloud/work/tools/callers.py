#!/usr/bin/env python3
"""callers.py NAME...: game functions whose retail words `jal` NAME, plus
likely function-pointer references (lui/addiu pairs). Use it to find a group's
missing callers (closure gaps). Cloud Lane A helper.
"""
from pathlib import Path
import json, sys
ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
syms = {k: int(v, 16) for k, v in json.load(open(ROOT / 'asm/us/blob/symbols.json'))['symbols'].items()}
T = score.targets()
for name in sys.argv[1:]:
    a = syms[name]; jal = 0x0C000000 | ((a >> 2) & 0x3FFFFFF)
    hi = (a + 0x8000) >> 16; lo = a & 0xFFFF
    calls = [n for n, w in T.items() if jal in w]
    ptr = [n for n, w in T.items() if any((x & 0xFFFF) == lo and (x >> 26) in (9,) for x in w) and any((x >> 26) == 15 and (x & 0xFFFF) == hi for x in w)]
    print(f'{name}: jal from {calls}; possible &fn (lui/addiu) in {ptr}')
