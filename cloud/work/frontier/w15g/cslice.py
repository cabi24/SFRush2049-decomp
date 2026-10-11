import sys,re
f,keep,out=sys.argv[1:4]
keep=set(keep.split(','))
L=open(f).read().split('\n')
res=[];i=0
while i<len(L):
    l=L[i]
    m=re.match(r'^[A-Za-z_][\w\s\*]*?\b(\w+)\s*\(',l)
    def isdef(i):
        for k in range(i,min(i+4,len(L))):
            if ';' in L[k] and '{' not in L[k]: return False
            if '{' in L[k]: return True
        return False
    if m and not l.startswith('#') and isdef(i):
        name=m.group(1); depth=0; j=i; started=False
        while True:
            # ignore preprocessor lines when counting
            if not L[j].startswith('#'):
                depth+=L[j].count('{')-L[j].count('}')
                if '{' in L[j]: started=True
            if started and depth==0: break
            j+=1
        if name in keep: res+=L[i:j+1]
        i=j+1; continue
    res.append(l); i+=1
open(out,'w').write('\n'.join(res))
