#!/usr/bin/env python3
"""objdiff_ovl.py OBJ FN [--norm]: aligned diff of FN in OBJ vs image A retail (Pi-side, no compile)."""
import sys, os, re, json, struct, subprocess, tempfile, difflib
from pathlib import Path
REPO = Path('/home/cburnes/projects/rush2049-decomp')
sys.path.insert(0, str(REPO / 'tools' / 'cloud'))
obj = Path(os.path.abspath(sys.argv[1])); fn = sys.argv[2]; NORM = '--norm' in sys.argv
os.chdir(REPO)
import score
score.ASM_DIR = (REPO / 'asm/us/ovl_a').resolve()
want = list(score.targets()[fn])
words = score.text_words(obj); fns = score.symbols(obj)
start = fns[fn]; end = min((o for o in fns.values() if o > start), default=len(words)*4)
res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, score.image_symbols())
got = list(res[start//4:end//4])
for o, m in masks.items():
    if start <= o < end:
        k = (o-start)//4
        if k < len(want): want[k] &= m
        if k < len(got): got[k] &= m
def dis(ws):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(ws)}I', *ws)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
    return [re.sub(r'\s+',' ',m.group(3)) for m in (re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', l) for l in out.splitlines()) if m]
w = dis(want); g = dis(got)
def n(t):
    if not NORM: return t
    t = re.sub(r'\b(t\d|v[01]|a[0-3]|s\d|fp|at|zero|ra)\b', 'R', t); t = re.sub(r'\$f\d+', 'F', t)
    return re.sub(r'0x[0-9a-f]+$', 'L', t)
sm = difflib.SequenceMatcher(None, [n(x) for x in w], [n(x) for x in g], autojunk=False)
bad = 0
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal': continue
    bad += max(i2-i1, j2-j1)
    print(f'--- {tag} want[{i1}:{i2}] got[{j1}:{j2}]')
    for k in range(i1, i2): print(f' - {k*4:04x} {w[k]}')
    for k in range(j1, j2): print(f' + {k*4:04x} {g[k]}')
pos = sum(1 for i in range(max(len(w),len(g))) if i >= len(want) or i >= len(got) or want[i] != got[i])
print(f'{len(w)} want, {len(g)} got; aligned-diff {bad}; positional {pos}')
