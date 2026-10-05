void steering_sensitivity(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4, f32 arg5) {
    void *sp60;                                     /* compiler-managed */
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 sp44;
    f32 output[3];
    f32 sp30;                                       /* compiler-managed */
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 temp_f18;
    f32 temp_f18_2;
    f32 temp_f18_3;
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f2_5;
    f32 var_f16;
    f32 var_f16_2;
    f32 var_f22;
    void *temp_a0;
    s32 temp_t6;
    void *var_v1;
    void *temp_s0;

    temp_t6 = arg1 & 0xFFFF;
    temp_s0 = (temp_t6 * 0x84) + (u8 *) D_80152034;
    if ((temp_t6 + 1) < (s32) M2C_FIELD(D_801526F0, u16 *, 0)) {
        var_v1 = (u8 *) temp_s0 + 0x84;
    } else {
        var_v1 = (u8 *) D_80152034;
    }
    temp_a0 = (u8 *) temp_s0 + 0xC;

    sp60 = (void *) var_v1;
    math_utility(temp_a0, arg4);
    vector_diff_process((f32 *) temp_s0, (f32 *) temp_a0, (f32 *) arg2, (f32 *) output);
    temp_f0 = M2C_FIELD(temp_s0, f32 *, 0x60);
    sp44 = output[2];
    if (output[2] < 0.0f) {
        sp44 = 0.0f;
    } else if (temp_f0 < output[2]) {
        sp44 = temp_f0;
    }
    temp_f2 = M2C_FIELD(temp_s0, f32 *, 0x58);
    temp_f0_2 = sp44 / temp_f0;
    temp_f12 = M2C_FIELD(temp_s0, f32 *, 0x54);
    var_f16 = output[0] + M2C_FIELD(temp_s0, f32 *, 0x5C);
    temp_f18 = temp_f2 + ((M2C_FIELD(var_v1, f32 *, 0x58) - temp_f2) * temp_f0_2);
    temp_f20 = ((temp_f12 + ((M2C_FIELD(var_v1, f32 *, 0x54) - temp_f12) * temp_f0_2)) - temp_f18) - 2.5f;
    if (M2C_FIELD(arg4, f32 *, 0x10) < 0.0f) {
        sp60 = var_v1;
        sp4C = var_f16;
        sp50 = temp_f18;
        func_800AD090(0.0f, -1.0f, arg4);
        var_f16 = -var_f16;
        var_f22 = (temp_f20 - output[1]) + 5.0f;
    } else {
        var_f22 = output[1] - temp_f20;
    }
    if (var_f16 < -temp_f18) {
        var_f16_2 = var_f16 + temp_f18;
        goto block_17;
    }
    if (temp_f18 < var_f16) {
        var_f16_2 = var_f16 - temp_f18;
block_17:
        temp_f0_3 = sqrtf((var_f16_2 * var_f16_2) + (var_f22 * var_f22));
        sp54 = temp_f0_3;
        if (temp_f0_3 < D_80123BFC) {
            M2C_FIELD(arg3, f32 *, 4) = (f32) (M2C_FIELD(arg3, f32 *, 4) + temp_f20);
            return;
        }
        temp_f2_2 = temp_f20 - temp_f0_3;
        M2C_FIELD(arg3, f32 *, 4) = temp_f2_2;
        if (temp_f2_2 < arg5) {
            temp_f12_2 = M2C_FIELD(var_v1, f32 *, 0x58);
            temp_f2_3 = M2C_FIELD(temp_s0, f32 *, 0x58);
            temp_f18_2 = temp_f12_2 - temp_f2_3;
            temp_f20_2 = ((M2C_FIELD(var_v1, f32 *, 0x54) - temp_f12_2) - M2C_FIELD(temp_s0, f32 *, 0x54)) + temp_f2_3;
            temp_f14 = fabsf(temp_f20_2);
            sp30 = temp_f14;
            if (temp_f14 > 1.0f) {
                temp_f2_4 = M2C_FIELD(temp_s0, f32 *, 0x60);
                sp50 = temp_f18_2;
                sp4C = var_f16_2;
                temp_f0_4 = sqrtf((temp_f20_2 * temp_f20_2) + (temp_f2_4 * temp_f2_4));
                sp44 = temp_f0_4;
                func_800ACFF8(-temp_f20_2 / temp_f0_4, temp_f2_4 / temp_f0_4, arg4);
            }
            sp4C = var_f16_2;
            sp50 = temp_f18_2;
            func_800AD090(-var_f16_2 / sp54, -var_f22 / sp54, arg4);
            if (fabsf(temp_f18_2) > 1.0f) {
                temp_f2_5 = M2C_FIELD(temp_s0, f32 *, 0x60);
                temp_f18_3 = (fabsf(var_f16_2) * temp_f18_2) / sp54;
                temp_f0_5 = sqrtf((temp_f18_3 * temp_f18_3) + (temp_f2_5 * temp_f2_5));
                func_800ACFF8(-temp_f18_3 / temp_f0_5, temp_f2_5 / temp_f0_5, arg4);
            }
            if (sp30 > 1.0f) {
                func_800ACFF8(temp_f20_2 / sp44, M2C_FIELD(temp_s0, f32 *, 0x60) / sp44, arg4);
            }
        }
        return;
    }
    if (var_f22 > 0.0f) {
        M2C_FIELD(arg3, f32 *, 4) = (f32) (temp_f20 - var_f22);
        func_800AD090(0.0f, -1.0f, arg4);
        return;
    }
    M2C_FIELD(arg3, f32 *, 4) = (f32) (temp_f20 + var_f22);
}
