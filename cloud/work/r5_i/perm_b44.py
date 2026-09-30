import itertools, re, sys, shutil, tempfile, subprocess
from pathlib import Path
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
d=Path('/home/user/SFRush2049-decomp/cloud/work/ipa-groups/audio_heap')
base=(d/'group.c').read_text()
ty={'heap':'Heap *heap','count':'u16 count','a':'u16 a','pad':'s32 pad'}
tyalt={'heap':'Heap *heap','count':'s32 count','a':'s32 a','pad':'s32 pad'}
res=[]
for names in itertools.permutations(['heap','count','a','pad']):
  for T in (ty,tyalt):
    decl=', '.join(T[n] for n in names)
    def call(h): return ', '.join({'heap':h,'count':'count','a':'a','pad':'0'}[n] for n in names)
    s=base.replace("void func_800E7B44(Heap *heap, u16 count, u16 a)","void func_800E7B44(%s)"%decl)
    s=s.replace("void func_800E7B44(Heap *heap, u16 count, u16 a);","void func_800E7B44(%s);"%decl)
    s=s.replace("func_800E7B44(h, count, a);","func_800E7B44(%s);"%call('h')).replace("func_800E7B44(D_801527C8, count, a);","func_800E7B44(%s);"%call('D_801527C8'))
    t=Path(tempfile.mkdtemp()); shutil.copy(d/'group.json',t/'group.json'); (t/'group.c').write_text(s)
    try:
        obj=t/'o.o'; score.compile_group(t,obj); c=score.compare(obj,'func_800E7B44',show=0)
        res.append((c.differing,decl))
    except SystemExit: pass
    shutil.rmtree(t)
for r in sorted(res)[:8]: print(r)
