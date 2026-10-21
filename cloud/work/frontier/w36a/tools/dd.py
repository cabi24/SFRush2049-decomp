#!/usr/bin/env python3
"""dd.py SRC NAME [--flags F] [--ctx N] [--img ovl_a]: compile on builder, aligned diff here."""
import argparse, json, re, struct, subprocess, sys, tempfile, difflib, bisect
from pathlib import Path
ROOT = Path('/home/cburnes/projects/rush2049-decomp')
ap = argparse.ArgumentParser()
ap.add_argument('src'); ap.add_argument('name')
ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared')
ap.add_argument('--img', default='ovl_a')
ap.add_argument('--ctx', type=int, default=3)
ap.add_argument('--full', action='store_true')
ap.add_argument('--norm', action='store_true')
ap.add_argument('--dump', default=None)
ap.add_argument('--nosp', action='store_true')
ap.add_argument('--obj', default=None)
ap.add_argument('--maxlines', type=int, default=400)
a = ap.parse_args()
S = 'rush2049/scratch/frontier/w35b'
if a.obj:
    sys.path.insert(0, str(ROOT / 'tools' / 'cloud'))
    import score
    score.ASM_DIR = (ROOT / 'asm' / 'us' / a.img).resolve()
    want = score.targets()[a.name]
    words = score.text_words(a.obj)
    fns = score.symbols(a.obj)
    start = fns[a.name]
    end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    res, masks_, unres, unv, errs = score.relocate(a.obj, words, start, end, score.image_symbols())
    d = dict(want=want, got=res[start // 4:end // 4], masks={str((k - start) // 4): v for k, v in masks_.items() if start <= k < end}, unres=unres, errs=errs)
else:
    if Path(a.src).is_dir():
        gn = 'G_' + Path(a.src).name
        subprocess.run(['ssh', 'watchman2', f'mkdir -p {S}/cand/{gn}'], check=True)
        subprocess.run(['scp', '-q', a.src + '/group.c', a.src + '/group.json', f'watchman2:{S}/cand/{gn}/'], check=True)
        tgt = f'cand/{gn}'
    else:
        subprocess.run(['scp', '-q', a.src, f'watchman2:{S}/cand/{a.name}.c'], check=True)
        tgt = f'cand/{a.name}.c'
    cmd = (f"cd {S} && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido "
           f"python3 gw.py {tgt} {a.name} '{a.flags}' asm/us/{a.img}")
    p = subprocess.run(['ssh', 'watchman2', cmd], capture_output=True, text=True)
    if p.returncode:
        print(p.stderr[-3000:] or p.stdout[-3000:]); sys.exit(1)
    d = json.loads(p.stdout)
want, got = d['want'], d['got']
masks = {int(k): v for k, v in d['masks'].items()}
def mk(i, w): return w & masks.get(i, 0xFFFFFFFF)
# disassemble
def dis(words, base):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB', '-M', 'gpr-names=32', f.name], capture_output=True, text=True).stdout
    r = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m: r.append(re.sub(r'\s+', ' ', m.group(3)))
    return r
wd, gd = dis(want, 0), dis(got, 0)
if a.dump:
    open(a.dump,'w').write('\n'.join(f'{i:4d} {x:08x} {gd[i]}' for i,x in enumerate(got))+'\n')
def norm_ins(ins, i, masked):
    return ins
# alignment on masked words
wk = [w for w in want]
gk = [mk(i, w) if False else w for i, w in enumerate(got)]
# compare ignoring relocation-masked bits
def key_w(i): return want[i] & masks.get(i, 0xFFFFFFFF) if False else want[i]
def nz(s):
    s = re.sub(r'\b(s[0-8]|t[0-9]|v[01]|a[0-3])\b', lambda m: {'s':'S','t':'T','v':'V','a':'A'}[m.group(1)[0]], s)
    s = re.sub(r'\$f\d+', '$F', s)
    if re.match(r'(b\w*|beq\w*|bne\w*|bgez\w*|blez\w*|bgtz\w*|bltz\w*|j)\s', s) and 'jal' not in s: s = re.sub(r'0x[0-9a-f]+$', 'X', s)
    if a.nosp: s = re.sub(r'-?\d+\(sp\)', 'N(sp)', s); s = re.sub(r'addiu sp,sp,-?\d+', 'addiu sp,sp,N', s)
    return s
if a.norm:
    sm = difflib.SequenceMatcher(None, [nz(x) for x in wd], [nz(x) for x in gd], autojunk=False)
else:
    sm = difflib.SequenceMatcher(None, [x & 0xFFFFFFFF for x in want], [x for x in got], autojunk=False)
def eq_masked(wi, gi):
    m = masks.get(gi, 0xFFFFFFFF)
    return (want[wi] & m) == (got[gi] & m)
ops = sm.get_opcodes()
nd = 0
lines = []
for tag, i1, i2, j1, j2 in ops:
    if tag == 'equal':
        lines.append(('eq', i1, i2, j1, j2))
    else:
        lines.append((tag, i1, i2, j1, j2))
# count words that differ under positional compare (scorer metric)
pos_bad = sum(1 for i in range(min(len(want), len(got))) if not eq_masked(i, i)) + max(0, len(want) - len(got))
def lcs_stat(keyw, keyg):
    sm2 = difflib.SequenceMatcher(None, keyw, keyg, autojunk=False)
    m = sum(b.size for b in sm2.get_matching_blocks())
    return len(keyw) - m, len(keyg) - m
ex = lcs_stat([x for x in want], [x for x in got])
nm = lcs_stat([nz(x) for x in wd], [nz(x) for x in gd])
print(f'lcs: exact -{ex[0]}/+{ex[1]}  norm -{nm[0]}/+{nm[1]}', file=sys.stderr)
print(f'{a.name}: want {len(want)} words, got {len(got)} words, positional diffs {pos_bad} lcs-exact -{ex[0]}/+{ex[1]} lcs-norm -{nm[0]}/+{nm[1]}, unresolved {d["unres"]} errs {d["errs"]}')
out = []
for tag, i1, i2, j1, j2 in lines:
    if tag == 'eq':
        if a.full:
            for k in range(i2 - i1): out.append(f'  {i1+k:4d}/{j1+k:4d}   {wd[i1+k]}')
        else:
            n = i2 - i1
            if n > 2 * a.ctx:
                for k in range(a.ctx): out.append(f'  {i1+k:4d}/{j1+k:4d}   {wd[i1+k]}')
                out.append(f'      ... {n - 2*a.ctx} equal')
                for k in range(n - a.ctx, n): out.append(f'  {i1+k:4d}/{j1+k:4d}   {wd[i1+k]}')
            else:
                for k in range(n): out.append(f'  {i1+k:4d}/{j1+k:4d}   {wd[i1+k]}')
    else:
        for k in range(i1, i2): out.append(f'- {k:4d}        {wd[k]}')
        for k in range(j1, j2): out.append(f'+      {k:4d}   {gd[k]}')
for l in out[:a.maxlines]: print(l)
if len(out) > a.maxlines: print(f'... ({len(out)-a.maxlines} more lines)')
