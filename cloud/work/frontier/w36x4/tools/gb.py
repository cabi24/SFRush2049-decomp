#!/usr/bin/env python3
"""gb.py (runs on the builder in the scratch dir): batch-score group variants.
  gb.py GROUPDIR VARDIR FILE NAMES [--jobs N]
For every VARDIR/v*.c: copy GROUPDIR, replace FILE with the variant, compile the group, score NAMES
(comma list). Prints one line per variant: per-name strict words / norm rows / frame, sorted by total."""
import os, sys, shutil, tempfile, difflib, re, json
from pathlib import Path
from concurrent.futures import ProcessPoolExecutor
import multiprocessing
sys.path.insert(0, os.getcwd() + '/tools/cloud')
import score
sys.argv_saved = sys.argv
gs = {}
src = open(os.getcwd() + '/tools/gscore.py').read().replace('\nmain()', '\n')
exec(compile(src, 'gscore', 'exec'), gs)

def one(args):
    gdir, var, fname, names = args
    with tempfile.TemporaryDirectory() as tmp:
        d = Path(tmp) / 'g'
        shutil.copytree(gdir, d)
        shutil.copy(var, d / fname)
        obj = Path(tmp) / 'o.o'
        try:
            score.compile_group(d, obj)
        except SystemExit as e:
            return (10**9, os.path.basename(var), 'COMPILE FAIL')
        out = []; tot = 0
        for n in names:
            try:
                r = score.compare(obj, n, show=0)
                w, g = gs['fixed_got'](obj, n)
                wl, gl = gs['dis'](w), gs['dis'](g)
                def rows(a, b):
                    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
                    return sum(max(i2-i1, j2-j1) for t, i1, i2, j1, j2 in sm.get_opcodes() if t != 'equal')
                rn = rows([gs['norm'](x) for x in wl], [gs['norm'](x) for x in gl])
                s = r.summary()
                m = re.match(r'(\d+)/(\d+)', s)
                k = 0 if s.startswith('MATCH') else (int(m.group(1)) if m else 999)
                tot += k
                out.append(f'{n}={k}/n{rn}/f{gs["frame"](gl)}')
            except SystemExit as e:
                tot += 999; out.append(f'{n}=ERR')
        return (tot, os.path.basename(var), ' '.join(out))

def main():
    gdir, vdir, fname, names = sys.argv[1:5]
    jobs = int(sys.argv[sys.argv.index('--jobs')+1]) if '--jobs' in sys.argv else 2
    score.ASM_DIR = Path('asm/us/blob').resolve()
    vs = sorted(str(p) for p in Path(vdir).glob('v*.c'))
    with ProcessPoolExecutor(jobs, mp_context=multiprocessing.get_context('fork')) as ex:
        res = list(ex.map(one, [(gdir, v, fname, names.split(',')) for v in vs]))
    res.sort()
    with open(Path(vdir) / 'score.txt', 'w') as f:
        for r in res: f.write(f'{r[0]:5d} {r[1]} {r[2]}\n')
    for r in res[:int(os.environ.get('TOP', '25'))]: print(f'{r[0]:5d} {r[1]} {r[2]}')
if __name__ == '__main__':
    main()
