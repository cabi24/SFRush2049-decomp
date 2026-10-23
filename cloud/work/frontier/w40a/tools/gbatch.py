#!/usr/bin/env python3
"""Lane tool (w40a), builder side, run from the scratch root:
  gbatch.py HARNESS_GROUP_DIR VARIANT_DIR FILE NAME[,NAME...] [--jobs 2]
For every *.c in VARIANT_DIR: copy the harness group, replace FILE with the variant, compile_group, and
score.compare each NAME (strict differing words, extra words). Prints one line per variant, best first,
and writes VARIANT_DIR/score.txt."""
import sys, shutil, tempfile, io, contextlib, difflib
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor
sys.path.insert(0, 'tools/cloud')
import score

def opkey(w):
    op = w >> 26
    if op == 0: return (0, w & 63)
    if op == 1: return (1, (w >> 16) & 31)
    if op == 17: return (17, (w >> 21) & 31, w & 63)
    return (op,)

def rows(obj, name):
    want = score.targets()[name]
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[name]
    end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, score.image_symbols())
    got = res[start // 4:end // 4]
    while got and got[-1] == 0 and len(got) > len(want): got.pop()
    out = []
    for kw, kg in ((want, got), ([opkey(x) for x in want], [opkey(x) for x in got])):
        n = 0
        for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, kw, kg, autojunk=False).get_opcodes():
            if tag != 'equal': n += max(i2 - i1, j2 - j1)
        out.append(n)
    return out[0], out[1], len(got) - len(want), sig(got)

REG = 'zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp s8 ra'.split()

def sig(words):
    """audio-thread regime signature: F=forwarded song arg, A=reg holding &gSong (addiu x,x,576), B=# of b"""
    jal = 0x0C000000 | ((0x800979A0 >> 2) & 0x3FFFFFF)
    f = '?'
    for i, w in enumerate(words):
        if w == jal and i + 1 < len(words):
            d = words[i + 1]
            f = 'F' if (d >> 26) == 0 and (d & 63) in (0x21, 0x25) else 'L'
    a = sorted({REG[(w >> 16) & 31] for w in words if (w >> 26) == 9 and (w & 0xFFFF) == 576})
    b = sum(1 for w in words if (w >> 16) == 0x1000)
    return f + '/' + ','.join(a) + '/b%d' % b

def one(harness, var, fname, names):
    with tempfile.TemporaryDirectory(prefix='gb-') as t:
        g = Path(t) / 'g'
        shutil.copytree(harness, g)
        shutil.copy(var, g / fname)
        obj = Path(t) / 'o.o'
        try:
            with contextlib.redirect_stdout(io.StringIO()):
                score.compile_group(g, obj)
        except SystemExit as e:
            return var.name, None, str(e).splitlines()[0][:80]
        res = []
        for n in names:
            try:
                with contextlib.redirect_stdout(io.StringIO()):
                    c = score.compare(obj, n, show=0)
                    r, o, dl, sg = rows(obj, n)
                res.append((r, o, dl, c.differing, sg))
            except SystemExit as e:
                res.append((9999, 9999, 0, 9999, ''))
        return var.name, res, ''

def main():
    a = sys.argv[1:]
    jobs = 2
    if '--jobs' in a:
        i = a.index('--jobs'); jobs = int(a[i + 1]); del a[i:i + 2]
    harness, vdir, fname, names = Path(a[0]), Path(a[1]), a[2], a[3].split(',')
    vars_ = sorted(vdir.glob('*.c'))
    with ProcessPoolExecutor(jobs) as ex:
        out = list(ex.map(one, [harness] * len(vars_), vars_, [fname] * len(vars_), [names] * len(vars_)))
    out.sort(key=lambda r: (sum(x[0] for x in r[1]) if r[1] else 10**6, r[0]))
    lines = []
    for name, res, err in out:
        if res is None:
            lines.append(f'{name}\tERROR {err}')
        else:
            lines.append(f'{name}\t' + '\t'.join(f'{n}: rows {r} ops {o} len{dl:+d} strict {d} {sg}' for n, (r, o, dl, d, sg) in zip(names, res)))
    (vdir / 'score.txt').write_text('\n'.join(lines) + '\n')
    print('\n'.join(lines))

if __name__ == '__main__':
    main()
