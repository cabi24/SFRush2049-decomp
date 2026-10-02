/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "types.h"
#define M2C_FIELD(p,t,o) (*(t)((u8*)(p)+(o)))
extern f32 D_80114184;
extern f32 D_80114188;
extern f32 D_801243B4;
extern f32 D_801243B8;
extern f32 D_801243BC;
extern s8 D_80152B71;
extern f32 fabsf(f32),sqrtf(f32);
#pragma intrinsic (fabsf)
#pragma intrinsic (sqrtf)
extern f32 func_8008B3C8(void*);
#define NULL ((void*)0)
#define M2C_BITWISE(t,x) (x)
void func_800E114C(void *arg0) {
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 var_f12;
    f32 var_f18;

    sp1C = M2C_FIELD(arg0, f32 *, 0x28) * M2C_FIELD(arg0, f32 *, 0x634);
    sp20 = M2C_FIELD(arg0, f32 *, 0x2C) * M2C_FIELD(arg0, f32 *, 0x634);
    sp24 = M2C_FIELD(arg0, f32 *, 0x30) * M2C_FIELD(arg0, f32 *, 0x634);
    M2C_FIELD(arg0, f32 *, 0x40) = (f32) (sp1C + M2C_FIELD(arg0, f32 *, 0x40));
    M2C_FIELD(arg0, f32 *, 0x44) = (f32) (sp20 + M2C_FIELD(arg0, f32 *, 0x44));
    M2C_FIELD(arg0, f32 *, 0x48) = (f32) (sp24 + M2C_FIELD(arg0, f32 *, 0x48));
    if (M2C_FIELD(arg0, f32 *, 0x3D0) < D_801243B4) {
        if (fabsf(M2C_FIELD(arg0, f32 *, 0x40)) < D_801243B8) {
            M2C_FIELD(arg0, f32 *, 0x40) = 0.0f;
        }
        if (fabsf(M2C_FIELD(arg0, f32 *, 0x48)) < D_801243B8) {
            M2C_FIELD(arg0, f32 *, 0x48) = 0.0f;
        }
    }
    if (*(&D_80152B71 + (M2C_FIELD(arg0, s16 *, 0x7C6) * 0x3B8)) != 2) {
        M2C_FIELD(arg0, f32 *, 0x3F0) = M2C_BITWISE(f32, func_8008B3C8((u8 *) arg0 + 0x40));
    }
    var_f12 = 300.0f;
    temp_f2 = M2C_FIELD(arg0, f32 *, 0x3F0);
    if (temp_f2 > 300.0f) {
        temp_f0 = 300.0f / temp_f2;
        M2C_FIELD(arg0, f32 *, 0x40) = (f32) (M2C_FIELD(arg0, f32 *, 0x40) * temp_f0);
        M2C_FIELD(arg0, f32 *, 0x44) = (f32) (M2C_FIELD(arg0, f32 *, 0x44) * temp_f0);
        M2C_FIELD(arg0, f32 *, 0x48) = (f32) (M2C_FIELD(arg0, f32 *, 0x48) * temp_f0);
        return;
    }
    sp1C = M2C_FIELD(arg0, f32 *, 0x34) * M2C_FIELD(arg0, f32 *, 0x634);
    sp20 = M2C_FIELD(arg0, f32 *, 0x38) * M2C_FIELD(arg0, f32 *, 0x634);
    sp24 = M2C_FIELD(arg0, f32 *, 0x3C) * M2C_FIELD(arg0, f32 *, 0x634);
    if (M2C_FIELD(arg0, f32 *, 0x650) != 0.0f) {
        sp1C -= M2C_FIELD(arg0, f32 *, 0x4C) * D_801243BC;
        sp20 -= M2C_FIELD(arg0, f32 *, 0x50) * D_801243BC;
        sp24 -= M2C_FIELD(arg0, f32 *, 0x54) * D_801243BC;
    }
    M2C_FIELD(arg0, f32 *, 0x4C) = (f32) (sp1C + M2C_FIELD(arg0, f32 *, 0x4C));
    M2C_FIELD(arg0, f32 *, 0x50) = (f32) (sp20 + M2C_FIELD(arg0, f32 *, 0x50));
    M2C_FIELD(arg0, f32 *, 0x54) = (f32) (sp24 + M2C_FIELD(arg0, f32 *, 0x54));
    if (((M2C_FIELD(arg0, f32 *, 0x5EC) > 0.0f) && (M2C_FIELD(arg0, f32 *, 0x5F0) > 0.0f) && (M2C_FIELD(arg0, f32 *, 0x5F4) > 0.0f) && (M2C_FIELD(arg0, f32 *, 0x5F8) > 0.0f)) || (M2C_FIELD(arg0, s8 *, 0x640) != 0)) {
        temp_f0_2 = M2C_BITWISE(f32, func_8008B3C8((u8 *) arg0 + 0x4C));
        temp_f16 = temp_f0_2 * temp_f0_2;
        if (M2C_FIELD(arg0, s8 *, 0x640) != 0) {
            var_f18 = D_80114188;
        } else {
            var_f18 = D_80114184;
        }
        temp_f14 = temp_f16 * var_f18;
        temp_f0_3 = M2C_FIELD(arg0, f32 *, 0x4C);
        temp_f2_2 = M2C_FIELD(arg0, f32 *, 0x50);
        var_f12 = M2C_FIELD(arg0, f32 *, 0x54);
        M2C_FIELD(arg0, f32 *, 0x4C) = (f32) (temp_f0_3 - (temp_f0_3 * temp_f14));
        M2C_FIELD(arg0, f32 *, 0x50) = (f32) (temp_f2_2 - (temp_f2_2 * temp_f14));
        M2C_FIELD(arg0, f32 *, 0x54) = (f32) (var_f12 - (var_f12 * temp_f14));
    }
    if ((M2C_FIELD(arg0, f32 *, 0x3F0) < 3.0f) && ((M2C_FIELD(arg0, f32 *, 0x3D4) > 0.5f) || (M2C_FIELD(arg0, f32 *, 0x2FC) < 0.0f))) {
        M2C_FIELD(arg0, f32 *, 0x40) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x48) = 0.0f;
    }
    if ((M2C_FIELD(arg0, f32 *, 0x3F0) < 3.0f) && (M2C_FIELD(arg0, f32 *, 0x2FC) < -0.75f) && (M2C_BITWISE(f32, func_8008B3C8((u8 *) arg0 + 0x4C)) < 0.5f)) {
        M2C_FIELD(arg0, f32 *, 0x4C) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x50) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x54) = 0.0f;
    }
}
