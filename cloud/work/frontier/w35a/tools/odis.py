#!/usr/bin/env python3
"""odis.py NAME [image=a|b]: disassemble an overlay function from asm/us/ovl_X with symbol names
for jal targets and lui/addiu/lw data addresses."""
import json, re, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
name = sys.argv[1]
img = sys.argv[2] if len(sys.argv) > 2 else 'a'
d = ROOT / 'asm/us' / f'ovl_{img}'
sym = json.load(open(d / 'symbols.json'))['symbols']
sym = {k: int(v, 16) for k, v in sym.items()}
rev = {}
for k, v in sym.items(): rev.setdefault(v, []).append(k)
txt = open(next(d.glob('*.s'))).read()
m = re.search(r'\.section \.text\.%s,.*?\n(.*?)(?=\n\.section|\Z)' % name, txt, re.S)
words = [int(x, 16) for x in re.findall(r'\.word (0x[0-9A-Fa-f]+)', m.group(1))]
va = sym[name]
with tempfile.NamedTemporaryFile(suffix='.bin') as f:
    f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
    out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB',
                          f'--adjust-vma=0x{va:x}', '-M', 'gpr-names=32', f.name], capture_output=True, text=True).stdout
def nm(a):
    if a in rev: return rev[a][0]
    # nearest lower symbol that is a data object
    best = None
    for k, v in sym.items():
        if v <= a and k.startswith('D_') and (best is None or v > best[1]): best = (k, v)
    if best and a - best[1] < 0x2000: return f'{best[0]}+0x{a-best[1]:x}'
    return None
luis = {}
print(f'== {name} @ 0x{va:08X} ({len(words)} words)')
for line in out.splitlines():
    mm = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
    if not mm: continue
    ins = mm.group(3)
    j = re.search(r'jal\s+(0x)?([0-9a-f]+)', ins)
    note = ''
    if j:
        t = int(j.group(2), 16); note = ' <' + (nm(t) or '?') + '>'
    ml = re.match(r'lui\s+(\w+),(0x[0-9a-f]+|\d+)', ins)
    if ml: luis[ml.group(1)] = int(ml.group(2), 0) << 16
    mo = re.match(r'(addiu|lw|lh|lhu|lb|lbu|sw|sh|sb|lwc1|swc1|ori)\s+(\w+),(-?\d+|0x[0-9a-f]+)\((\w+)\)', ins)
    if mo and mo.group(4) in luis:
        off = int(mo.group(3), 0)
        a = (luis[mo.group(4)] + off) & 0xffffffff
        n = nm(a); note = f' ; 0x{a:08X}' + (f' {n}' if n else '')
    mo2 = re.match(r'addiu\s+(\w+),(\w+),(-?\d+)', ins)
    if mo2 and mo2.group(2) in luis:
        a = (luis[mo2.group(2)] + int(mo2.group(3))) & 0xffffffff
        n = nm(a); note = f' ; 0x{a:08X}' + (f' {n}' if n else '')
    print(f'  {mm.group(1)}: {ins}{note}')
