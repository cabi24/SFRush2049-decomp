import sys
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import parse_ucode, MTYPE_NAMES
r=parse_ucode(sys.argv[1])
need=set(int(x,0) for x in sys.argv[2].split(','))
excl=set(int(x,0) for x in sys.argv[3].split(',')) if len(sys.argv)>3 else set()
procs=[];cur=None
for x in r:
    if x.name=='ent': cur=[x]; procs.append(cur)
    elif cur is not None: cur.append(x)
for p in procs:
    cs=set(x.integer_constant for x in p if x.name=='ldc')
    if need<=cs and not (excl & cs):
        for x in p[:int(sys.argv[4]) if len(sys.argv)>4 else None]:
            print(x.index, 'U'+x.name, MTYPE_NAMES[x.mtype], x.dtype, x.lexlev, ' '.join('%x'%w for w in x.words[1:]))
        break
