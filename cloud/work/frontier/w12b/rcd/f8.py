import subprocess,re
L=['\t.text','\t.globl\tfunc_800F8EC8']+open('f8/s.s').read().split('\n')
def run(lines,name):
    p='f8/v_%s.s'%name; open(p,'w').write('\n'.join(lines))
    out=subprocess.run(['sh','../tools/asmdis.sh','rcd',p],capture_output=True,text=True).stdout.split('\n')
    idx=[i for i,l in enumerate(out) if re.search(r'lbu\tv0,',l)]
    if not idx: return 'nolbu '+str(len(out))
    i=idx[0]; return ' / '.join(x.split(':',1)[1].strip() for x in out[i:i+4])
k=[i for i,l in enumerate(L) if l.strip()=='lbu\t$2, D_80153E88+7($24)'][0]
V={}
V['orig']=L
V['labb']=L[:k-1]+['\tb\t$9999','$9999:']+L[k-1:]
V['lab_only']=L[:k-1]+['$9999:']+L[k-1:]
for n,v in V.items(): print(n,run(v,n),flush=True)
