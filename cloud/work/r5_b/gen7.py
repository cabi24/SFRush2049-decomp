import subprocess,itertools,re
src=open('func_8008E26C.c').read()
vars_=[]
for ret in ['s32','s16']:
  for ityp in ['s32','s16']:
    for pa in ['s32','s16','u16']:
      for pc in ['s16','s32']:
        s=src.replace('s32 func_8008E26C(s32 a, s32 b, s16 c, s32 d) {\n    s32 i;','%s func_8008E26C(%s a, s32 b, %s c, s32 d) {\n    %s i;'%(ret,pa,pc,ityp))
        vars_.append(((ret,ityp,pa,pc),s))
for k,s in vars_:
    open('t_b.c','w').write(s)
    for F in ['-O2','-O3']:
        r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_b.c','func_8008E26C','--flags','-g0 %s -mips2 -G 0 -non_shared'%F],capture_output=True,text=True,cwd='../../..')
        print(k,F,r.stdout.strip().splitlines()[-1][:110])
