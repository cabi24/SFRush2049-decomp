#!/usr/bin/env python3
"""vrun.py SPEC.py [FUNCS...]: SPEC defines VARIANTS = {name: [(part_file, old, new), ...]} applied to groups/trackscr parts.
Builds v/<name>/ group dirs, scores all on the builder in one ssh, prints per-function diff counts."""
import sys, os, shutil, subprocess, re, runpy
L = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
base = os.path.join(L, 'groups', os.environ.get('VGROUP', 'trackscr'))
spec = runpy.run_path(sys.argv[1])
funcs = sys.argv[2:] or None
V = spec['VARIANTS']
vroot = os.path.join(L, 'v')
for n in os.listdir(vroot):
    shutil.rmtree(os.path.join(vroot, n))
for name, edits in V.items():
    d = os.path.join(vroot, name)
    shutil.copytree(base, d)
    for f, old, new in edits:
        p = os.path.join(d, f)
        s = open(p).read()
        if old not in s:
            print(f'!! {name}: edit not found in {f}: {old[:60]!r}'); continue
        s = s.replace(old, new, 1)
        open(p, 'w').write(s)
    subprocess.run(['bash', os.path.join(d, 'build.sh')], check=True)
subprocess.run(['rsync', '-aq', '--delete', vroot + '/', 'watchman2:rush2049/scratch/frontier/w36d/v/'], check=True)
if funcs:
    subprocess.run(['rsync', '-aq', os.path.join(L, 'tools', 'vscore.py'), 'watchman2:rush2049/scratch/frontier/w36d/vscore.py'], check=True)
    cmd = ('cd ~/rush2049/scratch/frontier/w36d && export IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; '
           'python3 vscore.py ' + ','.join(funcs) + ' ' + ' '.join('v/%s/' % n for n in V))
    print(subprocess.run(['ssh', 'watchman2', cmd], capture_output=True, text=True).stdout)
    sys.exit(0)
cmd = ('cd ~/rush2049/scratch/frontier/w36d && export IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; '
       'for d in v/*/; do echo "=== $d"; python3 tools/cloud/score.py group $d --targets asm/us/ovl_a 2>&1 | grep -E "^func|MATCH|words differ|rror"; done')
out = subprocess.run(['ssh', 'watchman2', cmd], capture_output=True, text=True).stdout
cur = None; res = {}
fn = None
for line in out.splitlines():
    m = re.match(r'=== v/(\S+)/', line)
    if m: cur = m.group(1); res[cur] = {}; continue
    m = re.match(r'^(func_\w+):', line)
    if m: fn = m.group(1); continue
    m = re.match(r'\s+(\d+)/(\d+) words differ', line)
    if m and cur: res[cur][fn] = m.group(1); continue
    if re.match(r'\s+MATCH', line) and cur: res[cur][fn] = 'M'; continue
    if 'rror' in line and cur: res[cur]['ERR'] = line.strip()[:120]
for name in V:
    r = res.get(name, {})
    items = [(k, v) for k, v in r.items() if funcs is None or k in funcs or k == 'ERR']
    print(name.ljust(28), ' '.join(f'{k[-4:]}:{v}' for k, v in items))
