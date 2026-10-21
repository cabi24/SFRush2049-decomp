#!/usr/bin/env python3
"""iroles.py WANT.dis GOT.txt [--max N]: like roles.py but for INTEGER registers (temps t0-t9, v0/v1, a0-a3, at).
For each aligned instruction, map the rows that define its integer source operands through the alignment; a
mismatch = operand-order/definer difference that a register-normalised diff hides."""
import re, sys, difflib
INT = r'\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|gp|sp|ra|k[01])\b'
def norm(s):
    s = re.sub(r'<[^>]*>', '', s); s = re.sub(r'\$f\d+', 'F', s)
    s = re.sub(INT, 'R', s)
    return re.sub(r'-?0x[0-9a-f]+|-?\d+', 'N', s)
def rd(path, pat):
    out=[]
    for l in open(path):
        m=re.match(pat,l)
        if m: out.append(re.sub(r'\s+',' ',m.group(1)).strip())
    return out
want=rd(sys.argv[1], r'\s*[0-9a-f]{8}:\s+[0-9a-f]{8}\s+(.*)')
got=[re.match(r'\s*\d+ (.*)',l).group(1).strip() for l in open(sys.argv[2]) if re.match(r'\s*\d+ ',l) and not l.startswith('compiled')]
mx=int(sys.argv[sys.argv.index('--max')+1]) if '--max' in sys.argv else 40
TEMPS={'at','v0','v1','a0','a1','a2','a3','t0','t1','t2','t3','t4','t5','t6','t7','t8','t9'}
NODST={'sw','sh','sb','swc1','sdc1','beq','bne','beql','bnel','blez','bgtz','bltz','bgez','beqz','bnez','beqzl','bnezl','blezl','bgtzl','bltzl','bgezl','jr','mtc1','mult','multu','div','divu','jalr'}
def defs(rows):
    last={}; out=[]
    for i,r in enumerate(rows):
        p=r.replace(',',' ').replace('(',' ').replace(')',' ').split()
        op=p[0]; regs=[x for x in p[1:] if re.fullmatch(INT,x)]
        if op in NODST or op.startswith('b'): dst=None; srcs=regs
        elif op in ('jal',): last={k:v for k,v in last.items() if k not in TEMPS}; out.append([]); last['v0']=i; continue
        elif op=='mfc1' or op=='mflo' or op=='mfhi': dst=regs[0] if regs else None; srcs=[]
        elif op=='mtc1': dst=None; srcs=regs[:1]
        else: dst=regs[0] if regs else None; srcs=regs[1:]
        if op in ('lwc1','swc1','ldc1','sdc1'): dst=None; srcs=regs
        out.append([last.get(s,-1) if s in TEMPS else s for s in srcs])
        if dst and dst in TEMPS: last[dst]=i
    return out
dw=defs(want); dg=defs(got)
sm=difflib.SequenceMatcher(None,[norm(x) for x in want],[norm(x) for x in got],autojunk=False)
w2g={}; g2w={}
for t,i1,i2,j1,j2 in sm.get_opcodes():
    if t=='equal':
        for k in range(i2-i1): w2g[i1+k]=j1+k; g2w[j1+k]=i1+k
n=0; tot=0
for i,j in sorted(w2g.items()):
    a=dw[i]; b=[(g2w.get(x,-2) if x>=0 else -1) if isinstance(x,int) else x for x in dg[j]]
    if not a and not b: continue
    tot+=1
    if a!=b:
        n+=1
        if n<=mx: print(f'want {i} got {j}: want {a} got {b} | {want[i]} || {got[j]}')
print(f'role-differing aligned int rows: {n} of {tot} (alignment {len(w2g)}/{len(want)})')
