import sys,re,difflib
W=[];G=[]
for l in open(sys.argv[1]):
    if len(l)<10 or l.startswith('want') or 'FAIL' in l or 'EQUAL' in l: continue
    a=l[9:42].strip(); b=l[42:].strip()
    if a: W.append(a)
    if b: G.append(b)
def norm(x):
    x=re.sub(r'\$f\d+','F',x)
    x=re.sub(r'\b(zero|at|v[01]|a[0-3]|t\d|s[0-8]|ra|sp)\b','R',x)
    x=re.sub(r'-?\d+\(R\)','M(R)',x)
    x=re.sub(r'0x[0-9a-f]+','N',x)
    x=re.sub(r',-?\d+$',',N',x)
    return x
nw=[norm(x) for x in W]; ng=[norm(x) for x in G]
sm=difflib.SequenceMatcher(None,nw,ng,autojunk=False)
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag!='equal':
        print('---- %s retail[%d:%d] ours[%d:%d]'%(tag,i1,i2,j1,j2))
        for k in range(i1,i2): print('  R +%03x %s'%(k*4,W[k]))
        for k in range(j1,j2): print('  O       %s'%G[k])
