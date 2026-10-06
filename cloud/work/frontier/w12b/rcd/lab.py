import subprocess,re,sys
L=open('listing/s.s').read().split('\n')
def run(lines,name):
    p='listing/l_%s.s'%name; open(p,'w').write('\n'.join(lines))
    out=subprocess.run(['sh','../tools/asmdis.sh','rcd',p],capture_output=True,text=True).stdout.split('\n')
    idx=[i for i,l in enumerate(out) if re.search(r'lb\tv0,0\(a1\)',l)]
    if not idx: return 'nolb'
    i=idx[-1]; return ' / '.join(x.split(':',1)[1].strip() for x in out[i:i+4])
def var(which, order):
    M=list(L)
    ln={132:'bne\t$3, $12, $2546',139:'beq\t$24, -1, $2546'}
    for k in which:
        assert M[k-1].strip()==ln[k], M[k-1]
        M[k-1]=M[k-1].replace('$2546','$999%d'%k)
    j=M.index('$2546:')
    ins=['$999%d:'%k for k in which]
    if order=='after': M[j+1:j+1]=ins
    else: M[j:j]=ins
    return M
for which in ([132],[139],[132,139]):
    for order in ('before','after'):
        n='%s_%s'%('_'.join(map(str,which)),order)
        print(n, run(var(which,order),n), flush=True)
