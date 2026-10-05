typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];

void model_transform_setup(s32 a, s32 mode, u32 v) {
    s32 t;
    s32 c;

    if (mode == 1) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t & ~((v << 8) | 0x80000000);
        t = D_8012E700[a].child;
        if (t != -1) model_transform_setup(t, 2, v);
    } else if (mode == 0) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t & ~((v << 8) | 0x80000000);
    } else if (mode == 0) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = (t & 0x7FFFFFFF) | (v << 8);
    } else if (mode == 2) {
        do {
            t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t & ~((v << 8) | 0x80000000);
            c = D_8012E700[a].child;
            if (c != -1) model_transform_setup(c, 2, v);
            a = D_8012E700[a].sibling;
        } while (a != -1);
    }
}
