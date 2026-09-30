import sys,itertools,os,tempfile
sys.path.insert(0,'.')
import gen7f,harness
from multiprocessing import Pool
SP='/tmp/claude-0/-home-user-SFRush2049-decomp/37f33778-7a6e-51d1-85db-44651954a6c5/scratchpad'
def build(o):
    t=gen7f.gen_gl(dict(sty=o['sty'],tiei='i',ntype='s16',ltype='s32',tform='for'))
    for key in ["D_80152038[D_8014A250[D_80143F54[0]].idx].key","D_80152818[D_8014A250[D_80143F54[0]].idx].b931","D_80152818[D_8014A250[D_80143F54[0]].idx].b238"]:
        kk=key.replace("D_8014A250[D_80143F54[0]].idx","f")
        t=t.replace("      if (%s =="%key,"      f = D_8014A250[D_80143F54[0]].idx;\n      if (%s =="%kk)
    t=t.replace("  s8 a;","  %s a;"%o['ab']).replace("  s8 b;","  %s b;"%o['ab'])
    t=t.replace("  s16 ia;","  %s ia;"%o['iab']).replace("  s16 ib;","  %s ib;"%o['iab'])
    t=t.replace("  s16 f;","  %s f;"%o['f'])
    if o['lt']=='s16': t=t.replace("  s32 i;","  s16 i;")
    return t
def work(o):
    fd,path=tempfile.mkstemp(suffix='.c',dir=SP); os.write(fd,build(o).encode()); os.close(fd)
    r=harness.evaluate(path,'func_800F7F3C'); os.unlink(path); return r,o
if __name__=='__main__':
    jobs=[dict(sty=s,ab=ab,iab=iab,f=f,lt='s32') for s in ['idx','named','plain'] for ab in ['s8','s32','u8'] for iab in ['s16','s32'] for f in ['s16','s32']]
    with Pool(4) as P: res=P.map(work,jobs)
    res=[x for x in res if x[0]]; res.sort(key=lambda x:(-x[0][1]))
    for r in res[:10]: print(r)
