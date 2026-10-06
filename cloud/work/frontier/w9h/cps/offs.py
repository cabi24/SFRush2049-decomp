import sys,re,collections
w=collections.Counter(); g=collections.Counter()
for l in open(sys.argv[1]):
    if len(l)<10 or l.startswith('want'): continue
    a=l[9:42]; b=l[42:]
    for m in re.findall(r'(-?\d+)\(sp\)',a): w[int(m)]+=1
    for m in re.findall(r'(-?\d+)\(sp\)',b): g[int(m)]+=1
f=lambda c:' '.join('%d:%d'%(k,c[k]) for k in sorted(c) if k>=112)
print('retail',f(w)); print('ours  ',f(g))
