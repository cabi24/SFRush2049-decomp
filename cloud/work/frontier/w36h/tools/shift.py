#!/usr/bin/env python3
"""shift.py WANT.dis GOT.txt: along the difflib-aligned rows print runs of (want-got) ring shift for GP temps (t6 t7 t8 t9) and FP temps (f4 f6 f8 f10)."""
import re, sys, difflib
from itertools import groupby
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
sm=difflib.SequenceMatcher(None,[norm(x) for x in want],[norm(x) for x in got],autojunk=False)
GP=['t6','t7','t8','t9']; FP=['f4','f6','f8','f10']
ev=[]
for t,i1,i2,j1,j2 in sm.get_opcodes():
    if t!='equal': continue
    for k in range(i2-i1):
        w=want[i1+k]; g=got[j1+k]
        RX=r'(?<![\w$])(t[6-9]|\$f(?:4|6|8|10))(?!\w)'
        wr=re.findall(RX,w); gr=re.findall(RX,g)
        wr=[x.replace('$','') for x in wr]; gr=[x.replace('$','') for x in gr]
        if len(wr)!=len(gr): continue
        for a,b in zip(wr,gr):
            R=GP if a[0]=='t' else FP
            if b in R: ev.append((i1+k,'GP' if a[0]=='t' else 'FP',(R.index(a)-R.index(b))%4))
mode=sys.argv[3] if len(sys.argv)>3 else 'GP'
sel=[e for e in ev if e[1]==mode]
for key,grp in groupby(sel,key=lambda e:e[2]):
    g=list(grp); print(f'{mode} shift {key}: {len(g)} regs rows {g[0][0]}-{g[-1][0]}')
