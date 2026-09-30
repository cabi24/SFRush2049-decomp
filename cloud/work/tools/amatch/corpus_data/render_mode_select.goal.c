/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u8 pad0[22]; s16 s22; s16 s24; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];
extern s16 D_8015B254;
s32 func_8008E144(s32);
void render_mode_select(s16 a, s16 b) {
    if (b < 0) {
        if (D_8015B254 < 0) {
            D_8015B254 = a;
        } else {
            D_8012E700[func_8008E144(D_8015B254)].s24 = a;
        }
    } else {
        if (D_8012E700[b].s22 == -1) {
            D_8012E700[b].s22 = a;
        } else {
            D_8012E700[func_8008E144(D_8012E700[b].s22)].s24 = a;
        }
    }
}
