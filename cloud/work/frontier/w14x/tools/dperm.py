#!/usr/bin/env python3
"""dperm.py BEST.c OUTDIR N [--seed S]: write BEST (v0000) plus N-1 variants with the function's local declaration
lines randomly permuted (declaration order sets stack slots and colouring priority). Declarations are the 4-space
indented lines between the function header and the first statement."""
import sys, random, re
from pathlib import Path
best, out, n = Path(sys.argv[1]), Path(sys.argv[2]), int(sys.argv[3])
seed = int(sys.argv[sys.argv.index('--seed') + 1]) if '--seed' in sys.argv else 1
rng = random.Random(seed)
lines = best.read_text().split('\n')
FN = next((a for a in sys.argv[4:] if a.startswith('func_')), 'func_800CC50C')
h = next(i for i, l in enumerate(lines) if re.match(r'^[^ ].*\b' + FN + r'\(.*\{\s*$', l))
i = h + 1
decl = []
while i < len(lines) and (lines[i].strip() == '' or (re.match(r'^\s*[A-Za-z_][\w ]*\**\s*\**[\w, \*]*;$', lines[i]) and not lines[i].strip().startswith(('return', 'goto')))):
    if lines[i].strip():
        decl.append(i)
    i += 1
decl_lines = ['    ' + lines[k].strip() for k in decl]
out.mkdir(parents=True, exist_ok=True)
(out / 'v0000.c').write_text('\n'.join(lines) + '\n')
seen = set()
for k in range(1, n):
    for _ in range(50):
        perm = decl_lines[:]
        rng.shuffle(perm)
        if tuple(perm) not in seen and perm != decl_lines:
            break
    seen.add(tuple(perm))
    new = lines[:decl[0]] + perm + lines[decl[-1] + 1:]
    (out / ('v%04d.c' % k)).write_text('\n'.join(new) + '\n')
print(len(decl), 'decl lines; wrote', n, 'files to', out)
