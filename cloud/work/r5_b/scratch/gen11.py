import subprocess,itertools,re
hdr='''typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
'''
def build(ext, a_t, loopform, vol):
    E='Ent' if not vol else 'Ent'
    f='flags'
    s=hdr+ext+'\n'
    s+='void model_data_load(%s a, s32 mode, s32 v) {\n'%a_t
    if loopform=='goto':
        s+='''  top:
    if (mode == 2) {
        D_8012E700[a].flags |= v << 8;
        a = D_8012E700[a].child;
        if (a == -1) return;
        mode = 3;
        goto top;
    }
    if (mode == 0) {
        D_8012E700[a].flags |= 0x80000000;
    } else if (mode == 1) {
        D_8012E700[a].flags |= v << 8;
    } else if (mode == 3) {
        do {
            D_8012E700[a].flags |= v << 8;
            if (D_8012E700[a].child != -1) model_data_load(D_8012E700[a].child, 3, v);
            a = D_8012E700[a].sibling;
        } while (a != -1);
    }
}'''
    elif loopform=='while':
        s+='''    while (1) {
    if (mode == 2) {
        D_8012E700[a].flags |= v << 8;
        a = D_8012E700[a].child;
        if (a == -1) return;
        mode = 3;
    } else break;
    }
    if (mode == 0) {
        D_8012E700[a].flags |= 0x80000000;
    } else if (mode == 1) {
        D_8012E700[a].flags |= v << 8;
    } else if (mode == 3) {
        while (1) {
            D_8012E700[a].flags |= v << 8;
            if (D_8012E700[a].child != -1) model_data_load(D_8012E700[a].child, 3, v);
            a = D_8012E700[a].sibling;
            if (a == -1) break;
        }
    }
}'''
    return s
res=[]
for ext in ['extern Ent D_8012E700[];','extern volatile Ent D_8012E700[];']:
 for a_t in ['s16','s32']:
  for lf in ['goto','while']:
   for F in ['-O2','-O3']:
    open('t_d.c','w').write(build(ext,a_t,lf,0))
    r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_d.c','model_data_load','--flags','-g0 %s -mips2 -G 0 -non_shared'%F],capture_output=True,text=True,cwd='../../..')
    tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
    m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
    if m_: res.append((int(m_.group(3)),int(m_.group(2)),m_.group(1),ext[:14],a_t,lf,F))
    else: print(tt[:200])
res.sort(reverse=True)
for x in res[:10]: print(x)
