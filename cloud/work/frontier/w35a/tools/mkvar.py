#!/usr/bin/env python3
"""mkvar.py BASE.c OUTDIR 'old=>new' ['old=>new' ...]: each spec yields a variant (also 'base'). Specs of the form
 a=>b|c=>d apply several replacements in one variant."""
import sys
from pathlib import Path
base = Path(sys.argv[1]).read_text(); out = Path(sys.argv[2]); out.mkdir(parents=True, exist_ok=True)
for f in out.glob('*.c'): f.unlink()
(out/'v000_base.c').write_text(base)
for i, spec in enumerate(sys.argv[3:], 1):
    t = base
    for r in spec.split('|'):
        a, b = r.split('=>')
        a = a.replace('\\n', '\n'); b = b.replace('\\n', '\n')
        if a not in t: print('NOT FOUND', repr(a), 'in spec', i); 
        t = t.replace(a, b)
    (out/f'v{i:03d}.c').write_text(t)
print(len(list(out.glob('*.c'))), 'variants')
