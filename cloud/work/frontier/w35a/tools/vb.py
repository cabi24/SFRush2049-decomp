#!/usr/bin/env python3
"""vb.py (builder scratch root): vb.py PARTS_DIR VARIANT_DIR NAME... [--targets asm/us/ovl_a]
PARTS_DIR has head.c tail.c group.json; VARIANT_DIR/*.c are segments. Prints differing/total per NAME per variant."""
import json, sys, tempfile, shutil
from pathlib import Path
sys.path.insert(0, str(Path.cwd() / 'tools' / 'cloud'))
import score, difflib, json as _j
sys.path.insert(0, str(Path.cwd()))
import ovtool
args = sys.argv[1:]
tg = 'asm/us/ovl_a'
if '--targets' in args:
    i = args.index('--targets'); tg = args[i+1]; del args[i:i+2]
parts, vdir, names = Path(args[0]), Path(args[1]), args[2:]
score.ASM_DIR = Path(tg).resolve()
head = (parts/'head.c').read_text(); tail = (parts/'tail.c').read_text()
rows = []
for v in sorted(vdir.glob('*.c')):
    with tempfile.TemporaryDirectory() as t:
        t = Path(t)
        (t/'group.c').write_text(head + v.read_text() + tail)
        shutil.copy(parts/'group.json', t/'group.json')
        obj = t/'o.o'
        try:
            score.compile_group(t, obj)
        except SystemExit as e:
            rows.append((v.name, None, str(e)[:150].replace('\n', ' '))); continue
        res = []
        syms = score.image_symbols()
        rev = {}
        for k, vv in syms.items(): rev.setdefault(vv, []).append(k)
        words = score.text_words(obj); fns = score.symbols(obj)
        for n in names:
            try:
                r = score.compare(obj, n, show=0)
                want = score.targets()[n]
                st = fns[n]
                en = min((o for o in fns.values() if o > st), default=len(words) * 4)
                resolved, masks, *_ = score.relocate(obj, words, st, en, syms)
                ow = resolved[st // 4:en // 4]
                va = syms[n]
                mk = {va + (k - st): m for k, m in masks.items() if st <= k < en}
                ml = ovtool.listing(ow, va, syms, rev, mk)
                a, b = ovtool.pair(ovtool.listing(want, va, syms, rev), ml, {(k - st) // 4 for k in masks if st <= k < en})
                sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
                m = sum(x.size for x in sm.get_matching_blocks())
                res.append((len(a) + len(b) - 2 * m, r.differing, len(a), len(b)))
            except SystemExit as e:
                res.append((-1, -1, 0, 0))
        rows.append((v.name, res, ''))
def key(r):
    return (sum(x[0] for x in r[1]) if r[1] else 10**9)

for n, res, err in sorted(rows, key=key):
    print(n, ' '.join(('MATCH' if x[1] == 0 else f'd{x[0]}/{x[1]}w {x[2]}->{x[3]}') for x in res) if res else 'FAIL ' + err)
