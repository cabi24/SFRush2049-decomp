import sys,re
f,fn,out=sys.argv[1:4]
L=open(f).read().split('\n')
first=next(i for i,l in enumerate(L) if re.match(r'\s*(LEAF|ENTRY|XLEAF)\(',l))
pre=L[:first]
s=next(i for i,l in enumerate(L) if re.match(r'\s*LEAF\(%s\)'%re.escape(fn),l))
e=next(i for i in range(s,len(L)) if re.match(r'\s*END\(%s\)'%re.escape(fn),L[i]))
open(out,'w').write('\n'.join(pre+['']+L[s:e+1])+'\n')
