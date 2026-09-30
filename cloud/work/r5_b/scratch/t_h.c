typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
#define E(a) D_8012E700[a]
void model_data_load(u32 a, s32 mode, s32 v) {
    if (mode == 2) {
        E(a).flags = ((u32)v << 8) | D_8012E700[a].flags;
        if (E(a).child != -1) model_data_load(E(a).child, 3, v);
    } else if (mode == 0) {
        E(a).flags = (0x80000000) | D_8012E700[a].flags;
    } else if (mode == 1) {
        E(a).flags = ((u32)v << 8) | D_8012E700[a].flags;
    } else if (mode == 3) {
        do {
            E(a).flags = ((u32)v << 8) | D_8012E700[a].flags;
            if (E(a).child != -1) model_data_load(E(a).child, 3, v);
            a = E(a).sibling;
        } while (a != -1);
    }
}
