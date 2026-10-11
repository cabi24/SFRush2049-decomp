#!/usr/bin/env python3
"""wdiff.py FILE.c FN [flags]: run on the builder (lane scratch with tools/cloud). Prints the word-aligned
diff of retail (want) vs ours (got) with mnemonics from mips-linux-gnu-objdump."""
import re, sys, tempfile, difflib, subprocess, struct
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
src, fn = Path(sys.argv[1]), sys.argv[2]
flags = sys.argv[3] if len(sys.argv) > 3 else '-g0 -O3 -mips2 -G 0 -non_shared'
want = score.targets()[fn]; syms = score.image_symbols()
def dis_words(ws):
    with tempfile.TemporaryDirectory() as t:
        p = Path(t) / 'w.bin'; p.write_bytes(b''.join(struct.pack('>I', w) for w in ws))
        out = subprocess.run(['mips-linux-gnu-objdump', '-b', 'binary', '-m', 'mips:isa32r2', '-EB', '-D', str(p)],
                             capture_output=True, text=True).stdout
    txt = []
    for line in out.splitlines():
        parts = line.split('\t')
        if len(parts) >= 3 and re.match(r'^\s*[0-9a-f]+:$', parts[0]):
            txt.append(parts[2].strip())
    return txt[:len(ws)] + [''] * max(0, len(ws) - len(txt))
with tempfile.TemporaryDirectory() as t:
    obj = Path(t) / 'o.o'
    score.compile_single(str(src), flags, obj)
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[fn]
    end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, syms)
    got = res[start // 4:end // 4]
mw = dis_words(want); mg = dis_words(got)
sm = difflib.SequenceMatcher(None, want, got, autojunk=False)
n = 0
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal': continue
    for k in range(max(i2 - i1, j2 - j1)):
        i, j = i1 + k, j1 + k
        w = ('%08x %s' % (want[i], mw[i])) if i < i2 else '-'
        g = ('%08x %s' % (got[j], mg[j])) if j < j2 else '-'
        print('%-7s %3d  W %-40s | G %s' % (tag, i, w, g)); n += 1
print('blocks/rows printed', n)
