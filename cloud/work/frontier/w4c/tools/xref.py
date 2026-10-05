#!/usr/bin/env python3
"""xref.py ADDR...: list functions in the game image whose lui/lo16 pairs reference ADDR (approximate scan)."""
import struct, json, bisect, sys
d = open('build/game_code.bin', 'rb').read(); base = 0x80086A50
s = json.load(open('asm/us/blob/symbols.json'))['symbols']
fns = sorted((int(v, 16), k) for k, v in s.items() if int(v, 16) < base + len(d))
addrs = [f[0] for f in fns]
W = struct.unpack('>%dI' % (len(d) // 4), d[:len(d) // 4 * 4])
for target in [int(a, 16) for a in sys.argv[1:]]:
    hi = (target + 0x8000) >> 16; lo = target & 0xFFFF
    hits = {}
    for i, w in enumerate(W):
        op = w >> 26
        if op in (0x09, 0x23, 0x2B, 0x21, 0x25, 0x20, 0x24, 0x28, 0x29, 0x31, 0x39) and (w & 0xFFFF) == lo:
            rs = (w >> 21) & 31
            for j in range(max(0, i - 40), i):
                v = W[j]
                if v >> 26 == 0x0F and (v & 0xFFFF) == hi and ((v >> 16) & 31) == rs:
                    a = base + 4 * i; k = bisect.bisect_right(addrs, a) - 1
                    hits.setdefault(fns[k][1], []).append('%08x' % a); break
    print('%08x:' % target, {k: len(v) for k, v in hits.items()})
