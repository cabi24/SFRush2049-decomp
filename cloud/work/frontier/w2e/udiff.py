#!/usr/bin/env python3
"""udiff.py NAME [--all] [--tag w2e]: aligned diff of NAME in build/blob_unit/<tag>/unit.o (left by
`blob_unit --tag <tag> score ...`) against the retail words. Relocated fields (jal targets, %hi/%lo
immediates) are masked on both sides, so this is a reading aid, not a gate."""
import sys, struct, subprocess, tempfile, difflib, re, json
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT)); sys.path.insert(0, str(ROOT / 'tools' / 'cloud'))
import score
from tools.conveyor.pipeline import blob_unit
args = [a for a in sys.argv[1:] if not a.startswith('--')]
tag = 'w2e'
if '--tag' in sys.argv: tag = sys.argv[sys.argv.index('--tag') + 1]; args.remove(tag)
name = args[0]
obj = blob_unit.Obj.parse((ROOT / 'build/blob_unit' / tag / 'unit.o').read_bytes())
funcs = obj.functions(); text = obj.data('.text')
start = funcs[name]; end = min((o for o in funcs.values() if o > start), default=len(text))
got = list(struct.unpack('>%dI' % ((end - start) // 4), text[start:end]))
want = score.targets()[name]
while got and got[-1] == 0 and len(got) > len(want): got.pop()
MASK = {4: 0xFC000000, 5: 0xFFFF0000, 6: 0xFFFF0000}
gm = {}
for off, typ, sym in obj.relocs.get('.text', []):
    if start <= off < end and typ in MASK: gm[(off - start) // 4] = MASK[typ]
def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack('>%dI' % len(words), *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-z', '-b', 'binary', '-m', 'mips:4300', '-EB', f.name], capture_output=True, text=True).stdout
    return [re.sub(r'\s+', ' ', m.group(1)) for m in (re.match(r'\s*[0-9a-f]+:\s+[0-9a-f]{8}\s+(.*)', l) for l in out.splitlines()) if m]
def key(w):
    op = w >> 26
    if op == 3: return w & 0xFC000000
    return w
gk = [g & gm.get(i, 0xFFFFFFFF) for i, g in enumerate(got)]
# mask want words the same way when the got word at the matched position is relocated: approximate by masking
# every want word whose masked form equals some masked got form class (jal / lui / lo16 users)
def wkey(w):
    op = w >> 26
    if op == 3: return w & 0xFC000000
    return w
relk = set(g & gm[i] for i, g in enumerate(got) if i in gm)
plain = set(g for i, g in enumerate(got) if i not in gm)
wk = []
for w in want:
    k = w
    for m in (() if w in plain else (0xFC000000, 0xFFFF0000)):
        if (w & m) in relk and ((w >> 26) == 3 or m == 0xFFFF0000): k = w & m; break
    wk.append(k); continue
    for m in (0xFC000000, 0xFFFF0000):
        if (w & m) in relk and ((w >> 26) == 3 or m == 0xFFFF0000): k = w & m; break
    wk.append(k)
gk2 = [g & gm.get(i, 0xFFFFFFFF) for i, g in enumerate(got)]
dw, dg = dis(want), dis(got)
sm = difflib.SequenceMatcher(None, wk, gk2, autojunk=False)
rows = []
for t, i1, i2, j1, j2 in sm.get_opcodes():
    if t == 'equal':
        for k in range(i2 - i1): rows.append((' ', i1 + k, dw[i1 + k], dg[j1 + k]))
    else:
        for k in range(max(i2 - i1, j2 - j1)):
            rows.append(('|', i1 + k if i1 + k < i2 else None, dw[i1 + k] if i1 + k < i2 else '', dg[j1 + k] if j1 + k < j2 else ''))
al = '--all' in sys.argv
show = [al or r[0] == '|' for r in rows]
for i, r in enumerate(rows):
    if show[i] or (i > 0 and show[i - 1]) or (i + 1 < len(rows) and show[i + 1]):
        print('%s %5s  %-32s %s' % (r[0], ('+%03x' % (r[1] * 4)) if r[1] is not None else '', r[2], r[3]))
print('want %d words, got %d; differing rows %d (relocated fields masked)' % (len(want), len(got), sum(1 for r in rows if r[0] == '|')))
