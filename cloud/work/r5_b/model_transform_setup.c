/* flags: -g0 -O2 -mips2 -G 0 -non_shared ; NOT a match: 88/101 differ (shape 98%) */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
#define E(a) D_8012E700[a]
void model_transform_setup(s32 a, s32 mode, s32 v) {
    if (mode == 1) {
        { u32 t = D_8012E700[(s16)a].flags; E(a).flags = t & ~((v << 8) | 0x80000000); }
        if (E(a).child != -1) model_transform_setup(E(a).child, 2, v);
    } else if (mode == 0) {
        { u32 t = D_8012E700[(s16)a].flags; E(a).flags = t & ~((v << 8) | 0x80000000); }
    } else if (mode == 0) {
        { u32 t = D_8012E700[(s16)a].flags; E(a).flags = (t & 0x7FFFFFFF) | (v << 8); }
    } else if (mode == 2) {
        u32 m = ~((v << 8) | 0x80000000);
        do {
            { u32 t = D_8012E700[(s16)a].flags; E(a).flags = t & m; }
            if (E(a).child != -1) model_transform_setup(E(a).child, 2, v);
            a = E(a).sibling;
        } while (a != -1);
    }
}
