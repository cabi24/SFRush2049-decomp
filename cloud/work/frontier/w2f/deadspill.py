#!/usr/bin/env python3
"""deadspill.py: list locked game functions whose retail body stores a caller-saved int register (v0,v1,t0-t9)
to a stack slot that is never loaded in that function (the 'spill without reload' signature)."""
from pathlib import Path
import json, struct, sys, re
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
lock = json.load(open(ROOT / 'blob_matched.lock.json'))
fs = lock.get('functions', lock)
T = score.targets()
CS = {2,3,8,9,10,11,12,13,14,15,24,25}
for name in sorted(T):
    w = T[name]
    if not w or (w[0] >> 16) != 0x27BD: 
        # frame adj may not be first
        pass
    frame = 0
    for x in w:
        if (x >> 16) == 0x27BD and (x & 0x8000): frame = 0x10000 - (x & 0xFFFF); break
    if not frame: continue
    st = {}; ld = set()
    for i, x in enumerate(w):
        op = x >> 26; rs = (x >> 21) & 31; rt = (x >> 16) & 31; off = x & 0xFFFF
        if rs != 29 or off >= frame: continue
        if op == 0x2B and rt in CS: st.setdefault(off, []).append((i, rt))
        if op in (0x23, 0x21, 0x25, 0x20, 0x24, 0x31, 0x22, 0x26): ld.add(off & ~3)
        if op == 0x09: ld.add(-1)  # addiu x,sp,off : address taken
    at = any((x >> 26) == 9 and ((x >> 21) & 31) == 29 and ((x >> 16) & 31) != 29 for x in w)
    dead = [(o, v) for o, v in st.items() if o not in ld]
    if dead and not at:
        print(name, 'LOCKED' if name in fs else 'open', len(w), 'frame', frame, [(o, [r for _, r in v]) for o, v in dead])
