#!/usr/bin/env python3
"""olim.py GROUPDIR FN : (builder) compile the group with env-overridable per-step Olimit/flags and score FN.
env: UM_OL (umerge Olimit), UO_OL (uopt Olimit), AS_OL (as1 Olimit), AS_EXTRA (extra as1 flags), UG_EXTRA."""
import os, sys, shlex, re, json, tempfile
from pathlib import Path
sys.path.insert(0, 'tools/cloud'); sys.path.insert(0, 'tools')
import score, gscore
score.ASM_DIR = Path('asm/us/ovl_b').resolve()
gd = Path(sys.argv[1]); fns = sys.argv[2].split(',')
spec = json.loads((gd/'group.json').read_text())
ido = score.ido
with tempfile.TemporaryDirectory() as tmp:
    work = Path(tmp); out = work/'out.o'
    for n in spec['files']: (work/n).write_text((gd/n).read_text())
    (work/'keep.txt').write_text(''.join(k+'\n' for k in spec['keep']))
    flags = shlex.split(spec['flags']); units = [re.sub(r'\.c$', '.u', f) for f in spec['files']]
    common = ['-mips2', '-EB', '-g0', '-O3']
    E = os.environ.get
    steps = [
        [ido('cc'), '-j', *flags, *spec['files']],
        [ido('uld'), '-L/usr/lib/mips2/nonshared', '-_SYSTYPE_SVR4', '-mips2', '-non_shared', '-g0', '-no_AutoGnum', '-kp', 'keep.txt', *units, '-ko', 'linked'],
        [ido('usplit'), '-mips2', '-o', 'split', '-t', 'st', 'linked'],
        [ido('umerge'), '-Olimit', E('UM_OL', '5000'), *common, 'split', '-o', 'merged', '-t', 'st'],
        [ido('uopt'), '-G', '0', '-Olimit', E('UO_OL', '5000'), *(common[:3]+[E('UO_O','-O3')]), 'merged', 'opt', '-t', 'st', 'optlog'],
        [ido('ugen'), '-G', '0', *(common[:3]+[E('UG_O','-O3')]), *shlex.split(E('UG_EXTRA', '')), 'opt', '-o', 'gen', '-t', 'st', '-temp', 'ugtmp'],
        [ido('as1'), '-elf', '-G', '0', '-p0', *(common[:3]+[E('AS_O','-O3')]), score.R4300_AS1, '-Olimit', E('AS_OL', '5000'), *shlex.split(E('AS_EXTRA', '')), 'gen', '-o', str(out), '-t', 'st'],
    ]
    for s in steps:
        p = score._run(s, cwd=work)
        if p.returncode != 0: print('FAIL', s[0], (p.stderr or p.stdout)[:500]); sys.exit(1)
    for fn in fns:
        r = score.compare(out, fn, show=0)
        w, g = gscore.fixed_got(out, fn); wl, gl = gscore.dis(w), gscore.dis(g)
        import difflib
        def rows(a, b):
            sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
            return sum(max(i2-i1, j2-j1) for t, i1, i2, j1, j2 in sm.get_opcodes() if t != 'equal')
        print(fn, r.summary()[:40], 'rows', rows(wl, gl), 'norm', rows([gscore.norm(x) for x in wl], [gscore.norm(x) for x in gl]))
