#!/usr/bin/env python3
"""udiff.py NAME [--all] [--tag w2f]: instruction-aligned diff of NAME in build/blob_unit/<tag>/unit.o (the last
`blob_unit --tag <tag> score` build) against the retail words. Relocated fields (HI16/LO16 immediates, jal targets)
are masked for alignment only; this is a reading aid, never a score."""
import sys, struct, subprocess, tempfile, re, difflib, bisect
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT)); sys.path.insert(0, str(ROOT / 'tools' / 'cloud'))
import score
from tools.conveyor.pipeline import blob_unit as bu
args = [a for a in sys.argv[1:] if not a.startswith('--')]
tag = 'w2f'
if '--tag' in sys.argv: tag = sys.argv[sys.argv.index('--tag') + 1]; args.remove(tag)
name = args[0]
obj = bu.Obj.parse((ROOT / 'build/blob_unit' / tag / 'unit.o').read_bytes())
text = obj.data('.text'); funcs = obj.functions()
starts = sorted(set(funcs.values())); st = funcs[name]
en = next((s for s in starts if s > st), len(text))
got = list(struct.unpack('>%dI' % ((en - st) // 4), text[st:en]))
want = score.targets()[name]
while got and got[-1] == 0 and len(got) > len(want): got.pop()
mask = {}
for off, rtype, symndx in obj.relocs.get('.text', []):
    if st <= off < en: mask[(off - st) // 4] = 0xFC000000 if rtype == 4 else 0xFFFF0000
def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack('>%dI' % len(words), *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-z', '-b', 'binary', '-m', 'mips:4300', '-EB', f.name], capture_output=True, text=True).stdout
    r = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m: r.append(re.sub(r'\s+', ' ', m.group(3)))
    return r
def key(w):
    op = w >> 26
    if op == 3: return w & 0xFC000000
    return w
gm = [w & mask.get(i, 0xFFFFFFFF) for i, w in enumerate(got)]
# mask the same class of retail words: lui/addiu/load/store with symbol are unknown; approximate by masking retail jal and
# any retail word whose opcode/regs equal a masked got word at alignment time (done via key on opcode+regs)
def k2(w, m): return w & m
wk = [key(w) for w in want]; gk = [key(w) if i not in mask else (w & mask[i]) | 1 << 40 for i, w in enumerate(got)]
# for alignment, compare on (opcode,rs,rt) for masked got words
wk2 = []
gset = {(w & 0xFFFF0000) for i, w in enumerate(got) if mask.get(i) == 0xFFFF0000}
for w in want:
    wk2.append(((w & 0xFFFF0000) | 1 << 40) if (w & 0xFFFF0000) in gset and (w >> 26) in (0x0F, 0x09, 0x23, 0x2B, 0x21, 0x25, 0x20, 0x24, 0x28, 0x29, 0x31, 0x39, 0x35, 0x3D) else key(w))
dw, dg = dis(want), dis(got)
sm = difflib.SequenceMatcher(None, wk2, gk, autojunk=False)
show_all = '--all' in sys.argv; n = 0
rows = []
for tagc, i1, i2, j1, j2 in sm.get_opcodes():
    if tagc == 'equal':
        for k in range(i2 - i1): rows.append((' ', i1 + k, dw[i1 + k], dg[j1 + k]))
    else:
        for k in range(max(i2 - i1, j2 - j1)):
            rows.append(('|', i1 + k if i1 + k < i2 else None, dw[i1 + k] if i1 + k < i2 else '', dg[j1 + k] if j1 + k < j2 else '')); n += 1
for idx, (c, i, a, b) in enumerate(rows):
    if show_all or c == '|' or (idx > 0 and rows[idx - 1][0] == '|') or (idx + 1 < len(rows) and rows[idx + 1][0] == '|'):
        print('%s %5s  %-34s %s' % (c, ('+%03x' % (i * 4)) if i is not None else '', a, b))
print('want %d words, got %d; differing rows %d' % (len(want), len(got), n))
