# uproc.py OPT.u PROCIDX LINE0 LINE1: dump ucode of procedure PROCIDX between source lines (uses the vendored workbench parser)
import sys
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import parse_ucode, MTYPE_NAMES
r=parse_ucode(sys.argv[1]); idx=int(sys.argv[2]); l0=int(sys.argv[3]); l1=int(sys.argv[4])
procs=[];cur=None
for x in r:
    if x.name in('ent','aent'): cur=[x]; procs.append(cur)
    elif cur is not None: cur.append(x)
line=0
for x in procs[idx]:
    if x.name=='loc': line=int(x.detail.split('line=')[1])
    if l0<=line<=l1:
        print(x.index,'U'+x.name,MTYPE_NAMES[x.mtype] if isinstance(x.mtype,int) and x.mtype<len(MTYPE_NAMES) else x.mtype,x.dtype,x.detail,' '.join('%x'%w for w in x.words[1:]))
