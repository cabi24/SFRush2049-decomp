#!/usr/bin/env python3
"""zbuild.py GROUP_DIR [--as1=-r4300_mul] [--umerge/--uopt/--ugen FLAGS] [--only a,b]

Build an IPA group like `score.py group`, but with extra flags per IDO pass, and
print each member's emitted size and strict comparison (score.compare). Used for
the `-r4300_mul` results in cloud/work/R4300_MUL.md. It calls score.py internals
(_run, symbols, text_words, compare, targets), so it may need updating when
score.py changes. Cloud Lane A helper, not part of the CI path.
"""
import sys, tempfile, io, contextlib, json, shlex, re, argparse
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
ap = argparse.ArgumentParser(); ap.add_argument('g'); ap.add_argument('--umerge', default=''); ap.add_argument('--only', default=''); ap.add_argument('--as1', default=''); ap.add_argument('--ugen', default=''); ap.add_argument('--uopt', default=''); ap.add_argument('--idodir', default='')
a = ap.parse_args(); g = Path(a.g); spec = json.loads((g/'group.json').read_text())
ido = (lambda t: str(Path(a.idodir)/t)) if a.idodir else score.ido
def build(out):
    with tempfile.TemporaryDirectory() as tmp:
        work = Path(tmp)
        for name in spec['files']: (work/name).write_text((g/name).read_text())
        (work/'keep.txt').write_text(''.join(k+'\n' for k in spec['keep']))
        units = [re.sub(r'\.c$', '.u', f) for f in spec['files']]
        common = ['-mips2', '-EB', '-g0', '-O3']
        steps = [[ido('cc'), '-j', *shlex.split(spec['flags']), *spec['files']],
                 [ido('uld'), '-L/usr/lib/mips2/nonshared', '-_SYSTYPE_SVR4', '-mips2', '-non_shared', '-g0', '-no_AutoGnum', '-kp', 'keep.txt', *units, '-ko', 'linked'],
                 [ido('usplit'), '-mips2', '-o', 'split', '-t', 'st', 'linked'],
                 [ido('umerge'), *shlex.split(a.umerge), '-Olimit', '5000', *common, 'split', '-o', 'merged', '-t', 'st'],
                 [ido('uopt'), '-G', '0', '-Olimit', '5000', *common, *shlex.split(a.uopt), 'merged', 'opt', '-t', 'st', 'optlog'],
                 [ido('ugen'), '-G', '0', *common, *shlex.split(a.ugen), 'opt', '-o', 'gen', '-t', 'st', '-temp', 'ugtmp'],
                 [ido('as1'), '-elf', '-G', '0', '-p0', *common, *shlex.split(a.as1), '-Olimit', '5000', 'gen', '-o', str(out), '-t', 'st']]
        for s in steps:
            p = score._run(s, cwd=work)
            if p.returncode: sys.exit(Path(s[0]).name + ' failed: ' + (p.stderr or p.stdout)[:2000])
with tempfile.TemporaryDirectory() as t:
    obj = Path(t)/'o.o'; build(obj)
    fns = score.symbols(obj); words = score.text_words(obj)
    order = sorted(fns.items(), key=lambda x: x[1])
    print('emitted:', ' '.join(f"{n}({((order[i+1][1] if i+1 < len(order) else len(words)*4) - o)//4})" for i, (n, o) in enumerate(order)))
    tot = ok = 0
    for n in spec['members']:
        if a.only and n not in a.only.split(','): continue
        want = len(score.targets()[n]); tot += want
        if n not in fns:
            print(f'  {n:28s} MISSING  target {want}'); continue
        st = fns[n]; end = min([o for o in fns.values() if o > st] or [len(words)*4])
        with contextlib.redirect_stdout(io.StringIO()):
            c = score.compare(obj, n)
        if c.differing == 0 and not c.extra_words: ok += want
        print(f'  {n:28s} size {(end-st)//4:4d}/{want:<4d} {c.summary()[:110]}')
    print(f'matched words {ok}/{tot}')
