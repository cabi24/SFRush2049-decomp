/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u8 pad[20]; u16 key; u8 pad22[46]; } Ent;
typedef struct { u8 *base; s32 x; } Tab;
extern s32 D_80156990;
extern Ent D_8012E700[];
extern Tab D_801161F4[];
s32 func_8008AD04(void *, void *);
s16 car_gear_shift(void *a) {
    s32 i;
    Ent *e = D_8012E700;
    for (i = 0; i < D_80156990; i++, e++) {
        if (func_8008AD04(a, D_801161F4[e->key >> 10].base + (e->key & 0x3FF) * 88) == 0) {
            return i;
        }
    }
    return -1;
}
