#!/usr/bin/env python3
"""obscore.py DIR FN[,FN2..] [--group GDIR --file shots.c] [--targets asm/us/ovl_b] [--jobs 2] [--top 20]
Runs on the builder from the lane scratch. Scores every DIR/*.c (standalone, or as GDIR with FILE replaced)
for FN (first name ranks; others reported). Prints strict words, aligned rows (difflib on text), norm rows."""
import sys, argparse, tempfile, os, shutil, difflib, json
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, 'tools/cloud'); sys.path.insert(0, 'tools')
import score
import gscore

def one(src, fns, a):
    try:
        with tempfile.TemporaryDirectory() as t:
            obj = Path(t) / 'o.o'
            try:
                if a.group:
                    g = Path(t) / 'g'; shutil.copytree(a.group, g)
                    shutil.copy(src, g / a.file)
                    score.compile_group(g, obj)
                else:
                    score.compile_single(str(src), a.flags, obj)
            except SystemExit:
                return (9999, src.name + ' COMPILE FAIL')
            out = []
            first = None
            for fn in fns:
                r = score.compare(obj, fn, show=0)
                w, g = gscore.fixed_got(obj, fn)
                wl, gl = gscore.dis(w), gscore.dis(g)
                def rows(x, y):
                    sm = difflib.SequenceMatcher(None, x, y, autojunk=False)
                    return sum(max(i2-i1, j2-j1) for tg, i1, i2, j1, j2 in sm.get_opcodes() if tg != 'equal')
                rw = rows(wl, gl); rn = rows([gscore.norm(x) for x in wl], [gscore.norm(x) for x in gl])
                st = 0 if r.summary().startswith('MATCH') else int(r.summary().split('/')[0])
                if first is None: first = (st, rw, rn)
                out.append(f'{fn[-4:]}:{st}/{rw}/{rn}')
            return (first, src.name + '  ' + ' '.join(out))
    except Exception as e:
        return ((9998, 0, 0), src.name + ' ERR ' + repr(e)[:100])

def main():
    ap = argparse.ArgumentParser(); ap.add_argument('dir'); ap.add_argument('fn')
    ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared')
    ap.add_argument('--targets', default='asm/us/ovl_b')
    ap.add_argument('--group', default=''); ap.add_argument('--file', default='shots.c')
    ap.add_argument('--jobs', type=int, default=2); ap.add_argument('--top', type=int, default=20)
    ap.add_argument('--sort', default='rows')
    a = ap.parse_args()
    score.ASM_DIR = Path(a.targets).resolve()
    fns = a.fn.split(',')
    files = sorted(Path(a.dir).glob('*.c'))
    with ThreadPoolExecutor(a.jobs) as ex:
        res = list(ex.map(lambda f: one(f, fns, a), files))
    res = [r if isinstance(r[0], tuple) else ((r[0], 0, 0), r[1]) for r in res]
    res.sort(key=lambda r: (r[0][1], r[0][0]) if a.sort == 'rows' else r[0])
    for r in res[:a.top]:
        print('strict %4d rows %4d norm %4d  %s' % (r[0][0], r[0][1], r[0][2], r[1]))
    print('%d files' % len(res))
main()
