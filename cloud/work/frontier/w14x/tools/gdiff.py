#!/usr/bin/env python3
"""gdiff.py FILE.c FN KEEP[,KEEP]: aligned want/got disassembly for one -O3 group file (runs on builder, lane scratch)."""
import sys, difflib, tempfile, json
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
src, fn, keep = Path(sys.argv[1]), sys.argv[2], [k for k in sys.argv[3].split(',') if k]
flags = '-g0 -O3 -mips2 -G 0 -non_shared'
want = score.targets()[fn]; syms = score.image_symbols()
with tempfile.TemporaryDirectory() as t:
    obj = Path(t) / 'o.o'; g = Path(t) / 'g'; g.mkdir()
    (g / 'group.c').write_text(src.read_text())
    (g / 'group.json').write_text(json.dumps({'files': ['group.c'], 'keep': keep, 'flags': flags}))
    score.compile_group(g, obj)
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[fn]; end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    res, masks, *_ = score.relocate(obj, words, start, end, syms)
    got = res[start // 4:end // 4]
while got and got[-1] == 0 and len(got) > len(want): got.pop()
def dis(w):
    try: return score.disasm_word(w) or '?'
    except Exception: return '?'
sm = difflib.SequenceMatcher(None, want, got, autojunk=False)
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal': continue
    for i in range(max(i2 - i1, j2 - j1)):
        a = i1 + i; b = j1 + i
        wa = '%08x %-28s' % (want[a], dis(want[a])[:28]) if a < i2 else ' ' * 37
        gb = '%08x %-28s' % (got[b], dis(got[b])[:28]) if b < j2 else ''
        print('%3d/%3d %s | %s' % (a, b, wa, gb))
print('want %d got %d' % (len(want), len(got)))
