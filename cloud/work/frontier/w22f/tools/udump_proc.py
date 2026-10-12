import sys
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import parse_ucode, MTYPE_NAMES
r=parse_ucode(sys.argv[1]); blk=sys.argv[2]; l0=int(sys.argv[3]); l1=int(sys.argv[4])
cur=None; procs=[]
for x in r:
    if x.name in ('ent',):
        cur=[x]; procs.append(cur)
    elif cur is not None: cur.append(x)
for p in procs:
    if p[0].words[1]==int(blk):
        line=0
        for x in p:
            if x.name=='loc': line=int(x.detail.split('line=')[1])
            if l0<=line<=l1:
                m=MTYPE_NAMES[x.mtype] if isinstance(x.mtype,int) and x.mtype<len(MTYPE_NAMES) else x.mtype
                print(x.index,'U'+x.name,m,x.dtype,x.detail)
        break
