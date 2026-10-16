import sys, collections
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import parse_ucode, MTYPE_NAMES
r=parse_ucode(sys.argv[1])
need=set(int(x,0) for x in sys.argv[2].split(",")) if len(sys.argv)>2 and sys.argv[2] else set()
procs=[];cur=None
for x in r:
    if x.name=='ent': cur=[x]; procs.append(cur)
    elif cur is not None: cur.append(x)
def sg(v): return v-(1<<32) if v&0x80000000 else v
for p in procs:
    cs=set(x.integer_constant for x in p if x.name=='ldc')
    if not need<=cs: continue
    offs=collections.OrderedDict(); line=0
    for x in p:
        if x.name=='loc': line=x.words[1]
        if x.name=='def' and MTYPE_NAMES[x.mtype]=='Mmt': print('Udef Mmt', ' '.join('%x'%w for w in x.words[1:]))
        if x.name in ('lod','str','lda','ilod','istr','vreg') and (MTYPE_NAMES[x.mtype] if x.mtype<len(MTYPE_NAMES) else "")=='Mmt':
            o=sg(x.words[3]); ln=x.words[2]
            offs.setdefault(o,[]).append('%s%s@%d'%(x.name,'' if x.name=='lda' else ln,line))
    for o in sorted(offs):
        v=offs[o]; print('%6d  n=%-3d %s'%(o,len(v),' '.join(v[:6])))
