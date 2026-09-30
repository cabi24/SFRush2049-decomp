import subprocess,itertools,re
exec(open('gen8.py').read().split('ptypes=')[0])
decls=['s16 idx; Ent *e; s32 i;','s16 idx; s32 i; Ent *e;','s32 i; s16 idx; Ent *e;','Ent *e; s32 i; s16 idx;','s32 i; Ent *e;','s16 idx; s32 i;','s32 i;']
ptl=[('s32','s32','s16','s32'),('s32','s32','s16','void *'),('s32','s32','s16','u32'),('s32','void *','s16','s32'),('s32','u32','s16','u32'),('s32','s32','s16','f32 *')]
tails={'T1':'idx = i; render_mode_select(idx, c); return idx;','T2':'render_mode_select(i, c); return (s16)i;','T3':'render_mode_select(i, c); return i;'}
bodies={
'p1':'''    %s
    e = &D_8012E700[i];
    e->w0 = d; e->w4 = 0; e->w8 = b; e->f12 = 1.0f; e->f16 = 1.0f; e->h20 = a; e->h22 = -1; e->h24 = -1; e->h26 = -1;
    e->w28[0]=0;e->w28[1]=0;e->w28[2]=0;e->w28[3]=0;e->w28[4]=0;e->w28[5]=0;e->w28[6]=0;e->w28[7]=0;e->w28[8]=0;''',
'p2':'''    %s
    e = &D_8012E700[i];
    e->w0 = d; e->w4 = 0; e->w8 = b; e->h20 = a; e->h22 = -1; e->h24 = -1; e->h26 = -1;
    e->w28[0]=0;e->w28[1]=0;e->w28[2]=0;e->w28[3]=0;e->w28[4]=0;e->w28[5]=0;e->w28[6]=0;e->w28[7]=0;e->w28[8]=0; e->f12 = 1.0f; e->f16 = 1.0f;''',
}
res=[]
for dk,d in enumerate(decls):
 for pt in ptl:
  for tk,t in tails.items():
   for bk,bd in bodies.items():
    for F in ['-O3']:
     if 'Ent *e' not in d: continue
     if 'idx' not in d and tk=='T1': continue
     s=hdr.replace('s32 w0; s32 w4; s32 w8;','s32 w0; s32 w4; s32 w8;')+'''s32 func_8008E26C(%s a, %s b, %s c, %s d) {
    %s
    %s
    %s
    %s
    %s
}'''%(pt+(d,loops['L1'],mids['M1'],bd%'',t))
     s=s.replace('e->w0 = d;','e->w0 = (s32)d;').replace('e->w8 = b;','e->w8 = (s32)b;')
     open('t_n.c','w').write(s)
     r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_n.c','func_8008E26C','--flags','-g0 %s -mips2 -G 0 -non_shared'%F],capture_output=True,text=True,cwd='../../..')
     tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
     m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
     if m_: res.append((int(m_.group(2)),int(m_.group(3)),m_.group(1),dk,pt,tk,bk,F))
     else: print(tt[:150])
res.sort(key=lambda x:(x[0],x[1]),reverse=True)
for x in res[:8]: print(x)
