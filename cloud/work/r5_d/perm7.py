import sys,itertools,os,tempfile
sys.path.insert(0,'.')
import gen7f,harness
from multiprocessing import Pool
names=['i','j','a','b','ia','ib','n','last','f','p','q']
T={'i':'s32','j':'s32','a':'s8','b':'s8','ia':'s16','ib':'s16','f':'s16'}
def build(order,sty,tform):
    t=gen7f.gen_gl(dict(sty=sty,tiei='i',ntype='s16',ltype='s32',tform=tform))
    # replace declaration block
    s=t.index('void func_800F7F3C(void)\n{\n')+len('void func_800F7F3C(void)\n{\n')
    e=t.index('  for (i = 0; i < D_80151AD0')
    decl=''.join('  %s %s;\n'%(T[x],x) for x in order)
    return t[:s]+decl+t[e:]
def work(args):
    order,sty,tform=args
    t=build(order,sty,tform)
    fd,path=tempfile.mkstemp(suffix='.c',dir='/tmp/claude-0/-home-user-SFRush2049-decomp/37f33778-7a6e-51d1-85db-44651954a6c5/scratchpad')
    os.write(fd,t.encode());os.close(fd)
    r=harness.evaluate(path,'func_800F7F3C'); os.unlink(path)
    return r,args
if __name__=='__main__':
    sty=sys.argv[1]; tform=sys.argv[2]
    base=['i','j','a','b','ia','ib']
    jobs=[(list(p),sty,tform) for p in itertools.permutations(base)]
    with Pool(8) as P:
        res=P.map(work,jobs)
    res=[x for x in res if x[0]]
    res.sort(key=lambda x:-x[0][1])
    for r in res[:8]: print(r)
