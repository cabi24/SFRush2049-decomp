#!/usr/bin/env python3
"""udiff.py FN [--all] [--obj build/blob_unit/w11e/unit.o]: aligned diff of FN in the blob_unit object vs retail (runs on the Pi)."""
import sys, argparse, tempfile, difflib, struct, subprocess, re
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score

def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack('>%dI' % len(words), *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-z', '-b', 'binary', '-m', 'mips:4300', '-EB', f.name],
                             capture_output=True, text=True).stdout
    r = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m: r.append(re.sub(r'\s+', ' ', m.group(3)))
    return r

ap = argparse.ArgumentParser(); ap.add_argument('fn'); ap.add_argument('--all', action='store_true'); ap.add_argument('--obj', default='build/blob_unit/w11e/unit.o')
a = ap.parse_args()
want = score.targets()[a.fn]
obj = Path(a.obj)
words = score.text_words(obj); fns = score.symbols(obj)
start = fns[a.fn]
end = min((o for o in fns.values() if o > start), default=len(words) * 4)
res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, score.image_symbols())
got = res[start // 4:end // 4]
while got and got[-1] == 0 and len(got) > len(want): got.pop()
dw, dg = dis(want), dis(got)
sm = difflib.SequenceMatcher(None, want, got, autojunk=False)
rows = []
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal':
        for k in range(i2 - i1): rows.append((' ', i1 + k, dw[i1 + k], dg[j1 + k]))
    else:
        n = max(i2 - i1, j2 - j1)
        for k in range(n):
            rows.append(('|', i1 + k if i1 + k < i2 else None, dw[i1 + k] if i1 + k < i2 else '', dg[j1 + k] if j1 + k < j2 else ''))
show = [a.all or r[0] == '|' for r in rows]
for i, r in enumerate(rows):
    if show[i] or (i > 0 and show[i - 1]) or (i + 1 < len(rows) and show[i + 1]):
        print('%s %5s  %-32s %s' % (r[0], ('+%03x' % (r[1] * 4)) if r[1] is not None else '', r[2], r[3]))
print('want %d words, got %d; differing rows %d; unverified %d unresolved %d' % (len(want), len(got), sum(1 for r in rows if r[0] == '|'), len(unverified), len(unresolved)))
