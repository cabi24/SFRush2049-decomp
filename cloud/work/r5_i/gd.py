#!/usr/bin/env python3
"""gd.py GROUPDIR FN: aligned diff of compiled group member vs retail."""
import sys, tempfile, difflib
from pathlib import Path
ROOT = Path('/home/user/SFRush2049-decomp')
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
gd = Path(sys.argv[1]); fn = sys.argv[2]
with tempfile.TemporaryDirectory() as t:
    obj = Path(t) / 'o.o'
    score.compile_group(gd, obj)
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[fn]; end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    res, masks, *_ = score.relocate(obj, words, start, end, score.image_symbols())
    got = res[start // 4:end // 4]
want = score.targets()[fn]
gm = [g & masks.get(start + 4 * i, 0xFFFFFFFF) for i, g in enumerate(got)]
wm = [w & masks.get(start + 4 * i, 0xFFFFFFFF) for i, w in enumerate(want)]
d = lambda w: score.disasm_word(w)
sm = difflib.SequenceMatcher(None, wm, gm, autojunk=False)
print(f'{fn}: want {len(wm)} got {len(gm)}')
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal': print(f'   = {i2-i1} words'); continue
    for k in range(max(i2-i1, j2-j1)):
        a = d(wm[i1+k]) if i1+k < i2 else ''; b = d(gm[j1+k]) if j1+k < j2 else ''
        print(f'  {"!" if tag=="replace" else ("-" if tag=="delete" else "+")} {i1+k:3d} {a:38s} | {b}')
