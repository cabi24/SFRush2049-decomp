/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    s32 w4;
    s8 b8;
    s8 b9;
    s8 b10;
    u8 pad11;
    s32 w12;
    f32 f16;
    f32 f20;
} Rec24;
extern Rec24 D_803BA2F0[];
extern u8 D_8013FE90[];

void func_80391470(void)
{
    s32 i;

    for (i = 0; i < 52; i++) {
        D_803BA2F0[i].b0 = 0;
        D_803BA2F0[i].b1 = 1;
        D_803BA2F0[i].b2 = 0;
        D_803BA2F0[i].w4 = -1;
        D_803BA2F0[i].b3 = 0;
        D_803BA2F0[i].b8 = -1;
        D_803BA2F0[i].b9 = -1;
        D_803BA2F0[i].b10 = -1;
        D_803BA2F0[i].w12 = -1;
        D_803BA2F0[i].f16 = -1.0f;
        D_803BA2F0[i].f20 = -1.0f;
        D_8013FE90[i] = 0;
    }
}
