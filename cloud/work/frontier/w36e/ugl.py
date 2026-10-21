#!/usr/bin/env python3
"""On builder (cwd = scratch): ugl.py GROUPDIR OUT.lst : run the group pipeline with ugen -l listing."""
import sys, json, shlex, re, subprocess, tempfile, os
from pathlib import Path
IDO = Path(os.environ['IDO_DIR'])
g = Path(sys.argv[1]); out = Path(sys.argv[2]).resolve()
spec = json.loads((g / 'group.json').read_text())
work = Path(tempfile.mkdtemp(prefix='ugl-', dir=str(Path.home() / 'rush2049/scratch/frontier/w36e')))
for n in spec['files']: (work / n).write_text((g / n).read_text())
(work / 'keep.txt').write_text(''.join(k + '\n' for k in spec['keep']))
flags = shlex.split(spec['flags']); units = [re.sub(r'\.c$', '.u', f) for f in spec['files']]
common = ['-mips2', '-EB', '-g0', '-O3']
steps = [[str(IDO/'cc'), '-j', *flags, *spec['files']],
 [str(IDO/'uld'), '-L/usr/lib/mips2/nonshared', '-_SYSTYPE_SVR4', '-mips2', '-non_shared', '-g0', '-no_AutoGnum', '-kp', 'keep.txt', *units, '-ko', 'linked'],
 [str(IDO/'usplit'), '-mips2', '-o', 'split', '-t', 'st', 'linked'],
 [str(IDO/'umerge'), '-Olimit', '5000', *common, 'split', '-o', 'merged', '-t', 'st'],
 [str(IDO/'uopt'), '-G', '0', '-Olimit', '5000', *common, 'merged', 'opt', '-t', 'st', 'optlog'],
 [str(IDO/'ugen'), '-G', '0', *common, 'opt', '-o', 'gen', '-t', 'st', '-temp', 'ugtmp', '-l', str(out)]]
for st in steps:
    p = subprocess.run(st, cwd=work, capture_output=True, text=True)
    if p.returncode: print(st[0], p.stderr[:2000]); sys.exit(1)
print('ok', out)
