import subprocess,re,itertools
hdr='''typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
#define E(a) D_8012E700[a]
'''
forms={
'f1':'E(a).flags = D_8012E700[(s16)a].flags | (X);',
'f2':'{ u32 t = D_8012E700[(s16)a].flags; E(a).flags = t | (X); }',
'f3':'{ u32 t = D_8012E700[(s16)a].flags; t |= (X); E(a).flags = t; }',
'f4':'{ u32 t = D_8012E700[(s16)a].flags; E(a).flags = t | ((X) & 0xFFFFFFFF); }',
'f5':'E(a).flags = D_8012E700[(s16)a].flags | (X);',
}
res=[]
for fk,f in forms.items():
 for ve in ['v << 8','(v & 0xFFFFFF) << 8','v * 256','(u32)(v << 8)', 'v << 8U']:
  for mtype in [None,'s32','u32']:
   for F in ['-O2']:
    def S(x):
        return f.replace('X',x)
    if mtype:
        vx='m'; pre='%s m = %s;'%(mtype,ve)
    else:
        vx=ve; pre=''
    b=hdr+'''void model_data_load(s32 a, s32 mode, s32 v) {
    %s
    if (mode == 2) {
        %s
        if (E(a).child != -1) model_data_load(E(a).child, 3, v);
    } else if (mode == 0) {
        %s
    } else if (mode == 1) {
        %s
    } else if (mode == 3) {
        do {
            %s
            if (E(a).child != -1) model_data_load(E(a).child, 3, v);
            a = E(a).sibling;
        } while (a != -1);
    }
}
'''%(pre,S(vx),S('0x80000000'),S(vx),S(vx))
    open('t_j.c','w').write(b)
    r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_j.c','model_data_load','--flags','-g0 %s -mips2 -G 0 -non_shared'%F],capture_output=True,text=True,cwd='../../..')
    tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
    m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
    if m_: res.append((int(m_.group(2)),int(m_.group(3)),m_.group(1),fk,ve,mtype,F))
    else: print(tt)
res.sort(key=lambda x:(x[0],x[1]),reverse=True)
for x in res[:10]: print(x)
