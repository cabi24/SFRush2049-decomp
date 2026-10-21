#!/usr/bin/env python3
"""vrun.py SPEC.py FUNCS(comma): SPEC defines BASE (lane-relative group dir) and VARIANTS = {name: [(old, new), ...]}
applied to BASE/group.c. Builds v/<spec>/<name>/ group dirs, scores all on the builder in one ssh."""
import sys, os, shutil, subprocess, runpy
L = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
spec = runpy.run_path(sys.argv[1]); funcs = sys.argv[2]
tag = os.path.splitext(os.path.basename(sys.argv[1]))[0]
base = os.path.join(L, spec['BASE'])
vroot = os.path.join(L, 'v', tag)
shutil.rmtree(vroot, ignore_errors=True); os.makedirs(vroot)
names = []
for name, edits in spec['VARIANTS'].items():
    d = os.path.join(vroot, name); shutil.copytree(base, d)
    p = os.path.join(d, 'group.c' if os.path.exists(os.path.join(d, 'group.c')) else 'cand.c'); s = open(p).read(); ok = True
    for old, new in edits:
        if s.count(old) != 1:
            print(f'!! {name}: edit matches {s.count(old)}x: {old[:70]!r}'); ok = False; break
        s = s.replace(old, new, 1)
    if not ok: shutil.rmtree(d); continue
    open(p, 'w').write(s); names.append(name)
B = 'watchman2:rush2049/scratch/frontier/w36x2'
subprocess.run(['rsync', '-aq', '--delete', vroot + '/', f'{B}/v/{tag}/'], check=True)
subprocess.run(['rsync', '-aq', os.path.join(L, 'tools', 'vscore.py'), f'{B}/vscore.py'], check=True)
cmd = ('cd ~/rush2049/scratch/frontier/w36x2 && export IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; '
       f'TGT={os.environ.get("TGT","asm/us/ovl_a")} python3 vscore.py {funcs} ' + ' '.join(f'v/{tag}/{n}' for n in names))
subprocess.run(['ssh', 'watchman2', cmd])
