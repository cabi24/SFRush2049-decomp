#!/usr/bin/env python3
"""spuse.py OBJ ovl_a|ovl_b NAME [MIN]: per stack offset >= MIN, how often retail and ours access it
(lw/sw/lh/sh/lb/sb/lbu/lhu/lwc1/swc1 with base sp, and addiu rX,sp,N)."""
import sys, collections
sys.path.insert(0, 'tools/cloud')
from pathlib import Path
import score
obj, img, name = sys.argv[1:4]; mn = int(sys.argv[4]) if len(sys.argv) > 4 else 0
score.ASM_DIR = Path('asm/us/' + img).resolve()
w = score.text_words(obj); s = score.symbols(obj); st = s[name]
end = min((o for o in s.values() if o > st), default=len(w) * 4)
got = w[st // 4:end // 4]; want = score.targets()[name]
def usage(ws):
    c = collections.Counter()
    for x in ws:
        op = x >> 26; base = (x >> 21) & 31
        if base == 29 and op in (0x23, 0x2b, 0x20, 0x24, 0x28, 0x21, 0x25, 0x29, 0x31, 0x39): c[x & 0xffff] += 1
        if op == 9 and base == 29 and ((x >> 16) & 31) != 29: c['&%d' % (x & 0xffff)] += 1
    return c
a = usage(want); b = usage(got)
keys = sorted(set(a) | set(b), key=lambda k: int(str(k).lstrip('&')))
print('offset retail ours')
for k in keys:
    if int(str(k).lstrip('&')) >= mn: print(k, a.get(k, 0), b.get(k, 0))
