import itertools,sys
sys.path.insert(0,'.')
import gen7f,harness
for tform,sty in itertools.product(['goto','dw','for'],['named','plain','idx']):
    opt=dict(sty=sty,tiei='i',ntype='s16',ltype='s32',tform=tform)
    open('tmp7.c','w').write(gen7f.gen_gl(opt))
    print(opt,harness.evaluate('tmp7.c','func_800F7F3C'),flush=True)
