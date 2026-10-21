#!/usr/bin/env python3
"""odis.py IMG NAME...: disassemble overlay function(s) of image IMG (a|b) with symbolised jal/lui+lo targets."""
import json, re, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
img = sys.argv[1]
d = ROOT / f'asm/us/ovl_{img}'
syms = {}
for p in (ROOT / 'asm/us/blob/symbols.json', d / 'symbols.json'):
    for k, v in json.load(open(p))['symbols'].items():
        syms.setdefault(int(v, 16), k)
try:
    ds = json.load(open(ROOT / 'asm/us/blob_data/symbols.json'))['symbols']
    for k, v in ds.items(): syms.setdefault(int(v, 16), k)
except Exception: pass
words = {}
cur = None
for line in open(d / f'ovl_{img}_8038a400.s'):
    m = re.match(r'\.section \.text\.(\S+?),', line.strip())
    if m: cur = words.setdefault(m.group(1), []); continue
    if line.strip().startswith('.section'): cur = None; continue
    m = re.match(r'\s*\.word\s+(0x[0-9A-Fa-f]+)', line)
    if m and cur is not None: cur.append(int(m.group(1), 16))
ov = {k: int(v, 16) for k, v in json.load(open(d / 'symbols.json'))['symbols'].items()}
def name(a):
    return syms.get(a, '')
for n in sys.argv[2:]:
    w = words[n]; va = ov[n]
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(w)}I', *w)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB', f'--adjust-vma=0x{va:x}', '-M', 'gpr-names=32', f.name], capture_output=True, text=True).stdout
    print(f'== {n} @ 0x{va:08X} ({len(w)} words, {len(w)*4} B)')
    regs = {}
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if not m: continue
        ins = m.group(3)
        j = re.search(r'\bjal\s+(0x)?([0-9a-f]+)', ins)
        if j: ins += '   <' + (name(int(j.group(2), 16)) or '?') + '>'
        l = re.match(r'lui\s+(\w+),(0x[0-9a-f]+)', ins)
        if l: regs[l.group(1)] = int(l.group(2), 16) << 16
        a = re.match(r'(addiu|ori)\s+(\w+),(\w+),(-?0x[0-9a-f]+|-?\d+)', ins)
        if a and a.group(3) in regs:
            v = (regs[a.group(3)] + int(a.group(4), 0)) & 0xffffffff
            ins += f'   ; {v:08X} {name(v)}'
            regs[a.group(2)] = v
        b = re.match(r'(lw|sw|lh|lhu|sh|lb|lbu|sb|lwc1|swc1|ldc1|sdc1)\s+(\w+),(-?\d+|-?0x[0-9a-f]+)\((\w+)\)', ins)
        if b and b.group(4) in regs:
            v = (regs[b.group(4)] + int(b.group(3), 0)) & 0xffffffff
            ins += f'   ; {v:08X} {name(v)}'
        print(f'  {m.group(1)}: {m.group(2)} {ins}')
