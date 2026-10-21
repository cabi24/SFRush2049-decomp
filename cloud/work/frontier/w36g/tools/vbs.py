#!/usr/bin/env python3
"""vbs.py VARIANT_DIR FN [flags]  (builder): score every VARIANT_DIR/*.c as a single (image A by default)."""
import sys, os, re, tempfile, difflib
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
score.ASM_DIR = Path(os.environ.get('TARGETS', 'asm/us/ovl_a')).resolve()
sys.path.insert(0, '.')
from adiff_lib import dis, norm_regs
vdir, fn = Path(sys.argv[1]), sys.argv[2]
flags = sys.argv[3] if len(sys.argv) > 3 else '-g0 -O3 -mips2 -G 0 -non_shared'
want = score.targets()[fn]
dw = dis(want)
res = []
for v in sorted(vdir.glob('*.c')):
    with tempfile.TemporaryDirectory() as t:
        obj = Path(t) / 'o.o'
        try:
            score.compile_single(str(v), flags, obj)
        except SystemExit as e:
            res.append((10**6, v.name, 'COMPILE FAIL ' + str(e)[:150])); continue
        words = score.text_words(obj); syms = score.symbols(obj)
        start = syms[fn]
        end = min((o for o in syms.values() if o > start), default=len(words) * 4)
        r, masks, unres, unver, errs = score.relocate(obj, words, start, end, score.image_symbols())
        got = r[start // 4:end // 4]
        while got and got[-1] == 0 and len(got) > len(want): got.pop()
        dg = dis(got)
        sm = difflib.SequenceMatcher(None, dw, dg, autojunk=False)
        rows = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != 'equal')
        kw, kg = [norm_regs(x) for x in dw], [norm_regs(x) for x in dg]
        sm = difflib.SequenceMatcher(None, kw, kg, autojunk=False)
        nrows = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != 'equal')
        res.append((rows, v.name, 'rows %d norm %d len %d/%d' % (rows, nrows, len(got), len(want))))
for rows, name, s in sorted(res):
    print('%6d %-14s %s' % (rows, name, s))
