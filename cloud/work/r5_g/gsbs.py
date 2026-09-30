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
ap = argparse.ArgumentParser(); ap.add_argument('g'); ap.add_argument('fn'); ap.add_argument('--lo',type=int,default=0); ap.add_argument('--hi',type=int,default=400); ap.add_argument('--umerge', default=''); ap.add_argument('--only', default=''); ap.add_argument('--as1', default=''); ap.add_argument('--ugen', default=''); ap.add_argument('--uopt', default=''); ap.add_argument('--idodir', default='')
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

import difflib
with tempfile.TemporaryDirectory() as t:
    obj = Path(t)/'o.o'; build(obj)
    words = score.text_words(obj); fns = score.symbols(obj); st = fns[a.fn]
    end = min((o for o in fns.values() if o > st), default=len(words)*4)
    res, masks, *_ = score.relocate(obj, words, st, end, score.image_symbols())
    got = res[st//4:end//4]; want = score.targets()[a.fn]
sm = difflib.SequenceMatcher(None, want, got, autojunk=False)
rows=[]; bad=0
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    for k in range(max(i2-i1,j2-j1)):
        x = want[i1+k] if i1+k<i2 else None; y = got[j1+k] if j1+k<j2 else None
        rows.append((tag[0], i1+k if x is not None else None, score.disasm_word(x) if x is not None else '', j1+k if y is not None else None, score.disasm_word(y) if y is not None else ''))
        if tag!='equal': bad+=1
print('aligned differing rows:', bad, 'size', len(got), '/', len(want))
for r in rows[a.lo:a.hi]:
    print(f"{r[0]} {str(r[1]):>4} {r[2]:30s} | {str(r[3]):>4} {r[4]}")
