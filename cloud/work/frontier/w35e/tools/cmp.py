#!/usr/bin/env python3
"""cmp.py IMG FN OBJDUMP_TXT: side-by-side target (odis) vs compiled function from dump.sh output."""
import re, subprocess, sys
from pathlib import Path
here = Path(__file__).resolve().parent
img, fn, dump = sys.argv[1:4]
tgt = subprocess.run(['python3', str(here/'odis.py'), img, fn], capture_output=True, text=True).stdout.splitlines()[1:]
tl = [re.sub(r'^\s*[0-9a-f]+: [0-9a-f]{8} ', '', l).split('   <')[0] for l in tgt]
txt = open(dump).read().splitlines()
# find function
out=[]; on=False
for l in txt:
    m = re.match(r'[0-9a-f]+ <(\S+)>:', l)
    if m: on = (m.group(1)==fn); continue
    if on:
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', l)
        if m: out.append((m.group(1), m.group(3).split('<')[0].strip() if 'jal' in m.group(3) else m.group(3)))
        elif out and l.strip().startswith('R_MIPS'):
            pass
gl = [o[1] for o in out]
import difflib
sm = difflib.SequenceMatcher(None, tl, gl, autojunk=False)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal':
        for k in range(i2-i1): print(f'   {i1+k:3d}*4 {tl[i1+k]}')
    else:
        a = tl[i1:i2]; b = gl[j1:j2]
        n = max(len(a),len(b))
        for k in range(n):
            x = a[k] if k<len(a) else ''; y = b[k] if k<len(b) else ''
            print(f'!! {i1+k:3d}*4 {x:45s} | {y}')
