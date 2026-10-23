#!/usr/bin/env python3
"""Builder side (w40a): stage a group compile into WORKDIR and keep every intermediate (merged, st, opt, gen, .o).
Usage (scratch root): gstage.py GROUP_DIR WORKDIR [extra uopt args...]   -> WORKDIR/{merged,st,opt,gen,out.o,uopt.log}"""
import sys, os, json, re, shlex, shutil, subprocess
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
g = Path(sys.argv[1]); work = Path(sys.argv[2]); extra = sys.argv[3:]
shutil.rmtree(work, ignore_errors=True); work.mkdir(parents=True)
spec = json.loads((g / 'group.json').read_text())
for n in spec['files']: (work / n).write_text((g / n).read_text())
(work / 'keep.txt').write_text(''.join(k + '\n' for k in spec['keep']))
flags = shlex.split(spec['flags']); units = [re.sub(r'\.c$', '.u', f) for f in spec['files']]
common = ['-mips2', '-EB', '-g0', '-O3']; I = score.ido
steps = [[I('cc'), '-j', *flags, *spec['files']],
 [I('uld'), '-L/usr/lib/mips2/nonshared', '-_SYSTYPE_SVR4', '-mips2', '-non_shared', '-g0', '-no_AutoGnum', '-kp', 'keep.txt', *units, '-ko', 'linked'],
 [I('usplit'), '-mips2', '-o', 'split', '-t', 'st', 'linked'],
 [I('umerge'), *os.environ.get('UMX', '').split(), '-Olimit', '5000', *common, 'split', '-o', 'merged', '-t', 'st'],
 [I('uopt'), '-G', '0', '-Olimit', '5000', *common, *extra, 'merged', 'opt', '-t', 'st', 'optlog'],
 [I('ugen'), '-G', '0', *common, 'opt', '-o', 'gen', '-t', 'st', '-temp', 'ugtmp'],
 [I('as1'), '-elf', '-G', '0', '-p0', *common, score.R4300_AS1, '-Olimit', '5000', 'gen', '-o', 'out.o', '-t', 'st']]
for s in steps:
    p = subprocess.run(s, cwd=work, capture_output=True, text=True)
    if Path(s[0]).name in ('uopt', 'umerge'): (work / (Path(s[0]).name + '.log')).write_text(p.stdout + p.stderr)
    if p.returncode: print(Path(s[0]).name, 'failed', p.stderr[:2000]); sys.exit(1)
print('staged', work)
