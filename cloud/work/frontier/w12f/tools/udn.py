import sys, struct
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import parse_ucode, MTYPE_NAMES
r=parse_ucode(sys.argv[1]); name=sys.argv[2].encode()
procs=[];cur=None
for x in r:
    if x.name=='ent': cur=[x]; procs.append(cur)
    elif cur is not None: cur.append(x)
for p in procs:
    c=p[1]
    if c.name!='comm': continue
    b=b''.join(struct.pack('>I',w) for w in c.words[1:])
    if name+b'\0' not in b: continue
    for x in p:
        print(x.index, 'U'+x.name, MTYPE_NAMES[x.mtype], x.dtype, x.lexlev, ' '.join('%x'%w for w in x.words[1:]))
    break
