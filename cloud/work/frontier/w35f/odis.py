#!/usr/bin/env python3
"""odis.py NAME [NAME...]: disassemble image-B overlay functions from asm/us/ovl_b/ovl_b_8038a400.s"""
import json, re, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
SYM = json.load(open(ROOT/'asm/us/ovl_b/symbols.json'))['symbols']
syms = {k:int(v,16) for k,v in SYM.items()}
rev = {}
for k,v in syms.items(): rev.setdefault(v,[]).append(k)
txt = open(ROOT/'asm/us/ovl_b/ovl_b_8038a400.s').read()
secs = {}
for m in re.finditer(r'\.section \.text\.(\w+), "ax", @progbits\n(?:\.globl \w+\n)?\w+:\n((?:\s+\.word 0x[0-9A-Fa-f]{8}\n)+)', txt):
    secs[m.group(1)] = [int(x,16) for x in re.findall(r'0x([0-9A-Fa-f]{8})', m.group(2))]
for name in sys.argv[1:]:
    words = secs[name]; va = syms[name]
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB',f'--adjust-vma=0x{va:x}','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
    print(f'== {name} @ 0x{va:08X} ({len(words)} words)')
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if not m: continue
        ins = m.group(3)
        j = re.search(r'jal\s+(0x)?([0-9a-f]+)', ins)
        if j: ins += '   <' + ','.join(rev.get(int(j.group(2),16), ['?'])) + '>'
        print(f'  {m.group(1)}: {m.group(2)} {ins}')
