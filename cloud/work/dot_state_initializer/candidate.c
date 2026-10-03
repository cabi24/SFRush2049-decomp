/* Research only: NONMATCH. Ordinary O32 two-pointer initializer. */
/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct { u8 pad0[0x7CC]; s8 f7CC; u8 pad1[0xF]; s8 f7DC; } A0;
typedef struct {
    u8 p0[0xFA]; s16 f0FA; s16 f0FC; s16 f0FE;
    u8 p1[0x314 - 0x100]; float f314; float f318;
    u8 p2[0x334 - 0x31C]; float f334; s16 f338; s16 f33A; s16 f33C; s16 f33E;
} A1;
extern float D_801543CC;
extern s32 D_801161D0;
extern s16 D_80151CEE;
void func_800EC270(A0 *a0, A1 *a1) {
    const s8 enabled = 1;
    const s16 invalid = -1;
    a1->f338 = 0;
    a1->f314 = 0.0f;
    a1->f318 = 0.0f;
    a1->f334 = D_801543CC;
    a1->f33A = invalid;
    if (a0->f7CC == enabled) {
        s32 index = D_801161D0;
        a1->f33C = index;
        a1->f33E = index;
        index = (s32)((unsigned int)index + 1U);
        D_801161D0 = index;
        if (index > 3) D_801161D0 = 0;
    } else {
        a1->f33C = 0;
    }
    if (D_80151CEE == 0) a0->f7DC = enabled;
    a1->f0FA = invalid;
    a1->f0FC = 0;
    a1->f0FE = 0;
}
