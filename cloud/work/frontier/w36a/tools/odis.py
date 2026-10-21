#!/usr/bin/env python3
"""odis.py NAME [-r]: disassemble an image-A overlay function with symbolised
lui/addiu/lw/sw pairs (data symbols from ovl_a + blob symbols.json)."""
import json, re, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path('/home/cburnes/projects/rush2049-decomp')
img = 'ovl_a'
args = [a for a in sys.argv[1:] if not a.startswith('-')]
if '-b' in sys.argv: img = 'ovl_b'
sa = json.load(open(ROOT / f'asm/us/{img}/symbols.json'))['symbols']
sb = json.load(open(ROOT / 'asm/us/blob/symbols.json'))['symbols']
syms = {}
for d in (sb, sa):
    for k, v in d.items(): syms[k] = int(v, 16)
rev = {}
for k, v in syms.items(): rev.setdefault(v, []).append(k)
# words per text section
txt = (ROOT / f'asm/us/{img}/{img}_8038a400.s').read_text().split('\n')
secs = {}; cur = None
for l in txt:
    m = re.match(r'\.section \.text\.(\w+)', l)
    if m: cur = m.group(1); secs[cur] = []; continue
    m = re.match(r'\s+\.word 0x([0-9A-Fa-f]{8})', l)
    if m and cur: secs[cur].append(int(m.group(1), 16))
import bisect
_addrs = sorted(set(rev))
def nm(a):
    i = bisect.bisect_right(_addrs, a) - 1
    if i >= 0 and a - _addrs[i] < 0x400:
        base = _addrs[i]
        n = sorted(rev[base], key=lambda s: (s.startswith('D_') is False and False, s))[0]
        n = [x for x in rev[base] if not x.startswith('func_')] or rev[base]
        n = n[0]
        return n if a == base else f'{n}+{a-base:#x}'
    return f'{a:#010x}'
def name_of(a):
    if a in rev:
        c = rev[a]
        c = sorted(c, key=lambda s: (s.startswith('D_'), s))
        return c[0]
    # nearest below within 0x100 for data
    return None
for name in args:
    words = secs[name]; va = syms[name]
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB',
                              f'--adjust-vma=0x{va:x}', '-M', 'gpr-names=32', f.name], capture_output=True, text=True).stdout
    print(f'== {name} @ 0x{va:08X} ({len(words)} words, {len(words)*4} bytes)')
    regs = {}
    import bisect
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if not m: continue
        ins = m.group(3); ad = int(m.group(1), 16)
        lm = re.match(r'lui\s+(\w+),(0x[0-9a-f]+)', ins)
        if lm: regs[lm.group(1)] = int(lm.group(2), 16) << 16; 
        else:
            mm2 = re.match(r'(addiu|ori)\s+(\w+),(\w+),(-?\d+|-?0x[0-9a-f]+)$', ins)
            mm = re.match(r'(lw|sw|lh|lhu|sh|lb|lbu|sb|lwc1|swc1)\s+(\w+),(-?0x[0-9a-f]+|-?\d+)\((\w+)\)', ins)
            if mm2 and mm2.group(3) in regs and (regs[mm2.group(3)] >= 0x80000000):
                v = (regs[mm2.group(3)] + int(mm2.group(4), 0)) & 0xffffffff
                ins += '   # ' + nm(v)
                regs[mm2.group(2)] = v
            elif mm and mm.group(4) in regs and regs[mm.group(4)] >= 0x80000000:
                v = (regs[mm.group(4)] + int(mm.group(3), 0)) & 0xffffffff
                ins += '   # ' + nm(v)
                if mm.group(1) in ('lw','lh','lhu','lb','lbu','lwc1'): regs.pop(mm.group(2), None)
            else:
                # clobber dest of other ops (rough)
                d = re.match(r'\w+\s+(\w+),', ins)
                if d and d.group(1) in regs and not ins.startswith(('sw','sh','sb','swc1','b','j')): regs.pop(d.group(1), None)
        j = re.search(r'jal\s+(0x)?([0-9a-f]+)', ins)
        if j: ins += '   <' + ','.join(rev.get(int(j.group(2),16), ['?'])) + '>'
        print(f'  {ad:08x}: {m.group(2)} {ins}')
