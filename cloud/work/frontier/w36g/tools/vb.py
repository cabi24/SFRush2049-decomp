#!/usr/bin/env python3
"""vb.py TEMPLATE_DIR VARIANT_DIR FN [FN2...]  (builder, inside the lane scratch)
TEMPLATE_DIR holds group.json + group.c with a line '/*@BODY@*/'; each VARIANT_DIR/*.c replaces that line.
Prints per variant: strict differing words (aligned rows), norm rows, frame want/got, for each FN."""
import sys, os, re, json, shutil, tempfile, difflib, subprocess, struct
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
score.ASM_DIR = Path(os.environ.get('TARGETS', 'asm/us/ovl_a')).resolve()
sys.path.insert(0, '.')
from adiff_lib import dis, norm_regs
tdir, vdir, fns = Path(sys.argv[1]), Path(sys.argv[2]), sys.argv[3:]
srcs = {f.name: f.read_text() for f in tdir.glob('*.c')}
mark = [n for n, t in srcs.items() if '/*@BODY@*/' in t]
assert len(mark) == 1, mark
targets = score.targets()
res = []
for v in sorted(vdir.glob('*.c')):
    with tempfile.TemporaryDirectory() as t:
        g = Path(t) / 'g'; g.mkdir()
        shutil.copy(tdir / 'group.json', g / 'group.json')
        for n, txt in srcs.items():
            (g / n).write_text(txt.replace('/*@BODY@*/', v.read_text()) if n == mark[0] else txt)
        obj = Path(t) / 'o.o'
        try:
            score.compile_group(g, obj)
        except SystemExit as e:
            res.append((10**6, v.name, 'COMPILE FAIL ' + str(e)[:200])); continue
        words = score.text_words(obj); syms = score.symbols(obj)
        out = []; tot = 0
        for fn in fns:
            want = targets[fn]
            start = syms[fn]
            end = min((o for o in syms.values() if o > start), default=len(words) * 4)
            r, masks, unres, unver, errs = score.relocate(obj, words, start, end, score.image_symbols())
            got = r[start // 4:end // 4]
            while got and got[-1] == 0 and len(got) > len(want): got.pop()
            dw, dg = dis(want), dis(got)
            sm = difflib.SequenceMatcher(None, dw, dg, autojunk=False)
            rows = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != 'equal')
            kw, kg = [norm_regs(x) for x in dw], [norm_regs(x) for x in dg]
            sm = difflib.SequenceMatcher(None, kw, kg, autojunk=False)
            nrows = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != 'equal')
            fr = lambda l: next((-int(re.match(r'addiu sp,sp,(-?\d+)', x).group(1)) for x in l if re.match(r'addiu sp,sp,(-?\d+)', x)), 0)
            exact = sum(1 for a, b in zip(want, got) if a == b) if len(want) == len(got) else -1
            def slots(l):
                o = set()
                for x in l:
                    for m in re.findall(r',(-?\d+)\(sp\)', x): o.add(int(m))
                    for m in re.findall(r'addiu \w+,sp,(\d+)$', x): o.add(int(m))
                return o
            sw, sg = slots(dw), slots(dg)
            sl = '' if sw == sg else ' slots-want %s got %s' % (sorted(x for x in sw - sg if x >= 40), sorted(x for x in sg - sw if x >= 40))
            out.append('%s: rows %d norm %d len %d/%d frame %d/%d posdiff %s%s' % (fn[-6:], rows, nrows, len(got), len(want), fr(dg), fr(dw), (len(want) - exact) if exact >= 0 else '-', sl))
            tot += rows
        res.append((tot, v.name, ' | '.join(out)))
for tot, name, s in sorted(res):
    print('%6d %-24s %s' % (tot, name, s))
