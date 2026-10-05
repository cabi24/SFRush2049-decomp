#!/usr/bin/env python3
"""rodata_check.py OBJDUMP_S_OUTPUT: compare an object's .rodata (objdump -s -j .rodata text) with the retail
words 0x80124638..0x801247EC from asm/us/blob_data/opaque.hex (run from the repo root)."""
import re, sys
got = b''
for l in open(sys.argv[1]).read().splitlines():
    m = re.match(r'\s([0-9a-f]{4,}) ((?:[0-9a-f]{8} ?){1,4})', l)
    if m: got += bytes.fromhex(m.group(2).replace(' ', ''))
d = {}
for l in open('asm/us/blob_data/opaque.hex'):
    m = re.match(r'([0-9A-Fa-f]{8}) ([0-9a-f]+)$', l.strip())
    if m:
        a = int(m.group(1), 16); b = bytes.fromhex(m.group(2))
        for i in range(len(b)): d[a + i] = b[i]
want = bytes(d[0x80124638 + i] for i in range(0x801247EC - 0x80124638))
bad = [(hex(i), got[i:i + 4].hex(), want[i:i + 4].hex()) for i in range(0, len(want), 4) if got[i:i + 4] != want[i:i + 4]]
print('object .rodata %d bytes, retail window %d bytes (%d words), mismatching words: %s, object tail: %s' % (len(got), len(want), len(want) // 4, bad, got[len(want):].hex()))
