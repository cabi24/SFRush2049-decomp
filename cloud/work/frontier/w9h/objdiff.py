#!/usr/bin/env python3
"""objdiff.py OBJ FN [--all]: (builder, agent copy root) aligned diff of FN in OBJ vs retail."""
import sys, difflib
sys.path.insert(0, 'tools/cloud'); sys.path.insert(0, 'cand')
import score
from bfull import dis
obj, fn = sys.argv[1], sys.argv[2]; allrows = '--all' in sys.argv
want = score.targets()[fn]
words = score.text_words(obj); fns = score.symbols(obj)
start = fns[fn]; end = min((o for o in fns.values() if o > start), default=len(words) * 4)
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
        for k in range(max(i2 - i1, j2 - j1)):
            rows.append(('|', i1 + k if i1 + k < i2 else None, dw[i1 + k] if i1 + k < i2 else '', dg[j1 + k] if j1 + k < j2 else ''))
for r in rows:
    if allrows or r[0] == '|':
        print('%s %5s  %-32s %s' % (r[0], ('+%03x' % (r[1] * 4)) if r[1] is not None else '', r[2], r[3]))
print('want %d words, got %d; differing rows %d' % (len(want), len(got), sum(1 for r in rows if r[0] == '|')))
