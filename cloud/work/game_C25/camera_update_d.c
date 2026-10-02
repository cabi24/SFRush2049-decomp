/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "types.h"
#define M2C_FIELD(p,t,o) (*(t)((u8*)(p)+(o)))
extern f32 D_80123AEC;
extern f32 D_80123AF0;
extern f32 D_80123AF4;
extern f32 D_80123AF8;
extern f32 fabsf(f32),sqrtf(f32);
#pragma intrinsic (fabsf)
#pragma intrinsic (sqrtf)
void camera_update_d(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9) {
    f32 sp50;
    f32 sp4C;
    f32 sp48;
    f32 sp28;
    f32 sp24;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f14_3;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f18;
    f32 temp_f18_2;
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f22;
    f32 temp_f22_2;
    f32 temp_f24;
    f32 temp_f24_2;
    f32 temp_f2;
    f32 temp_f6;
    f32 temp_f8;
    f32 var_f0;
    f32 var_f0_2;
    f32 var_f0_3;
    f32 var_f0_4;
    f32 var_f0_5;
    f32 var_f0_6;
    f32 var_f16;

    temp_f22 = arg4 - arg1;
    temp_f24 = arg5 - arg2;
    temp_f20 = arg6 - arg3;
    var_f16 = (temp_f22 * temp_f22) + (temp_f24 * temp_f24) + (temp_f20 * temp_f20);
    if (var_f16 < D_80123AEC) {
        var_f16 = D_80123AF0;
    }
    temp_f2 = -(1.0f / sqrtf(var_f16));
    temp_f22_2 = temp_f22 * temp_f2;
    temp_f24_2 = temp_f24 * temp_f2;
    temp_f20_2 = temp_f20 * temp_f2;
    temp_f12 = (arg8 * temp_f20_2) - (arg9 * temp_f24_2);
    sp50 = temp_f12;
    temp_f14 = (arg9 * temp_f22_2) - (arg7 * temp_f20_2);
    sp4C = temp_f14;
    temp_f16 = (arg7 * temp_f24_2) - (arg8 * temp_f22_2);
    sp48 = temp_f16;
    temp_f18 = (temp_f12 * temp_f12) + (temp_f14 * temp_f14) + (temp_f16 * temp_f16);
    sp28 = temp_f18;
    if (temp_f18 < D_80123AF4) {
        sp28 = D_80123AF8;
    }
    temp_f0 = 1.0f / sqrtf(sp28);
    temp_f16_2 = sp50 * temp_f0;
    temp_f18_2 = sp4C * temp_f0;
    temp_f14_2 = sp48 * temp_f0;
    sp24 = temp_f16_2 * 128.0f;
    if (sp24 < 127.0f) {
        var_f0 = sp24;
    } else {
        var_f0 = 127.0f;
    }
    M2C_FIELD(arg0, s8 *, 8) = (s8) (s32) var_f0;
    sp24 = temp_f18_2 * 128.0f;
    if (sp24 < 127.0f) {
        var_f0_2 = sp24;
    } else {
        var_f0_2 = 127.0f;
    }
    M2C_FIELD(arg0, s8 *, 9) = (s8) (s32) var_f0_2;
    sp24 = temp_f14_2 * 128.0f;
    if (sp24 < 127.0f) {
        var_f0_3 = sp24;
    } else {
        var_f0_3 = 127.0f;
    }
    temp_f6 = ((temp_f24_2 * temp_f14_2) - (temp_f20_2 * temp_f18_2)) * 128.0f;
    M2C_FIELD(arg0, s8 *, 0xA) = (s8) (s32) var_f0_3;
    sp24 = temp_f6;
    if (temp_f6 < 127.0f) {
        var_f0_4 = temp_f6;
    } else {
        var_f0_4 = 127.0f;
    }
    temp_f8 = ((temp_f20_2 * temp_f16_2) - (temp_f22_2 * temp_f14_2)) * 128.0f;
    M2C_FIELD(arg0, s8 *, 0x18) = (s8) (s32) var_f0_4;
    sp24 = temp_f8;
    if (temp_f8 < 127.0f) {
        var_f0_5 = temp_f8;
    } else {
        var_f0_5 = 127.0f;
    }
    temp_f14_3 = ((temp_f22_2 * temp_f18_2) - (temp_f24_2 * temp_f16_2)) * 128.0f;
    M2C_FIELD(arg0, s8 *, 0x19) = (s8) (s32) var_f0_5;
    if (temp_f14_3 < 127.0f) {
        var_f0_6 = temp_f14_3;
    } else {
        var_f0_6 = 127.0f;
    }
    M2C_FIELD(arg0, s8 *, 0) = 0;
    M2C_FIELD(arg0, s8 *, 1) = 0;
    M2C_FIELD(arg0, s8 *, 2) = 0;
    M2C_FIELD(arg0, s8 *, 3) = 0;
    M2C_FIELD(arg0, s8 *, 4) = 0;
    M2C_FIELD(arg0, s8 *, 5) = 0;
    M2C_FIELD(arg0, s8 *, 6) = 0;
    M2C_FIELD(arg0, s8 *, 7) = 0;
    M2C_FIELD(arg0, s8 *, 0x10) = 0;
    M2C_FIELD(arg0, s8 *, 0x11) = 0x80;
    M2C_FIELD(arg0, s8 *, 0x12) = 0;
    M2C_FIELD(arg0, s8 *, 0x13) = 0;
    M2C_FIELD(arg0, s8 *, 0x14) = 0;
    M2C_FIELD(arg0, s8 *, 0x15) = 0x80;
    M2C_FIELD(arg0, s8 *, 0x1A) = (s8) (s32) var_f0_6;
}
