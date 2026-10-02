/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef signed short s16;
typedef int s32;
typedef float f32;
extern f32 D_801243D8;
extern f32 D_801243DC;
#define M2C_FIELD(p, type, offset) (*(type)((s8 *)(p) + (offset)))
void func_800E2AC4(void *arg0) {
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f16;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 var_f14;
    s32 var_t1;
    s32 var_v0;

    temp_f2 = M2C_FIELD(arg0, f32 *, 0x42C);
    if (M2C_FIELD(arg0, f32 *, 0x408) < D_801243D8) {
        M2C_FIELD(arg0, f32 *, 0x408) = (f32) D_801243D8;
    }
    temp_f2_2 = M2C_FIELD(arg0, f32 *, 0x3CC);
    if (D_801243DC < temp_f2_2) {
        var_f14 = 0.0f;
    } else {
        var_f14 = (D_801243DC - temp_f2_2) * 1.25f * M2C_FIELD(M2C_FIELD(arg0, void **, 0), f32 *, 0xA8);
    }
    if ((M2C_FIELD(arg0, s16 *, 0x3F4) == 0) || (var_f14 == 0.0f)) {
        M2C_FIELD(arg0, f32 *, 0x420) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x410) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x424) = M2C_FIELD(arg0, f32 *, 0x408);
        M2C_FIELD(arg0, f32 *, 0x418) = (f32) M2C_FIELD(arg0, f32 *, 0x414);
        M2C_FIELD(arg0, f32 *, 0x408) = (f32) (M2C_FIELD(arg0, f32 *, 0x408) + (M2C_FIELD(arg0, f32 *, 0x404) * M2C_FIELD(M2C_FIELD(arg0, void **, 0), f32 *, 0xA0) * M2C_FIELD(arg0, f32 *, 0x634)));
        return;
    }
    var_v0 = 0;
    temp_f14 = var_f14 * M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 0x10);
    temp_f2_3 = M2C_FIELD(arg0, f32 *, 0x41C) * M2C_FIELD(arg0, f32 *, 0x42C);
    M2C_FIELD(arg0, f32 *, 0x424) = temp_f2_3;
    if (temp_f2_3 < M2C_FIELD(arg0, f32 *, 0x408)) {
        var_v0 = 1;
    }
    if (var_v0 != 0) {
        M2C_FIELD(arg0, f32 *, 0x420) = temp_f14;
    } else {
        M2C_FIELD(arg0, f32 *, 0x420) = (f32) -temp_f14;
    }
    temp_f2_4 = M2C_FIELD(arg0, f32 *, 0x404);
    temp_f14_2 = M2C_FIELD(arg0, f32 *, 0x424);
    var_t1 = 0;
    temp_f16 = M2C_FIELD(arg0, f32 *, 0x408) + ((temp_f2_4 - M2C_FIELD(arg0, f32 *, 0x420)) * M2C_FIELD(M2C_FIELD(arg0, void **, 0), f32 *, 0xA0) * M2C_FIELD(arg0, f32 *, 0x634));
    if (temp_f14_2 < temp_f16) {
        var_t1 = 1;
    }
    if (var_t1 != var_v0) {
        M2C_FIELD(arg0, f32 *, 0x408) = temp_f14_2;
        M2C_FIELD(arg0, f32 *, 0x420) = temp_f2_4;
    } else {
        M2C_FIELD(arg0, f32 *, 0x408) = temp_f16;
    }
    M2C_FIELD(arg0, f32 *, 0x410) = (f32) (M2C_FIELD(arg0, f32 *, 0x420) * M2C_FIELD(arg0, f32 *, 0x42C));
    M2C_FIELD(arg0, f32 *, 0x418) = (f32) (1.0f / ((1.0f / M2C_FIELD(arg0, f32 *, 0x414)) + ((temp_f2 * temp_f2) / M2C_FIELD(M2C_FIELD(arg0, void **, 0), f32 *, 0xA0))));
}
