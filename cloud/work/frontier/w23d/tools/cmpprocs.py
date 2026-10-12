import sys,hashlib
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import parse_ucode
def procs(f):
    cur=None; out=[]
    for x in parse_ucode(f):
        if x.name=='ent': cur=[x]; out.append(cur)
        elif cur is not None: cur.append(x)
    d={}
    for p in out:
        sig=hashlib.md5(' '.join(x.name for x in p if x.name not in('loc',)).encode()).hexdigest()
        d.setdefault(sig,[]).append((p[0].words[1],sum(1 for x in p if x.name=='loc'),len(p)))
    return d
a=procs(sys.argv[1]); b=procs(sys.argv[2])
for s in b:
    if s in a:
        la=a[s][0]; lb=b[s][0]
        print(lb[0], la[0], 'n=',lb[2], 'loc grp',lb[1],'unit',la[1], '' if la[1]==lb[1] else '<<')
    else: print(b[s][0][0],'no unit match', b[s][0][2])
