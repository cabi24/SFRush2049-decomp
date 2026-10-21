#!/usr/bin/env python3
"""stage.py GROUPDIR LABEL  (on the builder, in the scratch dir): run the group build up to umerge into st_LABEL/{merged,st}
(same steps as score.compile_group) so the trace toolkit (bin/uopt etc., tk/tk.sh) can work on it."""
import json, os, re, shlex, shutil, subprocess, sys
from pathlib import Path
gd = Path(sys.argv[1]).resolve(); label = sys.argv[2]
IDO = Path(os.environ['IDO_DIR'])
spec = json.loads((gd / 'group.json').read_text())
D = Path(os.getcwd()) / f'st_{label}'
if D.exists(): shutil.rmtree(D)
D.mkdir()
for n in spec['files']: shutil.copy(gd / n, D / n)
(D / 'keep.txt').write_text(''.join(k + '\n' for k in spec['keep']))
flags = shlex.split(spec['flags'])
units = [re.sub(r'\.c$', '.u', f) for f in spec['files']]
common = ['-mips2', '-EB', '-g0', '-O3']
steps = [
    [str(IDO/'cc'), '-j', *flags, *spec['files']],
    [str(IDO/'uld'), '-L/usr/lib/mips2/nonshared', '-_SYSTYPE_SVR4', '-mips2', '-non_shared', '-g0', '-no_AutoGnum', '-kp', 'keep.txt', *units, '-ko', 'linked'],
    [str(IDO/'usplit'), '-mips2', '-o', 'split', '-t', 'st', 'linked'],
    [str(IDO/'umerge'), '-Olimit', '5000', *common, 'split', '-o', 'merged', '-t', 'st'],
]
for s in steps:
    p = subprocess.run(s, cwd=D, capture_output=True, text=True)
    if p.returncode != 0:
        print('FAILED', s[0], (p.stderr or p.stdout)[:2000]); sys.exit(1)
print('staged', D)
