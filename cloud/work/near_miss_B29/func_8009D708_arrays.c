/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32; typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
extern u8 D_80150B70[];
void func_8009D708(s32 arg0, f32 *arg1, s32 *arg2, f32 arg3, s32 arg4) {
    f32 temp_f0;
    f32 temp_f16;
    f32 var_f12;
    f32 var_f14;
    f32 var_f16;
    f32 var_f2;
    s32 temp_f10;
    s32 temp_f10_2;
    s32 temp_f10_3;
    s32 temp_f4;
    s32 temp_f4_2;
    s32 temp_f4_3;
    s32 temp_f4_4;
    s32 temp_f6;
    s32 temp_f6_2;
    s32 temp_f6_3;
    s32 temp_f6_4;
    s32 temp_f8;
    s32 temp_f8_2;
    void *temp_v0;

    if (arg4 != 0) {
        var_f2 = arg1[9];
        var_f12 = arg1[10];
        var_f14 = arg1[11];
    } else {
        temp_v0 = (s32 *) ((arg0 * 0x98) + (u8 *) &D_80150B70);
        var_f2 = arg1[9] - M2C_FIELD(temp_v0, f32 *, 0x24);
        var_f12 = arg1[10] - M2C_FIELD(temp_v0, f32 *, 0x28);
        var_f14 = arg1[11] - M2C_FIELD(temp_v0, f32 *, 0x2C);
    }
    temp_f0 = sqrtf((var_f2 * var_f2) + (var_f12 * var_f12) + (var_f14 * var_f14));
    if (temp_f0 > 1600.0f) {
        var_f16 = 1600.0f / temp_f0;
        var_f2 *= var_f16;
        var_f12 *= var_f16;
        var_f14 *= var_f16;
    } else {
        var_f16 = 1.0f;
    }
    temp_f6 = (s32) (var_f2 * 1048576.0f);
    temp_f10 = (s32) (var_f12 * 1048576.0f);
    temp_f6_2 = (s32) (var_f14 * 1048576.0f);
    temp_f4 = (s32) (arg3 * 65536.0f);
    temp_f16 = var_f16 * 65536.0f;
    arg2[6] = (s32) ((temp_f6 & 0xFFFF0000) | ((u32) temp_f10 >> 0x10));
    arg2[14] = (s32) ((temp_f6 << 0x10) | (temp_f10 & 0xFFFF));
    arg2[7] = (s32) ((temp_f6_2 & 0xFFFF0000) | ((u32) temp_f4 >> 0x10));
    arg2[15] = (s32) ((temp_f6_2 << 0x10) | (temp_f4 & 0xFFFF));
    temp_f4_2 = (s32) (arg1[0] * temp_f16);
    temp_f10_2 = (s32) (arg1[1] * temp_f16);
    arg2[0] = (s32) ((temp_f4_2 & 0xFFFF0000) | ((u32) temp_f10_2 >> 0x10));
    arg2[8] = (s32) ((temp_f4_2 << 0x10) | (temp_f10_2 & 0xFFFF));
    temp_f8 = (s32) (arg1[2] * temp_f16);
    arg2[1] = (s32) (temp_f8 & 0xFFFF0000);
    arg2[9] = (s32) (temp_f8 << 0x10);
    temp_f6_3 = (s32) (arg1[3] * temp_f16);
    temp_f4_3 = (s32) (arg1[4] * temp_f16);
    arg2[2] = (s32) ((temp_f6_3 & 0xFFFF0000) | ((u32) temp_f4_3 >> 0x10));
    arg2[10] = (s32) ((temp_f6_3 << 0x10) | (temp_f4_3 & 0xFFFF));
    temp_f10_3 = (s32) (arg1[5] * temp_f16);
    arg2[3] = (s32) (temp_f10_3 & 0xFFFF0000);
    arg2[11] = (s32) (temp_f10_3 << 0x10);
    temp_f8_2 = (s32) (arg1[6] * temp_f16);
    temp_f6_4 = (s32) (arg1[7] * temp_f16);
    arg2[4] = (s32) ((temp_f8_2 & 0xFFFF0000) | ((u32) temp_f6_4 >> 0x10));
    arg2[12] = (s32) ((temp_f8_2 << 0x10) | (temp_f6_4 & 0xFFFF));
    temp_f4_4 = (s32) (arg1[8] * temp_f16);
    arg2[5] = (s32) (temp_f4_4 & 0xFFFF0000);
    arg2[13] = (s32) (temp_f4_4 << 0x10);
}
