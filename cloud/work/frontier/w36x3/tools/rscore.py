#!/usr/bin/env python3
"""rscore.py DIR FN[,FN2..] [--flags ...] [--keep a,b] [--jobs 2] [--top N]: runs on the builder from a lane scratch.
Compiles every DIR/*.c (one-file -O3 group when --keep) and prints, best first, the full.py-style aligned
'differing rows' (word SequenceMatcher) for each FN plus size delta. Derived from tools/vbatch/bscore.py."""
import sys, argparse, tempfile, difflib, json
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, 'tools/cloud')
import score

def rows(want, got):
    sm = difflib.SequenceMatcher(None, want, got, autojunk=False)
    return sum(max(i2 - i1, j2 - j1) for t, i1, i2, j1, j2 in sm.get_opcodes() if t != 'equal')

def one(src, fns, flags, wants, syms, keep, ctx=None):
    try:
        with tempfile.TemporaryDirectory() as t:
            obj = Path(t) / 'o.o'
            try:
                if keep:
                    g = Path(t) / 'g'; g.mkdir()
                    (g / 'group.c').write_text(src.read_text())
                    files = ['group.c']
                    if ctx:
                        for c in sorted(Path(ctx).glob('*.c')):
                            (g / c.name).write_text(c.read_text()); files.append(c.name)
                    (g / 'group.json').write_text(json.dumps({'files': files, 'keep': keep, 'flags': flags}))
                    score.compile_group(g, obj)
                else:
                    score.compile_single(str(src), flags, obj)
            except SystemExit:
                return ([9999] * len(fns), [0] * len(fns), src.name + ' COMPILE FAIL')
            words = score.text_words(obj); sy = score.symbols(obj)
            rs = []; ds = []; frames = []
            for fn in fns:
                start = sy[fn]
                end = min((o for o in sy.values() if o > start), default=len(words) * 4)
                res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, syms)
                got = res[start // 4:end // 4]
                want = wants[fn]
                while got and got[-1] == 0 and len(got) > len(want): got.pop()
                rs.append(rows(want, got)); ds.append(len(got) - len(want))
                fr = got[0] & 0xFFFF if got and (got[0] >> 16) == 0x27BD else 0
                frames.append(0x10000 - fr if fr else 0)
        return (rs, ds, src.name + ' frame=' + ','.join(map(str, frames)))
    except Exception as e:
        return ([9998] * len(fns), [0] * len(fns), src.name + ' ERR ' + repr(e)[:80])

ap = argparse.ArgumentParser(); ap.add_argument('dir'); ap.add_argument('fns')
ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared'); ap.add_argument('--top', type=int, default=20)
ap.add_argument('--keep', default=''); ap.add_argument('--jobs', type=int, default=2); ap.add_argument('--ctx', default='')
a = ap.parse_args()
fns = a.fns.split(',')
tg = score.targets(); wants = {f: tg[f] for f in fns}; syms = score.image_symbols()
files = sorted(Path(a.dir).glob('*.c'))
keep = [k for k in a.keep.split(',') if k]
with ThreadPoolExecutor(a.jobs) as ex:
    res = list(ex.map(lambda f: one(f, fns, a.flags, wants, syms, keep, a.ctx or None), files))
res.sort(key=lambda r: (sum(r[0]), r[2]))
for r in res[:a.top]:
    print('  '.join('%s rows %4d size %+d' % (fn[-6:], x, d) for fn, x, d in zip(fns, r[0], r[1])) + '  ' + r[2])
print('%d files' % len(res))
