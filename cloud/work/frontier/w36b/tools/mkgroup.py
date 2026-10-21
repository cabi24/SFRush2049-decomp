#!/usr/bin/env python3
"""mkgroup.py OUTDIR [NAME=body.c ...]: compose a group from the prior car-select group.c,
replacing named function definitions with the given files; writes group.c + group.json."""
import json, re, sys, shutil
from pathlib import Path
import os
prior = Path(os.environ.get('SCAFFOLD', '/home/cburnes/projects/rush2049-decomp/src/ovl_a/groups/dot_cleanup_colortrace_20261011'))
out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True)
text = (prior / 'group.c').read_text()
for spec in sys.argv[2:]:
    name, f = spec.split('=')
    strip = name.endswith('!')
    name = name.rstrip('!')
    body = Path(f).read_text()
    m = re.search(r'^[A-Za-z_][^\n;]*\b' + name + r'\([^\n]*\)\s*\n\{', text, re.M)
    assert m, name
    if strip:
        # drop contiguous extern/define lines immediately before the function
        start = m.start()
        while True:
            prev_end = start - 1
            prev_start = text.rfind('\n', 0, prev_end) + 1
            line = text[prev_start:prev_end]
            if line.startswith('extern ') or line.startswith('#define') or line.startswith('#pragma'):
                start = prev_start
            else: break
        text = text[:start] + text[m.start():]
        m = re.search(r'^[A-Za-z_][^\n;]*\b' + name + r'\([^\n]*\)\s*\n\{', text, re.M)
    # find closing brace at column 0
    end = text.index('\n}\n', m.end()) + 3
    text = text[:m.start()] + body.rstrip('\n') + '\n' + text[end:]
# patch prefix: base arrays become [][13]
names = ['D_80110E78','D_8011103C','D_80111080','D_8011119C','D_80111230','D_8011128C','D_80111604','D_80111648','D_8011168C','D_8011157C','D_801115C0']
for n in names:
    text = text.replace(n + '[13]', n + '[][13]')
    text = re.sub(r'\b' + n + r'\[car\]', n + '[0][car]', text)
text = text.replace('D_80151AD0;\nextern u8 D_80146180[4],D_80140BDC;','X') if False else text
text = re.sub(r'u8 D_80146180\[4\],D_80140BDC;', 'u8 D_80146180[4]; extern volatile u8 D_80140BDC;', text)
text = re.sub(r'extern f32 D_8002EB94,', 'extern volatile f32 D_8002EB94; extern f32 ', text)
for sub in filter(None, os.environ.get('GSUBS','').split(';;')):
    a,b = sub.split('>>'); assert a in text, a; text = text.replace(a,b)
(out / 'group.c').write_text(text)
shutil.copy(prior / 'group.json', out / 'group.json')
