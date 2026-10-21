#!/usr/bin/env python3
"""refchk.py OBJ FUNC[,FUNC] : per-reference own-.rodata content check (Pi, repo root, python3 -I).
For every HI16/LO16 pair against .rodata inside FUNC, decode the retail address from the retail instruction pair at
the same offset (image B) and compare our referenced word (jump-table words are .text-relative: mapped through
FUNC's start) with the retail word there."""
import re, struct, subprocess, sys, json
B = 0x8038A400
img = open('build/ovl/b/image.bin', 'rb').read()
syms = {k: int(v, 16) for k, v in json.load(open('asm/us/ovl_b/symbols.json'))['symbols'].items()}
obj = sys.argv[1]
def sec(name):
    return subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '--only-section=' + name, obj, '/dev/stdout'],
                          capture_output=True).stdout
ro, txt = sec('.rodata'), sec('.text')
nm = {l.split()[2]: int(l.split()[0], 16) for l in subprocess.run(['mips-linux-gnu-nm', obj], capture_output=True,
      text=True).stdout.splitlines() if len(l.split()) == 3}
rodata_rel = {}  # R_MIPS_32 inside .rodata (jump tables)
cur = None
rel = subprocess.run(['mips-linux-gnu-readelf', '-rW', obj], capture_output=True, text=True).stdout
for l in rel.splitlines():
    m = re.match(r"Relocation section '\.rel\.(\S+)'", l)
    if m: cur = m.group(1); continue
    m = re.match(r'\s*([0-9a-f]+)\s+[0-9a-f]+\s+(R_MIPS_\w+)\s+[0-9a-f]+\s+(\S+)', l)
    if m and cur == 'rodata' and m.group(2) == 'R_MIPS_32': rodata_rel[int(m.group(1), 16)] = m.group(3)
for f in sys.argv[2].split(','):
    fs = nm[f]; va = syms[f]; n = bad = 0; hi = None; cur = None
    fe = min([v for k, v in nm.items() if v > fs and k.startswith('func_')] + [len(txt)])
    for l in rel.splitlines():
        m = re.match(r"Relocation section '\.rel\.(\S+)'", l)
        if m: cur = m.group(1); continue
        m = re.match(r'\s*([0-9a-f]+)\s+[0-9a-f]+\s+(R_MIPS_\w+)\s+[0-9a-f]+\s+\.rodata', l)
        if not m or cur != 'text': continue
        off = int(m.group(1), 16)
        if not fs <= off < fe: continue
        w = struct.unpack('>I', txt[off:off + 4])[0]
        if m.group(2) == 'R_MIPS_HI16': hi = (off, w & 0xffff); continue
        lo = w & 0xffff; lo -= 0x10000 if lo & 0x8000 else 0
        secoff = (hi[1] << 16) + lo
        ra, rh = va + off - fs, va + hi[0] - fs
        rw, rhw = (struct.unpack('>I', img[a - B:a - B + 4])[0] for a in (ra, rh))
        rlo = rw & 0xffff; rlo -= 0x10000 if rlo & 0x8000 else 0
        raddr = ((rhw & 0xffff) << 16) + rlo
        ours = struct.unpack('>I', ro[secoff:secoff + 4])[0]
        if secoff in rodata_rel:  # jump table: .text offset -> image address via our function layout
            ours = ours - fs + va
        theirs = struct.unpack('>I', img[raddr - B:raddr - B + 4])[0]
        n += 1
        if ours != theirs:
            bad += 1; print(f'  {f} +0x{off - fs:x}: .rodata+0x{secoff:x} ours {ours:08x} retail@{raddr:08X} {theirs:08x}')
    print(f'{f}: {n} .rodata references, {bad} differ')
