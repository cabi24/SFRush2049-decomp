import subprocess,itertools,re,sys
hdr='''typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
#define E(a) D_8012E700[a]
#define R(a) D_8012E700[(s16)(a)]
'''
def mk(name, m2op, m0op, m1op, m3op):
    return hdr+'''void %s(s32 a, s32 mode, s32 v) {
  top:
    if (mode == 2) {
        E(a).flags = R(a).flags | %s;
        a = E(a).child;
        if (a == -1) return;
        mode = 3;
        goto top;
    }
    if (mode == 0) {
        E(a).flags = R(a).flags | %s;
    } else if (mode == 1) {
        E(a).flags = R(a).flags | %s;
    } else if (mode == 3) {
        do {
            E(a).flags = R(a).flags | %s;
            if (E(a).child != -1) %s(E(a).child, 3, v);
            a = E(a).sibling;
        } while (a != -1);
    }
}
'''%(name,m2op,m0op,m1op,m3op,name)
if __name__=='__main__':
    s=mk('model_data_load','(v << 8)','0x80000000','(v << 8)','(v << 8)')
    open('model_data_load.c','w').write(s)
