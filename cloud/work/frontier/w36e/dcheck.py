#!/usr/bin/env python3
"""rocheck.py OBJ NAME [NAME...]: verify each own-.rodata HI16/LO16 pair of NAME against the retail image.
For every pair: object rodata offset (from the object's REL addends) -> bytes; retail address (from the retail words)
-> image bytes; compare 4 bytes for lwc1/lw-float, NUL-terminated string for addiu, and report jump-table entries."""
import sys, struct
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
obj, names = sys.argv[1], sys.argv[2:]
img = open(ROOT / 'build/ovl/a/image.bin', 'rb').read(); BASE = 0x8038A400
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score
score.ASM_DIR = ROOT / 'asm/us/ovl_a'
data, secs = score._elf(Path(obj))
by = {s['name']: (i, s) for i, s in enumerate(secs)}
import os
SEC=os.environ.get('SEC','.rodata')
ro = by[SEC][1]; rodata = data[ro['off']:ro['off'] + ro['size']] if ro['type'] != 8 else b'\0'*ro['size']
tw = score.text_words(Path(obj))
text_i = by['.text'][0]
funcs = score.symbols(Path(obj)); targets = score.targets()
rel = next((i, s) for i, s in enumerate(secs) if s['type'] == 9 and s['info'] == text_i)
syms = score._symbol_table(data, secs, rel[1]['link'])
recs = []
for k in range(rel[1]['size'] // 8):
    off, info = struct.unpack_from('>II', data, rel[1]['off'] + 8 * k)
    recs.append((off, info & 0xff, syms[info >> 8]['name']))
def sx(v): return v - 0x10000 if v & 0x8000 else v
bad = 0; n = 0; base_offsets = set()
for name in names:
    start = funcs[name]; want = targets[name]; end = start + 4 * len(want)
    pend = []   # (hi off)
    for o, t, sn in recs:
        if not (start <= o < end) or sn != SEC: continue
        wi = (o - start) // 4
        if t == 5:
            pend.append(o)
        elif t == 6:
            # pick the nearest previous HI whose retail lui register matches this instruction's base register
            base = (want[wi] >> 21) & 31
            h = None
            for ho in reversed(pend):
                hw = want[(ho - start) // 4]
                if ((hw >> 16) & 31) == base: h = ho; break
            if h is None: print(name, hex(o - start), 'no HI'); bad += 1; continue
            hiw = want[(h - start) // 4]
            raddr = (((hiw & 0xffff) << 16) + sx(want[wi] & 0xffff)) & 0xffffffff
            ohi = tw[h // 4]; olo = tw[o // 4]
            ooff = (((ohi & 0xffff) << 16) + sx(olo & 0xffff))
            op = want[wi] >> 26
            n += 1
            if op in (0x31, 0x23, 0x39, 0x2b, 0x20, 0x24, 0x28, 0x21, 0x25, 0x29):      # lwc1/lw/swc1/sw/lb/lbu/sb/lh/lhu/sh
                a = rodata[ooff:ooff + 4]; b = img[raddr - BASE:raddr - BASE + 4]
                kind = 'f32' if op in (0x31,0x39) else ('lw' if op == 0x23 and SEC=='.rodata' else 'w')
            else:                       # addiu: string
                end_o = rodata.index(b'\0', ooff); a = rodata[ooff:end_o + 1]
                b = img[raddr - BASE:raddr - BASE + len(a)]; kind = 'str'
            ok = (a == b)
            base_offsets.add((raddr - ooff) & 0xffffffff)
            if kind == 'lw': print(name, hex(o - start), kind, hex(raddr), 'jump-table base (entries relocated)')
            else:
                if not ok: bad += 1
                print(name, hex(o - start), kind, hex(raddr), 'OK' if ok else f'DIFF obj={a!r} retail={b!r}')
print('sites', n, 'bad', bad, 'distinct (retail addr - obj rodata offset):', sorted(hex(x) for x in base_offsets))
