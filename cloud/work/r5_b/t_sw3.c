typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
#define E(a) D_8012E700[a]
#define R(a) D_8012E700[(s16)(a)]
void model_data_load(s32 a, s32 mode, s32 v) {
    while (1) {
        if (mode == 2) {
            { u32 t = R(a).flags; E(a).flags = t | (v << 8); }
            a = E(a).child;
            if (a == -1) return;
            mode = 3;
        } else if (mode == 0) {
            { u32 t = R(a).flags; E(a).flags = t | (0x80000000); }
            return;
        } else if (mode == 1) {
            { u32 t = R(a).flags; E(a).flags = t | (v << 8); }
            return;
        } else if (mode == 3) {
            { u32 t = R(a).flags; E(a).flags = t | (v << 8); }
            if (E(a).child != -1) model_data_load(E(a).child, 3, v);
            a = E(a).sibling;
            if (a == -1) return;
        } else return;
    }
}
