#!/usr/bin/env python3
"""adiff.py SRC.c FN [--flags ..] [--group DIR] [--mode words|ops|norm|nosp] [--all] [--ctx N]
(runs on the builder inside the lane scratch) compile SRC single (or a group dir) and print an aligned diff of FN
against the overlay-A retail words."""
import sys, argparse, tempfile, difflib, json, struct, subprocess, re, os
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
score.ASM_DIR = Path(os.environ.get('TARGETS', 'asm/us/ovl_a')).resolve()

def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack('>%dI' % len(words), *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-z', '-b', 'binary', '-m', 'mips:4300', '-EB', f.name],
                             capture_output=True, text=True).stdout
    r = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m: r.append(re.sub(r'\s+', ' ', m.group(3)))
    return r

def norm_regs(x):
    x = re.sub(r'\$f\d+', 'F', x)
    x = re.sub(r'\b(zero|at|v[01]|a[0-3]|t\d|s[0-8]|ra|sp|gp|k[01])\b', 'R', x)
    x = re.sub(r'-?\d+\(R\)', 'M(R)', x)
    x = re.sub(r'0x[0-9a-f]+', 'N', x)
    return re.sub(r',-?\d+$', ',N', x)

ap = argparse.ArgumentParser(); ap.add_argument('src'); ap.add_argument('fn')
ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared'); ap.add_argument('--group', default='')
ap.add_argument('--mode', default='words'); ap.add_argument('--all', action='store_true'); ap.add_argument('--ctx', type=int, default=1)
ap.add_argument('--obj', default='')
ap.add_argument('--dump', default='')
a = ap.parse_args()
want = score.targets()[a.fn]
with tempfile.TemporaryDirectory() as t:
    obj = Path(t) / 'o.o'
    if a.group:
        score.compile_group(Path(a.group), obj)
    else:
        score.compile_single(a.src, a.flags, obj)
    if a.obj:
        import shutil; shutil.copy(obj, a.obj)
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[a.fn]
    end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, score.image_symbols())
    got = res[start // 4:end // 4]
while got and got[-1] == 0 and len(got) > len(want): got.pop()
dw, dg = dis(want), dis(got)
if a.dump:
    open(a.dump,'w').write('\n'.join('%04x: %s' % (4*i, x) for i, x in enumerate(dg)) + '\n')
if a.mode == 'ops': kw, kg = [x.split(' ')[0] for x in dw], [x.split(' ')[0] for x in dg]
elif a.mode == 'norm': kw, kg = [norm_regs(x) for x in dw], [norm_regs(x) for x in dg]
elif a.mode == 'nosp':
    f = lambda x: re.sub(r'-?[0-9]+\(sp\)', 'N(sp)', re.sub(r'sp,sp,-?[0-9]+', 'sp,sp,N', x))
    kw, kg = [f(x) for x in dw], [f(x) for x in dg]
else: kw, kg = want, got
sm = difflib.SequenceMatcher(None, kw, kg, autojunk=False)
rows = []
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag == 'equal':
        for k in range(i2 - i1): rows.append((' ', i1 + k, dw[i1 + k], dg[j1 + k]))
    else:
        n = max(i2 - i1, j2 - j1)
        for k in range(n):
            rows.append(('|', i1 + k if i1 + k < i2 else None, dw[i1 + k] if i1 + k < i2 else '', dg[j1 + k] if j1 + k < j2 else ''))
show = [a.all or r[0] == '|' for r in rows]
c = a.ctx
for i, r in enumerate(rows):
    if show[i] or any(show[max(0,i-c):i+c+1]):
        print('%s %5s  %-34s %s' % (r[0], ('+%03x' % (r[1] * 4)) if r[1] is not None else '', r[2], r[3]))
fr = lambda l: next((-int(re.match(r'addiu sp,sp,(-?\d+)', x).group(1)) for x in l if re.match(r'addiu sp,sp,(-?\d+)', x)), 0)
print('want %d words, got %d; differing rows %d; frame want %d got %d; unverified %d unresolved %d' % (len(want), len(got), sum(1 for r in rows if r[0] == '|'), fr(dw), fr(dg), len(unverified), len(unresolved)))
