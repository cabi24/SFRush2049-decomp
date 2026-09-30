import subprocess,itertools,re
exec(open('gen8.py').read().split('res=[]')[0])
bodies={}
bodies['A']='''    s16 idx; Ent *e; s32 i; s32 n;
    n = D_80156990;
    for (i = 0; i < n; i++) { if (D_8012E700[i].h20 == 0xFFFF) break; }
    if (i == n) { n++; D_80156990 = n; }
    if (D_801569A8 < n) D_801569A8 = n;
%s
    render_mode_select(i, c); return (s16)i;'''
bodies['B']='''    s16 idx; Ent *e; s32 i;
    for (i = 0; i < D_80156990; i++) { if (D_8012E700[i].h20 == 0xFFFF) break; }
    if (i == D_80156990) D_80156990++;
    if (D_801569A8 < D_80156990) D_801569A8 = D_80156990;
    e = &D_8012E700[i];
%s
    render_mode_select(i, c); return (s16)i;'''
bodies['C']='''    s16 idx; Ent *e; s32 i;
    for (i = 0; i < D_80156990; i++) { if (D_8012E700[i].h20 == 0xFFFF) break; }
    if (i == D_80156990) D_80156990++;
    if (D_801569A8 < D_80156990) D_801569A8 = D_80156990;
    e = &D_8012E700[i];
%s
    idx = i;
    render_mode_select(idx, c); return idx;'''
bodies['D']='''    s16 idx; Ent *e; s32 i;
    for (i = 0; i < D_80156990; i++) { if (D_8012E700[i].h20 == 0xFFFF) break; }
    if (i == D_80156990) D_80156990++;
    if (D_801569A8 < D_80156990) D_801569A8 = D_80156990;
    e = &D_8012E700[i]; idx = i;
%s
    render_mode_select(idx, c); return idx;'''
st2='''    e->w0 = d; e->w4 = 0; e->w8 = b; e->f12 = 1.0f; e->f16 = 1.0f; e->h20 = a; e->h22 = -1; e->h24 = -1; e->h26 = -1;
    e->w28[0]=0;e->w28[1]=0;e->w28[2]=0;e->w28[3]=0;e->w28[4]=0;e->w28[5]=0;e->w28[6]=0;e->w28[7]=0;e->w28[8]=0;'''
st3='''    e->w0 = d; e->w4 = 0; e->w8 = b; e->h20 = a; e->h22 = -1; e->h24 = -1; e->h26 = -1;
    e->w28[0]=0;e->w28[1]=0;e->w28[2]=0;e->w28[3]=0;e->w28[4]=0;e->w28[5]=0;e->w28[6]=0;e->w28[7]=0;e->w28[8]=0; e->f12 = 1.0f; e->f16 = 1.0f;'''
res=[]
for bk,b in bodies.items():
  for sk,st in [('st2',st2),('st3',st3)]:
   for ptk,pt in [('s32',('s32','s32','s16','s32')),('s32u',('s32','s32','s16','u32')),('ptr',('s32','void *','s16','s32'))]:
    for F in ['-O2','-O3']:
        body=b%st
        s=hdr+'s32 func_8008E26C(%s a, %s b, %s c, %s d) {\n%s\n}'%(pt+(body,))
        open('t_c.c','w').write(s)
        r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_c.c','func_8008E26C','--flags','-g0 %s -mips2 -G 0 -non_shared'%F],capture_output=True,text=True,cwd='../../..')
        tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
        m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
        if m_: res.append((int(m_.group(3)),int(m_.group(2)),m_.group(1),bk,sk,ptk,F))
        else: print(tt[:100])
res.sort(reverse=True)
for x in res[:10]: print(x)
