/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u8 pad0[8];
    f32 unk8;
} TorqueSrc;

typedef struct {
    u8 pad0[4];
    TorqueSrc *unk4;
    u8 pad8[3];
    s8 unkB;
    s8 unkC;
} EngineState;

extern f32 D_801110C4[][3];

#define RPM_SCALE 1150

s16 func_800E2CC0(s32 a, s32 b, s32 rem, s32 total) {
    return (a + (((b - a) * rem) / total));
}

s16 func_800E2D18(EngineState *m, s16 rpm, s16 throttle, s16 *torquecurve)
{
    s16 rindex, tindex, rrem, trem, left, right;
    s16 *low_ptr, *hi_ptr;

    rindex = rpm / (s16)RPM_SCALE;
    rrem = rpm % (s16)RPM_SCALE;
    if (rindex < 0) {
        rindex = 0;
        rrem = 0;
    }
    if (rindex >= 11) {
        rindex = 10;
        rrem = RPM_SCALE - 1;
    }
    tindex = throttle / (s16)14;
    trem = throttle % (s16)14;
    if (tindex >= 9) {
        tindex = 8;
        trem = 13;
    }
    low_ptr = (s16 *)torquecurve + (tindex * 12) + rindex;
    hi_ptr = (s16 *)torquecurve + (tindex * 12) + rindex + 1;
    left = func_800E2CC0((*low_ptr), (*hi_ptr), rrem, RPM_SCALE - 1);
    low_ptr = (s16 *)torquecurve + ((tindex + 1) * 12) + rindex;
    hi_ptr = (s16 *)torquecurve + ((tindex + 1) * 12) + rindex + 1;
    right = func_800E2CC0((*low_ptr), (*hi_ptr), rrem, RPM_SCALE - 1);
    right = func_800E2CC0(left, right, trem, 14);
    return (m->unk4->unk8 * D_801110C4[m->unkC][m->unkB]) * right;
}
