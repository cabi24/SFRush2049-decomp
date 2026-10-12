import sys,re
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import parse_ucode
src,blk,lo,hi,out=sys.argv[1],int(sys.argv[2]),int(sys.argv[3]),int(sys.argv[4]),sys.argv[5]
data=open(src,'rb').read()
recs=parse_ucode(src)
drop=[];inproc=False
for x in recs:
    if x.name=='ent': inproc=(x.words[1]==blk)
    if inproc and x.name=='loc':
        line=int(str(x.detail).split('line=')[1])
        if lo<=line<=hi: drop.append((x.byte_offset,len(x.words)*4))
print('dropping',len(drop))
res=bytearray();pos=0
for o,l in drop: res+=data[pos:o]; pos=o+l
res+=data[pos:]
open(out,'wb').write(res)
