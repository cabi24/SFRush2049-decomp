#!/usr/bin/env python3
"""dead2.py BASE.c OUTDIR: two-variable dead conditions `if (width OP y) {}` before every statement inside the loop."""
import sys
from pathlib import Path
from comb import comb
src = open(sys.argv[1]).read(); out = Path(sys.argv[2]); out.mkdir(exist_ok=True)
head, body = src.split('    while (')
lines = body.split('\n')
Y = ['pos', 'wide', 'step', 'prev', 'ch', 'maxlen', '(s32) str', 'D_80149B70', 'D_801497F0->kerning', 'D_801497F0->spaceWidth', 's[pos]', '0', '1000', 'width']
E = []
for y in Y:
    for op in ['<', '&&', '||', '+', '==']:
        E.append('width %s %s' % (op, y)); 
        if y != 'width': E.append('%s %s width' % (y, op))
n = 0
for i, l in enumerate(lines[1:-5], 1):
    st = l.strip()
    if not st or st.startswith('}') or st.startswith('else'): continue
    ind = l[:len(l) - len(l.lstrip())]
    for j, x in enumerate(E):
        t = head + '    while (' + '\n'.join(lines[:i] + ['%sif (%s) {}' % (ind, x)] + lines[i:])
        (out / ('L%02d_%03d.c' % (i, j))).write_text(comb(t)); n += 1
print(n); open(out / 'E.txt', 'w').write('\n'.join('%d %s' % p for p in enumerate(E)))
