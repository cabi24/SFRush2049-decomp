#!/usr/bin/env python3
"""On builder: vscore.py FUNCS VDIR... -> per variant per function: strict positional diff, aligned raw, aligned norm."""
import sys, json, struct, subprocess, tempfile, re, difflib
sys.path.insert(0, 'tools/cloud')
import score
from pathlib import Path
score.ASM_DIR = Path('asm/us/ovl_a').resolve()
funcs = sys.argv[1].split(',')
targets = score.targets()
imgsyms = score.image_symbols()
def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
    return [re.sub(r'\s+',' ',m.group(3)) for m in (re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', l) for l in out.splitlines()) if m]
def norm(t):
    t = re.sub(r'\b(t\d|v[01]|a[0-3]|s\d|fp|at|zero|ra)\b', 'R', t)
    t = re.sub(r'\$f\d+', 'F', t)
    return re.sub(r'0x[0-9a-f]+$', 'L', t)
for vd in sys.argv[2:]:
    obj = Path(tempfile.mkdtemp()) / 'o.o'
    try:
        score.compile_group(Path(vd), obj)
    except Exception as e:
        print(vd, 'COMPILE-ERROR', str(e)[:200]); continue
    words = score.text_words(obj); fn = score.symbols(obj)
    res = []
    for name in funcs:
        if name not in fn: res.append(f'{name[-4:]}:missing'); continue
        start = fn[name]
        end = min((o for o in fn.values() if o > start), default=len(words)*4)
        want = list(targets[name])
        resolved, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, imgsyms)
        got = list(resolved[start//4:end//4])
        for o, m in masks.items():
            if start <= o < end:
                k = (o - start)//4
                if k < len(want): want[k] &= m
                if k < len(got): got[k] &= m
        pos = sum(1 for i in range(max(len(want),len(got))) if i >= len(want) or i >= len(got) or want[i] != got[i])
        w = dis(want); g = dis(got)
        def al(a, b):
            sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
            return sum(max(i2-i1, j2-j1) for t,i1,i2,j1,j2 in sm.get_opcodes() if t != 'equal')
        res.append(f'{name[-4:]}:{pos}/{al(w,g)}/{al([norm(x) for x in w],[norm(x) for x in g])}')
    print(vd.rstrip('/').split('/')[-1].ljust(28), ' '.join(res))
