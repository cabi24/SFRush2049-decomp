import sys,re
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import parse_ucode, MTYPE_NAMES
r=parse_ucode(sys.argv[1]); blk=int(sys.argv[2])
cur=None; procs=[]
for x in r:
    if x.name=='ent': cur=[x]; procs.append(cur)
    elif cur is not None: cur.append(x)
for p in procs:
    if p[0].words[1]==blk:
        for x in p:
            m=MTYPE_NAMES[x.mtype] if isinstance(x.mtype,int) and x.mtype<len(MTYPE_NAMES) else x.mtype
            d=re.sub(r'block=%d\b'%blk,'block=B',str(x.detail))
            print('U'+x.name,m,x.dtype,d)
