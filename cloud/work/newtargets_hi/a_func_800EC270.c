typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct { u8 pad0[0x7CC]; s8 f7CC; u8 pad1[0x13]; s8 f7E0; } A0;
typedef struct {
    u8 p0[0xFA]; s16 f0FA; s16 f0FC; s16 f0FE;
    u8 p1[0x314 - 0x100]; float f314; float f318;
    u8 p2[0x334 - 0x31C]; float f334; s16 f338; s16 f33A; s16 f33C; s16 f33E;
} A1;
extern float D_801543CC;
extern s32 D_801161D0;
extern s16 D_80151CEE;
void func_800EC270(A0 *a0, A1 *a1) {
    a1->f338 = 0;
    a1->f314 = 0.0f;
    a1->f318 = 0.0f;
    a1->f334 = D_801543CC;
    a1->f33A = -1;
    if (a0->f7CC == 1) {
        s32 t = D_801161D0;
        a1->f33C = t;
        a1->f33E = t;
        t++;
        D_801161D0 = t;
        if (t >= 4) D_801161D0 = 0;
    } else {
        a1->f33C = 0;
    }
    if (D_80151CEE == 0) a0->f7E0 = 1;
    a1->f0FA = -1;
    a1->f0FC = 0;
    a1->f0FE = 0;
}
