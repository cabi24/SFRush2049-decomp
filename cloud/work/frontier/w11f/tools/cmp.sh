#!/bin/sh
# cmp.sh FUNC : side-by-side retail vs last unit build (numeric regs)
R=/home/cburnes/projects/rush2049-decomp
python3 $R/cloud/work/tools/tdis.py "$1" | tail -n +2 | awk '{ $1=""; print }' | sed 's/^ //' > /tmp/claude-1000/cmp_r.txt
sh $R/cloud/work/frontier/w11f/tools/udis.sh "$1" | awk -F'\t' 'NF>=2{print $2" "$3}' > /tmp/claude-1000/cmp_u.txt
python3 - <<'PY'
import re
names='zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp s8 ra'.split()
def norm(l):
    l=re.sub(r'\$(\d+)\b',lambda m:names[int(m.group(1))] if int(m.group(1))<32 else m.group(0),l)
    l=l.replace('$f','$f')
    l=re.sub(r'\s+',' ',l).strip()
    l=re.sub(r'<.*?>','',l).strip()
    l=re.sub(r'^or (\w+),(\w+),zero$',r'move \1,\2',l)
    l=re.sub(r'^addiu (\w+),zero,(-?\d+)$',r'li \1,\2',l)
    l=re.sub(r'^beq zero,zero,',r'b ',l)
    l=re.sub(r'^or (\w+),zero,zero$',r'move \1,zero',l)
    return l
r=[norm(x) for x in open('/tmp/claude-1000/cmp_r.txt')]
u=[norm(x) for x in open('/tmp/claude-1000/cmp_u.txt')]
import sys
bad=0
def strip(x): return re.sub(r'0x[0-9a-f]+|-?\d+(?=\()|\b[0-9a-f]{5,8}\b','N',x)
out=[]
for i in range(max(len(r),len(u))):
    a=r[i] if i<len(r) else ''; b=u[i] if i<len(u) else ''
    if strip(a)==strip(b) and a.split(' ')[0] not in ('b','beqz','bnez','beq','bne','beql','bnel','jal','j'): a2=b
    if strip(a)!=strip(b): bad+=1
    out.append('%3d %s %-40s %s'%(i,' ' if a==b else ('~' if a.split(' ')[0]==b.split(' ')[0] else '*'),a[:40],b))
import os
if not os.environ.get('Q'): print('\n'.join(out))
print('DIFF',bad,len(r),len(u))
PY
