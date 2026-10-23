#!/usr/bin/env python3
"""On builder: obatch.py VARDIR TARGETS FUNC[,FUNC..] [--group GROUPJSON] [--flags F] [--jobs N]
Compile every v*.c in VARDIR (standalone with flags, or as group.c with GROUPJSON) and print strict differing
words for each FUNC, best first. TARGETS = asm/us/ovl_a | asm/us/ovl_b."""
import sys, os, io, shutil, tempfile, contextlib, argparse
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor
sys.path.insert(0, 'tools/cloud')
import score
ap = argparse.ArgumentParser(); ap.add_argument('vd'); ap.add_argument('tg'); ap.add_argument('funcs')
ap.add_argument('--group'); ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared')
ap.add_argument('--jobs', type=int, default=2); ap.add_argument('--top', type=int, default=30); ap.add_argument('--show', type=int, default=0)
a = ap.parse_args()
score.ASM_DIR = Path(a.tg).resolve()
funcs = a.funcs.split(',')
def one(f):
    d = Path(tempfile.mkdtemp(prefix='ob-'))
    try:
        obj = d / 'o.o'
        with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
            if a.group:
                shutil.copy(f, d / 'group.c'); shutil.copy(a.group, d / 'group.json')
                score.compile_group(d, obj)
            else:
                score.compile_single(str(f), a.flags, obj)
            row = []; det = ''
            for k, fn in enumerate(funcs):
                buf = io.StringIO()
                with contextlib.redirect_stdout(buf):
                    row.append(score.compare(obj, fn, show=a.show).differing)
                if k == 0 and a.show:
                    import re
                    det = ' | '.join(re.sub(r'\s+', ' ', l.strip()) for l in buf.getvalue().splitlines() if l.strip().startswith('+0x'))
        return (row, f.name + ((' ' + det) if det else ''))
    except BaseException as e:
        return ([10**6] * len(funcs), f.name + ' FAIL')
    finally:
        shutil.rmtree(d, ignore_errors=True)
if __name__ == '__main__':
  import multiprocessing as mp
  fs = sorted(Path(a.vd).glob('v*.c'))
  with ProcessPoolExecutor(a.jobs, mp_context=mp.get_context('fork')) as ex:
    res = list(ex.map(one, fs))
  res.sort(key=lambda x: (sum(x[0]), x[1]))
  out = '\n'.join(f'{n} {r}' for r, n in res)
  open(Path(a.vd) / 'score.txt', 'w').write(out + '\n')
  print('\n'.join(out.split('\n')[:a.top]))
