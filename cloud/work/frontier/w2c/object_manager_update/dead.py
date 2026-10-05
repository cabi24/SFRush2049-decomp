#!/usr/bin/env python3
"""dead.py BASE.c OUTDIR: insert one code-free dead read `if (X) {}` before every statement line of the function body."""
import sys
from pathlib import Path
from comb import comb
src = open(sys.argv[1]).read(); out = Path(sys.argv[2]); out.mkdir(exist_ok=True)
head, body = src.split('    sound_update_channel(0);\n')
lines = body.split('\n')
X = ['width', 'pos', 'wide', 'step', 'prev', 'ch', 's', 'maxlen', 'str', 'D_80149B70', 'D_801497F0', 'D_80149800', 'D_801497F0->kerning', 'D_801497F0->spaceWidth','s[pos]']
n = 0
for i, l in enumerate(lines):
    st = l.strip()
    if not st or st.startswith('}') or st.startswith('else'): 
        if st != '}': continue
    ind = l[:len(l) - len(l.lstrip())] if st != '}' else l[:len(l) - len(l.lstrip())] + '    '
    if i == len(lines) - 2: continue
    for j, x in enumerate(X):
        t = head + '    sound_update_channel(0);\n' + '\n'.join(lines[:i] + ['%sif (%s) {}' % (ind, x)] + lines[i:])
        (out / ('L%02d_%02d.c' % (i, j))).write_text(comb(t)); n += 1
print(n)
