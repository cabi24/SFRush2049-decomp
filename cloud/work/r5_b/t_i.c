typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
#define E(a) D_8012E700[a]
void model_data_load(s16 a, s32 mode, s32 v) {
    Ent *e;
    if (mode == 2) {
        { u32 t = D_8012E700[(s16)a].flags; E(a).flags = t | (v << 8); }
        e = &E(a); if (e->child != -1) model_data_load(e->child, 3, v);
    } else if (mode == 0) {
        { u32 t = D_8012E700[(s16)a].flags; E(a).flags = t | (0x80000000); }
    } else if (mode == 1) {
        { u32 t = D_8012E700[(s16)a].flags; E(a).flags = t | (v << 8); }
    } else if (mode == 3) {
        do {
            { u32 t = D_8012E700[(s16)a].flags; E(a).flags = t | (v << 8); }
            if (e->child != -1) model_data_load(e->child, 3, v); a = e->sibling;
        } while (a != -1);
    }
}
