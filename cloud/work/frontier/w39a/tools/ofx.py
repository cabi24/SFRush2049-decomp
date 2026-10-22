#!/usr/bin/env python3
"""ofx.py a|b ADDR [ADDR...]: disassemble an overlay function (extents.json) from build/ovl/<x>/image.bin,
jal targets named (overlay func_ names + game symbols + w36n MAT3 identity). Read-only."""
import json, re, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
x = sys.argv[1]
ext = json.load(open(ROOT / f'asm/us/ovl_{x}/extents.json'))
base = int(ext['base'], 16); img = open(ROOT / f'build/ovl/{x}/image.bin', 'rb').read()
fns = {int(f['address'], 16): f for f in ext['functions']}
gs = {int(v, 16): k for k, v in json.load(open(ROOT / 'asm/us/blob/symbols.json'))['symbols'].items()}
mp = {}
for r in json.load(open(ROOT / 'cloud/work/frontier/w36n/map_functions.json'))['rows']:
    if r.get('mat3_name') and r['population'] in ('game', 'ovl_' + x):
        mp[(r['population'], r['n64_name'])] = f"{r['mat3_name']}@{r['mat3_file']}[{r['confidence'][0]}]"
def lab(a):
    if a in fns:
        n = fns[a]['name']; return n + ('=' + mp[('ovl_' + x, n)] if ('ovl_' + x, n) in mp else '')
    n = gs.get(a, hex(a)); return n + ('=' + mp[('game', n)] if ('game', n) in mp else '')
for s in sys.argv[2:]:
    a = int(s, 16); f = fns[a]; n = f['size']
    with tempfile.NamedTemporaryFile(suffix='.bin') as t:
        t.write(img[a - base:a - base + n]); t.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB',
                              f'--adjust-vma=0x{a:x}', '-M', 'gpr-names=32', t.name], capture_output=True, text=True).stdout
    print(f"== {f['name']} @ {s} ({n} B) {mp.get(('ovl_'+x, f['name']), '')}")
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if not m: continue
        ins = m.group(3)
        j = re.search(r'^jal\s+(0x)?([0-9a-f]+)', ins)
        if j: ins += '   <' + lab(int(j.group(2), 16)) + '>'
        print(f'  {m.group(1)}: {ins}')
