import subprocess,itertools,re
hdr='''typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { s32 w0; s32 w4; s32 w8; f32 f12; f32 f16; u16 h20; s16 h22; s16 h24; s16 h26; s32 w28[9]; s32 w64; } Ent;
extern Ent D_8012E700[];
extern s32 D_80156990;
extern s32 D_801569A8;
void render_mode_select(s16 a, s16 b);
'''
loops={
'L1':'for (i = 0; i < D_80156990; i++) { if (D_8012E700[i].h20 == 0xFFFF) break; }',
'L2':'i = 0; while (i < D_80156990) { if (D_8012E700[i].h20 == 0xFFFF) break; i++; }',
'L3':'for (i = 0; i < D_80156990 && D_8012E700[i].h20 != 0xFFFF; i++) ;',
'L4':'for (i = 0; i < D_80156990; i++) { if (D_8012E700[i].h20 == (u16)-1) break; }',
}
mids={
'M1':'if (i == D_80156990) D_80156990++;\n if (D_801569A8 < D_80156990) D_801569A8 = D_80156990;',
'M2':'if (i == D_80156990) { D_80156990 = i + 1; }\n if (D_801569A8 < D_80156990) D_801569A8 = D_80156990;',
'M3':'if (i == D_80156990) { D_80156990 = D_80156990 + 1; }\n if (D_80156990 > D_801569A8) D_801569A8 = D_80156990;',
}
stores={
'S1':'''    e = &D_8012E700[i];
    e->w0 = d; e->w4 = 0; e->w8 = b; e->f12 = 1.0f; e->f16 = 1.0f; e->h20 = a; e->h22 = -1; e->h24 = -1; e->h26 = -1;
    e->w28[0]=0;e->w28[1]=0;e->w28[2]=0;e->w28[3]=0;e->w28[4]=0;e->w28[5]=0;e->w28[6]=0;e->w28[7]=0;e->w28[8]=0;''',
'S2':'''    e = &D_8012E700[i];
    e->w0 = d; e->w4 = 0; e->w8 = b; e->f12 = 1.0f; e->f16 = 1.0f; e->h20 = a; e->h22 = -1; e->h24 = -1; e->h26 = -1;
    for (j = 0; j < 9; j++) e->w28[j] = 0;''',
'S3':'''    D_8012E700[i].w0 = d; D_8012E700[i].w4 = 0; D_8012E700[i].w8 = b; D_8012E700[i].f12 = 1.0f; D_8012E700[i].f16 = 1.0f;
    D_8012E700[i].h20 = a; D_8012E700[i].h22 = -1; D_8012E700[i].h24 = -1; D_8012E700[i].h26 = -1;
    D_8012E700[i].w28[0]=0;D_8012E700[i].w28[1]=0;D_8012E700[i].w28[2]=0;D_8012E700[i].w28[3]=0;D_8012E700[i].w28[4]=0;D_8012E700[i].w28[5]=0;D_8012E700[i].w28[6]=0;D_8012E700[i].w28[7]=0;D_8012E700[i].w28[8]=0;''',
}
ptypes=[('s32','s32','s16','s32'),('s16','s32','s16','s32'),('u16','s32','s16','s32'),('s32','s32','s16','u32')]
res=[]
for (lk,l),(mk,m),(sk,st),pt,F in itertools.product(loops.items(),mids.items(),stores.items(),ptypes,['-O2','-O3']):
    s=hdr+'''s32 func_8008E26C(%s a, %s b, %s c, %s d) {
    s32 i; s32 j; s16 idx; Ent *e;
    %s
    %s
    %s
    idx = i;
    render_mode_select(idx, c);
    return idx;
}'''%(pt+(l,m,st))
    open('t_c.c','w').write(s)
    r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_c.c','func_8008E26C','--flags','-g0 %s -mips2 -G 0 -non_shared'%F],capture_output=True,text=True,cwd='../../..')
    t=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
    m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',t)
    if m_:
        res.append((int(m_.group(3)),int(m_.group(2)),m_.group(1),lk,mk,sk,pt,F))
res.sort(reverse=True)
for x in res[:10]: print(x)
