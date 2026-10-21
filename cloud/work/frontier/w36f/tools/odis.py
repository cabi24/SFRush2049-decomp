#!/usr/bin/env python3
"""odis.py IMG NAME...: disassemble overlay function (IMG = a|b) from asm/us/ovl_IMG; jal targets named
from the image symbols.json and the blob symbols.json. Lane w36f helper."""
import json, re, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
img = sys.argv[1]
d = ROOT / 'asm/us' / f'ovl_{img}'
sym = {k: int(v, 16) for k, v in json.load(open(d / 'symbols.json'))['symbols'].items()}
try:
    for k, v in json.load(open(ROOT / 'asm/us/blob/symbols.json'))['symbols'].items():
        sym.setdefault(k, int(v, 16))
except Exception: pass
rev = {}
for k, v in sym.items(): rev.setdefault(v, []).append(k)
text = (next(d.glob('ovl_*.s'))).read_text().splitlines()
fn = {}
cur = None
for l in text:
    m = re.match(r'\.section \.text\.(\S+?),', l.strip())
    if m: cur = fn.setdefault(m.group(1), []); continue
    if l.strip().startswith('.section'): cur = None; continue
    m = re.match(r'\s*\.word\s+(0x[0-9A-Fa-f]+)', l)
    if m and cur is not None: cur.append(int(m.group(1), 16))
for name in sys.argv[2:]:
    words = fn[name]; va = sym[name]
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB',
                              f'--adjust-vma=0x{va:x}', '-M', 'gpr-names=32', f.name], capture_output=True, text=True).stdout
    print(f'== {name} @ 0x{va:08X} ({len(words)} words, {len(words)*4} B)')
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if not m: continue
        ins = m.group(3)
        j = re.search(r'jal\s+(0x)?([0-9a-f]+)', ins)
        if j: ins += '   <' + ','.join(rev.get(int(j.group(2),16), ['?'])) + '>'
        print(f'  {m.group(1)}: {m.group(2)} {ins}')
