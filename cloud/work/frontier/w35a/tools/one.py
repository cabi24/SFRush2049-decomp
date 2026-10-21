#!/usr/bin/env python3
"""one.py SEGFILE NAME... [--tag T]: build group (head + SEGFILE + tail), score + dump diffs on the builder, pull to out/T.out"""
import json, subprocess, sys, shutil
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
W = ROOT / 'cloud/work/frontier/w35a'
args = sys.argv[1:]
tag = 'one'
if '--tag' in args:
    i = args.index('--tag'); tag = args[i + 1]; del args[i:i + 2]
seg = Path(args[0]); names = args[1:]
head = (W / 'parts/head.c').read_text(); tail = (W / 'parts/tail.c').read_text()
d = W / 'tmp' / tag; d.mkdir(parents=True, exist_ok=True)
(d / 'group.c').write_text(head + seg.read_text().rstrip('\n') + '\n\n' + tail)
shutil.copy(W / 'parts/group.json', d / 'group.json')
R = 'watchman2:rush2049/scratch/frontier/w35a'
subprocess.run(['ssh', 'watchman2', f'mkdir -p ~/rush2049/scratch/frontier/w35a/cand/{tag}'], check=True)
subprocess.run(['scp', '-q', str(d / 'group.c'), str(d / 'group.json'), f'{R}/cand/{tag}/'], check=True)
r = subprocess.run([str(W / 'tools/b.sh'), 'group', f'cand/{tag}', *names], capture_output=True, text=True)
for l in r.stdout.splitlines():
    print(l[:170])
print(r.stderr[:2000], end='')
o = W / 'out' / f'{tag}.out'
shutil.rmtree(o, ignore_errors=True)
subprocess.run(['scp', '-q', '-r', f'{R}/cand/{tag}.out', str(W / 'out') + '/'], check=True)
