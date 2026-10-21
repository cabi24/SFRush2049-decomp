#!/usr/bin/env python3
import sys, json, struct, subprocess, tempfile, re, difflib
d = json.load(open(sys.argv[1])); name = sys.argv[2]
def dis(words, base=0):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
    res = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m: res.append((int(m.group(1),16), m.group(2), re.sub(r'\s+',' ',m.group(3))))
    return res
mk = {int(k): v for k, v in d.get('masks', {}).items()}
for k, v in mk.items():
    if k < len(d['want']): d['want'][k] &= v
    if k < len(d['got']): d['got'][k] &= v
w = dis(d['want']); g = dis(d['got'])
NORM = len(sys.argv) > 3 and '--norm' in sys.argv[3]
import re as _re
def norm(x):
    t = x[2]
    if NORM:
        t = _re.sub(r'\b(t\d|v[01]|a[0-3]|s\d|fp|at|zero|ra)\b', 'R', t)
        t = _re.sub(r'0x[0-9a-f]+$', 'L', t)
    return t
sm = difflib.SequenceMatcher(None, [norm(x) for x in w], [norm(x) for x in g], autojunk=False)
ctx = 3
bad = 0
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal': continue
    bad += max(i2-i1, j2-j1)
    print(f'--- {tag}: want[{i1}:{i2}] got[{j1}:{j2}]')
    for k in range(max(i1-ctx,0), i1): print(f'   {w[k][0]:04x}  {w[k][2]}')
    for k in range(i1, i2): print(f' - {w[k][0]:04x}  {w[k][2]}')
    for k in range(j1, j2): print(f' + {g[k][0]:04x}  {g[k][2]}')
    for k in range(i2, min(i2+ctx,len(w))): print(f'   {w[k][0]:04x}  {w[k][2]}')
print(f'{len(w)} want words, {len(g)} got words')
