#!/usr/bin/env python3
"""tdis.py NAME [NAME...]: disassemble the retail words of game functions at
their vaddr, with jal targets named from symbols.json. Needs
binutils-mips-linux-gnu. Cloud Lane A helper.
"""
from pathlib import Path
import json, struct, subprocess, sys, tempfile, re
ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
syms = {k: int(v, 16) for k, v in json.load(open(ROOT / 'asm/us/blob/symbols.json'))['symbols'].items()}
rev = {}
for k, v in syms.items(): rev.setdefault(v, []).append(k)
for name in sys.argv[1:]:
    words = score.targets()[name]; va = syms[name]
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB',
                              f'--adjust-vma=0x{va:x}', '-M', 'gpr-names=32', f.name], capture_output=True, text=True).stdout
    print(f'== {name} @ 0x{va:08X} ({len(words)} words)')
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if not m: continue
        ins = m.group(3)
        j = re.search(r'jal\s+(0x)?([0-9a-f]+)', ins)
        if j: ins += '   <' + ','.join(rev.get(int(j.group(2),16), ['?'])) + '>'
        print(f'  {m.group(1)}: {ins}')
