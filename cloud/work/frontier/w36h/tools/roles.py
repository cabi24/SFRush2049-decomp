#!/usr/bin/env python3
"""roles.py WANT.dis GOT.txt [--max N]: compare FP dataflow roles (not register names) over a difflib alignment.
For each aligned FP op (add/sub/mul/div/compare/convert/store), the rows that define its FP source operands are mapped
through the alignment; a mismatch means the same instruction has operands in a different ROLE (commutative swap or a
different definer), which register-normalised diffs hide."""
import re, sys, difflib
def norm(s):
    s = re.sub(r'<[^>]*>', '', s); s = re.sub(r'\$f\d+', 'F', s)
    s = re.sub(r'\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|gp|sp|ra|k[01])\b', 'R', s)
    return re.sub(r'-?0x[0-9a-f]+|-?\d+', 'N', s)
want=[]
for l in open(sys.argv[1]):
    m=re.match(r'\s*[0-9a-f]{8}:\s+[0-9a-f]{8}\s+(.*)',l)
    if m: want.append(re.sub(r'\s+',' ',m.group(1)).strip())
got=[]
for l in open(sys.argv[2]):
    m=re.match(r'\s*(\d+) (.*)',l)
    if m and not l.startswith('compiled'): got.append(m.group(2).strip())
mx=int(sys.argv[sys.argv.index('--max')+1]) if '--max' in sys.argv else 40
def defs(rows):
    last={}; out=[]
    for i,r in enumerate(rows):
        op=r.split()[0]; regs=re.findall(r'\$(f\d+)',r); srcs=[]; dst=None
        if op in('add.s','sub.s','mul.s','div.s'): dst=regs[0]; srcs=regs[1:]
        elif op.startswith('c.'): srcs=regs
        elif op in('lwc1','mtc1','mfc1_'): dst=regs[0] if regs else None
        elif op in('swc1','mfc1'): srcs=regs
        elif op in('abs.s','neg.s','mov.s','cvt.s.w','cvt.d.s','cvt.s.d','sqrt.s','trunc.w.s','cvt.w.s','floor.w.s'):
            dst=regs[0]; srcs=regs[1:]
        elif op in('jal','jalr'): last={k:v for k,v in last.items() if k not in('f4','f6','f8','f10','f16','f18','f0','f2','f12','f14')}
        out.append([last.get(s,-1) for s in srcs])
        if dst: last[dst]=i
    return out
dw=defs(want); dg=defs(got)
sm=difflib.SequenceMatcher(None,[norm(x) for x in want],[norm(x) for x in got],autojunk=False)
w2g={}; g2w={}
for t,i1,i2,j1,j2 in sm.get_opcodes():
    if t=='equal':
        for k in range(i2-i1): w2g[i1+k]=j1+k; g2w[j1+k]=i1+k
n=0; tot=0
for i,j in sorted(w2g.items()):
    if not dw[i] and not dg[j]: continue
    tot+=1
    a=dw[i]; b=[g2w.get(x,-2) if x>=0 else -1 for x in dg[j]]
    if a!=b:
        n+=1
        if n<=mx: print(f'want row {i} got row {j} {want[i].split()[0]}: want srcs {a} got srcs(mapped) {b} | {want[i]} || {got[j]}')
print(f'role-differing aligned FP rows: {n} of {tot}  (alignment covers {len(w2g)}/{len(want)} want rows)')
