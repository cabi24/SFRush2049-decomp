#!/usr/bin/env python3
"""split.py SRCDIR OUTDIR: split SRCDIR/group.c (prelude + '/* 0x8038XXXX name */' function blocks) into per-function-group files.
Layout (emission order = reverse of file order, observed): files listed so that emission is retail address order."""
import re, os, shutil, json, sys
src, out = sys.argv[1], sys.argv[2]
if os.path.exists(out): shutil.rmtree(out)
os.makedirs(out)
main = open(f'{src}/group.c').read()
parts = re.split(r'(?m)^(?=/\* 0x8038[0-9A-F]{4} )', main)
head = parts[0]
blocks = {re.match(r'/\* 0x(8038[0-9A-F]{4}) ', b).group(1): b for b in parts[1:]}
# emission order wanted (file groups, in emission order)
groups = [('cb20', 'ctx'), ('d054', 'ctx'), ('f1', ['8038D200', '8038D328']), ('d3a4', 'ctx'), ('f2', ['8038D498']),
          ('d798', 'ctx'), ('f3', ['8038DA78']), ('dddc', 'ctx'), ('f4', ['8038E088', '8038E114', '8038F568', '8038F938', '8038FCE0'])]
files = []
for name, what in groups:
    if what == 'ctx':
        shutil.copy(f'{src}/ctx_{name}.c', f'{out}/ctx_{name}.c'); files.append(f'ctx_{name}.c')
    else:
        open(f'{out}/{name}.c', 'w').write(head + ''.join(blocks[k] for k in what)); files.append(f'{name}.c')
d = json.load(open(f'{src}/group.json'))
d['files'] = list(reversed(files))
json.dump(d, open(f'{out}/group.json', 'w'), indent=1)
print(d['files'])
