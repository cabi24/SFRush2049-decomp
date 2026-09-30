import subprocess,re,itertools
hdr='''typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
#define E(a) D_8012E700[a]
'''
def S(x): return '{ u32 t = D_8012E700[(s16)a].flags; E(a).flags = t | (X); }'.replace('X',x)
sh='v << 8'
cases={}
cases['c1']=('s32 c;','c = E(a).child; if (c != -1) model_data_load(c, 3, v);','c = E(a).child; if (c != -1) model_data_load(c, 3, v); a = E(a).sibling;')
cases['c2']=('s16 c;','c = E(a).child; if (c != -1) model_data_load(c, 3, v);','c = E(a).child; if (c != -1) model_data_load(c, 3, v); a = E(a).sibling;')
cases['c3']=('','if (E(a).child != -1) model_data_load(E(a).child, 3, v);','if (E(a).child != -1) model_data_load(E(a).child, 3, v); a = E(a).sibling;')
cases['c4']=('Ent *e;','e = &E(a); if (e->child != -1) model_data_load(e->child, 3, v);','if (e->child != -1) model_data_load(e->child, 3, v); a = e->sibling;')
res=[]
for ck,(decl,t2,t3) in cases.items():
  for at in ['s32','s16']:
    for F in ['-O2','-O3']:
        b=hdr+'''void model_data_load(%s a, s32 mode, s32 v) {
    %s
    if (mode == 2) {
        %s
        %s
    } else if (mode == 0) {
        %s
    } else if (mode == 1) {
        %s
    } else if (mode == 3) {
        do {
            %s
            %s
        } while (a != -1);
    }
}
'''%(at,decl,S(sh),t2,S('0x80000000'),S(sh),S(sh),t3)
        open('t_i.c','w').write(b)
        r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_i.c','model_data_load','--flags','-g0 %s -mips2 -G 0 -non_shared'%F],capture_output=True,text=True,cwd='../../..')
        tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
        m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
        if m_: res.append((int(m_.group(2)),int(m_.group(3)),m_.group(1),ck,at,F))
        else: print(tt)
res.sort(reverse=True)
for x in res[:10]: print(x)
