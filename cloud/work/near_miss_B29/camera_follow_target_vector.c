/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32; typedef float f32;
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
extern void camera_dolly(f32,f32,f32,s32,void*,f32*,f32*);
extern void func_8009E820(f32*,s32,s32);
extern f32 D_80110F80[],D_80111130[],D_80123E10,D_80123E14,D_80123E18,D_80123E1C,D_80123E20,D_80123E24;
extern s32 D_80142DB0;
void camera_follow_target(void *arg0, void *arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10, f32 arg11, s32 arg12) {
    f32 sp44;
    f32 sp40;
    f32 vec[3];
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f10;
    f32 temp_f8;
    f32 var_f0;
    f32 var_f14;
    f32 var_f2;
    f32 var_f2_2;

    if ((arg6 > 0.0f) && (arg7 > 0.0f)) {
        var_f14 = (arg6 - arg7) * arg9;
    } else {
        var_f14 = 0.0f;
    }
    temp_f0 = M2C_FIELD(arg1, f32 *, 4);
    var_f2 = arg11;
    if (temp_f0 < 0.0f) {
        var_f2 = arg10;
    }
    if (arg6 > 10.0f) {
        if (temp_f0 < 1.0f) {
            vec[1] = (1.0f - temp_f0) * M2C_FIELD(arg0, f32 *, 0x5C0) * -0.25f * M2C_FIELD(arg0, f32 *, 0x638);
        } else {
            vec[1] = (var_f14 + (arg6 * arg8)) - (temp_f0 * var_f2);
        }
    } else if (arg6 > 0.0f) {
        vec[1] = (var_f14 + (arg6 * arg8)) - (temp_f0 * var_f2);
    } else {
        vec[1] = 0.0f;
    }
    temp_f0_2 = M2C_FIELD(arg0, f32 *, 0x638);
    var_f2_2 = vec[1] * (temp_f0_2 * ((temp_f0_2 * D_80123E10) - D_80123E14));
    vec[1] = var_f2_2;
    if (var_f2_2 < 0.0f) {
        var_f2_2 = 0.0f;
        vec[1] = 0.0f;
    } else {
        temp_f0_3 = M2C_FIELD(arg0, f32 *, 0x5BC);
        if (temp_f0_3 < vec[1]) {
            var_f2_2 = temp_f0_3;
        }
    }
    camera_dolly(0.0f, var_f14, var_f2_2, arg4, arg3, &sp44, &sp40);
    if ((((u8 *) arg0 + 0x4E8) == (void *) arg3) || (((u8 *) arg0 + 0x544) == (void *) arg3)) {
        temp_f8 = sp40 * *((f32 *) ((u8 *) &D_80111130 + ((M2C_FIELD(arg0, s8 *, 0xC) * 0xC) + (M2C_FIELD(arg0, s8 *, 0xB) * 4))));
        sp40 = temp_f8;
        temp_f10 = temp_f8 * (1.0f + ((*((f32 *) ((u8 *) &D_80110F80 + ((M2C_FIELD(arg0, s8 *, 9) * 0x18) + (M2C_FIELD(arg0, s16 *, 0x3F4) * 4)))) - 1.0f) * (1.0f - fabsf(M2C_FIELD(arg0, f32 *, 0x720)))));
        sp40 = temp_f10;
        sp40 = temp_f10 * M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 0x18);
        sp44 *= M2C_FIELD(arg0, f32 *, 0x5B0) * (1.0f - (M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 0x1C) * D_80123E18));
        if (arg12 != 0) {
            if (D_80142DB0 == 2) {
                var_f0 = D_80123E1C;
            } else {
                var_f0 = (M2C_FIELD(arg0, f32 *, 0x5B4) * D_80123E20) + D_80123E24;
            }
            sp44 *= var_f0;
        }
    }
    vec[2] = sp40;
    vec[0] = sp44;
    func_8009E820(vec, arg5, arg2);
    M2C_FIELD(arg3, f32 *, 0x50) = sp44;
    M2C_FIELD(arg3, f32 *, 0x54) = sp40;
}
