import subprocess,itertools,re,sys
hdr='''typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
#define E(a) D_8012E700[a]
#define R(a) D_8012E700[(s16)(a)]
'''
stmts={
 'orig':'E(a).flags = R(a).flags | (X);',
 'tmp':'{ u32 t = R(a).flags; E(a).flags = t | (X); }',
 'swap':'E(a).flags = (X) | R(a).flags;',
 'oreq':'E(a).flags |= (X);',
 'oreqR':'E(a).flags = R(a).flags | (X);',
}
def body(st, loopf, vexpr):
    S=lambda x: st.replace('X',x)
    sh=vexpr
    if loopf=='goto':
        return '''void model_data_load(s32 a, s32 mode, s32 v) {
  top:
    if (mode == 2) {
        %s
        a = E(a).child;
        if (a == -1) return;
        mode = 3;
        goto top;
    }
    if (mode == 0) {
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
'''%(S(sh),S('0x80000000'),S(sh),S(sh))
    else:
        return '''void model_data_load(s32 a, s32 mode, s32 v) {
    while (1) {
        if (mode == 2) {
            %s
            a = E(a).child;
            if (a == -1) return;
            mode = 3;
        } else break;
    }
    if (mode == 0) {
        %s
    } else if (mode == 1) {
        %s
    } else if (mode == 3) {
        while (1) {
            %s
            if (E(a).child != -1) model_data_load(E(a).child, 3, v);
            a = E(a).sibling;
            if (a == -1) break;
        }
    }
}
'''%(S(sh),S('0x80000000'),S(sh),S(sh))
if __name__=='__main__':
    res=[]
    for sk,st in stmts.items():
      for lf in ['goto','while']:
        for ve in ['v << 8','(u32)v << 8']:
          for F in ['-O2','-O3','-O1']:
            open('t_f.c','w').write(hdr+body(st,lf,ve))
            r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_f.c','model_data_load','--flags','-g0 %s -mips2 -G 0 -non_shared'%F],capture_output=True,text=True,cwd='../../..')
            tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
            m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
            if m_: res.append((int(m_.group(2)),int(m_.group(3)),m_.group(1),sk,lf,ve,F))
            else: print(tt[:200])
    res.sort(reverse=True)
    for x in res[:10]: print(x)
