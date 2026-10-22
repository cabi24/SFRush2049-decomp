#!/usr/bin/env python3
"""(w39c copy of w38b/tools/objsyms.py; blob uses symbols.json addresses)
Builder-side: compile a group dir with score.py's compile_group and print the
.text function symbols in object order (offset, size-to-next, first words), plus
the retail extent of each name. Usage (from the scratch root):
  python3 objsyms.py GROUP_DIR ovl_a|ovl_b [OUT.o]"""
import sys, json, struct
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
gdir, img = Path(sys.argv[1]), sys.argv[2]
out = Path(sys.argv[3]) if len(sys.argv) > 3 else gdir / 'group.o'
score.ASM_DIR = Path('asm/us/' + img).resolve()
score.compile_group(gdir, out)
syms = score.symbols(out)
words = score.text_words(out)
if img == 'blob':
    ext = {k: (int(v, 16), 0) for k, v in json.load(open('asm/us/blob/symbols.json'))['symbols'].items()}
else:
    ext = {f['name']: (int(f['address'], 16), f['size'])
           for f in json.load(open(f'asm/us/{img}/extents.json'))['functions']}
order = sorted(syms.items(), key=lambda kv: kv[1])
spec = json.loads((gdir / 'group.json').read_text())
mem, ctx = set(spec['members']), set(spec.get('context', []))
for i, (n, off) in enumerate(order):
    end = order[i + 1][1] if i + 1 < len(order) else len(words) * 4
    w = words[off // 4:off // 4 + 2]
    stub = ' STUB' if end - off == 8 and w == [0x03e00008, 0] else ''
    r = ext.get(n)
    rs = f'retail {r[0]:08X}/{r[1]}' if r else 'retail -'
    role = 'M' if n in mem else ('C' if n in ctx else '-')
    print(f'{off:06x} {end-off:6d} {role} {n:32s} {rs}{stub}')
