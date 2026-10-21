#!/usr/bin/env python3
# reord.py IN OUT ORDER: re-emit the high-score file with function chunks in ORDER (comma list of chunk names; chunks not
# listed keep their place). Prototypes for every moved function are added after the preamble.
import sys, re
src = open(sys.argv[1]).read().split('\n')
order = sys.argv[3].split(',')
# chunk = (name, start, end) 0-based inclusive, found by 'type name(' at col 0 up to the closing '}' at col 0 (or one-liners)
chunks = []
i = 0
while i < len(src):
    m = re.match(r'^(?:static\s+)?(?:void|s32|s16|u32)\s+(\w+)\s*\(([^;]*)\)\s*(\{.*\})?\s*$', src[i])
    if m:
        name = m.group(1)
        if m.group(3):
            chunks.append((name, i, i, src[i][:src[i].index('{')].rstrip() + ';')); i += 1; continue
        j = i
        while src[j] != '}': j += 1
        chunks.append((name, i, j, src[i].rstrip() + ';')); i = j + 1; continue
    i += 1
by = {c[0]: c for c in chunks}
moved = [by[n] for n in order]
movedset = {c[0] for c in moved}
out = []
first = min(c[1] for c in chunks)
protos = [c[3] for c in moved]
# place: remove moved chunks, then append them (in ORDER) at the position of the LAST chunk in the file (after it)
keep = [l for k, l in enumerate(src) if not any(c[1] <= k <= c[2] for c in moved)]
text = '\n'.join(keep)
pre_end = '\n'.join(src[:first])
body = text[len(pre_end):]
tail = '\n\n'.join('\n'.join(src[c[1]:c[2] + 1]) for c in moved)
open(sys.argv[2], 'w').write(pre_end + '\n' + '\n'.join(protos) + '\n' + body.rstrip() + '\n\n' + tail + '\n')
