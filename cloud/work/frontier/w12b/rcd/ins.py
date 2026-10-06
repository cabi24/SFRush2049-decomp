import subprocess,re,sys
L=open('listing/s.s').read().split('\n')
def run(lines,name):
    p='listing/i_%s.s'%name; open(p,'w').write('\n'.join(lines))
    out=subprocess.run(['sh','../tools/asmdis.sh','rcd',p],capture_output=True,text=True).stdout.split('\n')
    idx=[i for i,l in enumerate(out) if re.search(r'lb\tv0,0\(a1\)',l)]
    if not idx: return 'nolb'
    i=idx[-1]; return ' / '.join(x.split(':',1)[1].strip() for x in out[i:i+4])+' | words %d'%len([l for l in out if l.strip()])
j=L.index('$2546:')
V={}
V['emptylab']=L[:j]+['$9999:']+L[j:]
V['emptylab_noalias']=L[:j-1]+['$9999:']+L[j-1:]
V['nop']=L[:j]+['\tnop']+L[j:]
for k,v in V.items(): print(k,run(v,k),flush=True)
