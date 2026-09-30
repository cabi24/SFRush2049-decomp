typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
void model_data_load(s16 a, s32 mode, s32 v) {
    if (mode == 2) {
        D_8012E700[a].flags |= v << 8;
        a = D_8012E700[a].child;
        if (a == -1) return;
        mode = 3;
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
}
