/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { s8 b[19]; } Ent19;
extern s16 D_8014A108;
extern Ent19 D_80150DD8[];
s32 func_800F7644(s32 idx) {
    s32 r = 0;
    s32 i;
    for (i = 0; i < D_8014A108; i++) {
        if (D_80150DD8[i].b[idx]) r = 1;
    }
    return r;
}
