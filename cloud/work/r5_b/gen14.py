import subprocess,re,itertools
hdr='''typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
#define E(a) D_8012E700[a]
'''
def gen(at, rd, st, sh, tailform):
    R=lambda: 'D_8012E700[%s].flags'%rd
    def S(x):
        return st.replace('X',x).replace('RD',R())
    child_tail = 'if (E(a).child != -1) model_data_load(E(a).child, 3, v);'
    return hdr+'''void model_data_load(%s a, s32 mode, s32 v) {
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
            if (E(a).child != -1) model_data_load(E(a).child, 3, v);
            a = E(a).sibling;
        } while (a != -1);
    }
}
'''%(at,S(sh),child_tail,S('0x80000000'),S(sh),S(sh))
sts={'A':'{ u32 t = RD; E(a).flags = t | (X); }','B':'{ u32 t = RD; E(a).flags = (X) | t; }','C':'E(a).flags = RD | (X);','D':'E(a).flags = (X) | RD;'}
rds=['(s16)a','(s16)(a)+0']
res=[]
for at,(sk,st),rd,sh,F in itertools.product(['s32','s16','u16','u32'],sts.items(),['(s16)a','a'],['v << 8','(u32)v << 8'],['-O2','-O3']):
    open('t_h.c','w').write(gen(at,rd,st,sh,0))
    r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_h.c','model_data_load','--flags','-g0 %s -mips2 -G 0 -non_shared'%F],capture_output=True,text=True,cwd='../../..')
    tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
    m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
    if m_: res.append((int(m_.group(2)),int(m_.group(3)),m_.group(1),at,sk,rd,sh,F))
res.sort(reverse=True)
for x in res[:10]: print(x)
