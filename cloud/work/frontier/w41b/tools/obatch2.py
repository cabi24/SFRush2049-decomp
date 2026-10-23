#!/usr/bin/env python3
"""On builder: obatch2.py VARDIR TARGETS FUNC[,CHECK..] --group GROUPJSON [--jobs N] [--top N]
Like w36x obatch.py, but for FUNC prints: strict differing words, non-sp words (differences that are not
only a stack-offset change), frame got, and the sp-offset remap (want->got:count); CHECK funcs: strict only."""
import sys, io, shutil, tempfile, contextlib, argparse, collections
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor
sys.path.insert(0, 'tools/cloud')
import score
ap = argparse.ArgumentParser(); ap.add_argument('vd'); ap.add_argument('tg'); ap.add_argument('funcs')
ap.add_argument('--group'); ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared')
ap.add_argument('--jobs', type=int, default=2); ap.add_argument('--top', type=int, default=40)
ap.add_argument('--glob', default='v*.c')
ap.add_argument('--gdir'); ap.add_argument('--file', default='group.c')
a = ap.parse_args()
score.ASM_DIR = Path(a.tg).resolve()
funcs = a.funcs.split(',')
def detail(obj, name):
    want = score.targets()[name]
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[name]; end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    res, masks, unres, unv, errs = score.relocate(obj, words, start, min(start + len(want) * 4, end), score.image_symbols())
    got = res[start // 4:end // 4]
    nonsp = 0; remap = collections.Counter()
    for i, w in enumerate(want):
        g = got[i] if i < len(got) else None
        m = masks.get(start + 4 * i, 0xFFFFFFFF)
        if g is None: nonsp += 1; continue
        if (g & m) == (w & m): continue
        if ((w ^ g) & 0xFFFF0000) == 0 and ((w >> 21) & 31) == 29 and (w >> 26) != 9:
            remap[(w & 0xFFFF, g & 0xFFFF)] += 1
        else:
            nonsp += 1
    fr = (0x10000 - (got[0] & 0xFFFF)) if got and (got[0] >> 16) == 0x27bd else -1
    rm = ' '.join(f'{x}>{y}:{c}' for (x, y), c in sorted(remap.items()))
    return nonsp, fr, rm, len(got) - len(want)
def one(f):
    d = Path(tempfile.mkdtemp(prefix='ob2-'))
    try:
        obj = d / 'o.o'
        with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
            if a.gdir:
                for g in Path(a.gdir).iterdir():
                    if g.is_file(): shutil.copy(g, d / g.name)
                shutil.copy(f, d / a.file)
            else:
                shutil.copy(f, d / 'group.c'); shutil.copy(a.group, d / 'group.json')
            score.compile_group(d, obj)
            row = [score.compare(obj, fn, show=0).differing for fn in funcs]
            nonsp, fr, rm, dl = detail(obj, funcs[0])
        return (row[0], f'{f.name} strict={row[0]} nonsp={nonsp} frame={fr} dlen={dl} chk={row[1:]} [{rm}]')
    except BaseException as e:
        return (10**6, f'{f.name} FAIL {type(e).__name__}')
    finally:
        shutil.rmtree(d, ignore_errors=True)
if __name__ == '__main__':
    import multiprocessing as mp
    fs = sorted(Path(a.vd).glob(a.glob))
    with ProcessPoolExecutor(a.jobs, mp_context=mp.get_context('fork')) as ex:
        res = list(ex.map(one, fs))
    res.sort()
    out = '\n'.join(n for _, n in res)
    open(Path(a.vd) / 'score2.txt', 'w').write(out + '\n')
    print('\n'.join(out.split('\n')[:a.top]))
