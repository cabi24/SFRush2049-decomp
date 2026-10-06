exec(open('ins.py').read().split("j=L.index")[0])
b=L.index('\tb\t$2549',200)
e=L.index('$2549:')
V={}
V['lx_before_epi']=L[:b]+['\tb\t$9990']+L[b+1:e]+['$9990:']+L[e:]
# all b $2549 / branches to $2549 in B region retarget
M=list(L); 
for i in range(b+1,e):
    if M[i].endswith('$2549') and 'beq\t$6, $2' in M[i]: M[i]=M[i].replace('$2549','$9990')
V['beq_to_lx']=M[:e]+['$9990:']+M[e:]
for k,v in V.items(): print(k,run(v,k),flush=True)
