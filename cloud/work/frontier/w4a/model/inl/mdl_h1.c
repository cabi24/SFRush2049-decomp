/* flags: -g0 -O2 -mips2 -G 0 -non_shared ; NOT a match: 3/93 words differ */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];

static u32 ent_flags(s16 i) {
    return D_8012E700[i].flags;
}

void model_data_load(s32 a, s32 mode, u32 v) {
    s32 t;
    s32 c;

    if (mode == 2) {
        t = ent_flags(a); D_8012E700[a].flags = t | (v << 8);
        t = D_8012E700[a].child;
        if (t != -1) model_data_load(t, 3, v);
    } else if (mode == 0) {
        t = ent_flags(a); D_8012E700[a].flags = t | 0x80000000;
    } else if (mode == 1) {
        t = ent_flags(a); D_8012E700[a].flags = t | (v << 8);
    } else if (mode == 3) {
        do {
            t = ent_flags(a); D_8012E700[a].flags = t | (v << 8);
            c = D_8012E700[a].child;
            if (c != -1) model_data_load(c, 3, v);
            a = D_8012E700[a].sibling;
        } while (a != -1);
    }
}
