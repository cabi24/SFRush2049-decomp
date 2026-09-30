void func_800B9B64(s32 *arg0, void *arg3, s32 *ipa_t0, s32 ipa_t1, s32 ipa_t2) {
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 var_f0;
    s32 temp_a2_2;
    s32 var_a1_3;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1_2;
    u16 var_t4;
    u16 var_v1;
    u8 var_a1;
    u8 var_a1_2;
    void *temp_a2;
    void *temp_t5;
    void *var_t3;

    var_f0 = D_80123DF8;
    *arg0 = -1;
    var_v1 = D_801407F0;
    var_v0 = 0;
    var_a1 = 0;
    if ((s32) var_v1 <= 0) {

    } else {
        var_a1_2 = (u8) D_801407F0;
        do {
            temp_a2 = D_801407F4 + var_a1_2;
            temp_f2 = (f32) (M2C_FIELD(temp_a2, s16 *, 0) - M2C_FIELD(arg3, s16 *, 0));
            temp_f12 = (f32) (M2C_FIELD(temp_a2, s16 *, 2) - M2C_FIELD(arg3, s16 *, 2));
            temp_f14 = (f32) (M2C_FIELD(temp_a2, s16 *, 4) - M2C_FIELD(arg3, s16 *, 4));
            temp_f16 = (temp_f2 * temp_f2) + (temp_f12 * temp_f12) + (temp_f14 * temp_f14);
            if (temp_f16 < var_f0) {
                *ipa_t0 = var_v0;
                var_f0 = temp_f16;
                var_v1 = (u16) D_801407F4;
            }
            var_v0 += 1;
            var_a1_2 += 6;
        } while (var_v0 < (s32) var_v1);
        var_a1 = D_801407F8;
    }
    var_v1_2 = 0;
    if ((s32) var_a1 > 0) {
        do {
            if ((var_v1_2 != ipa_t1) && ((temp_a2_2 = var_v1_2 * 0x10, var_t3 = D_801407FC + temp_a2_2, (ipa_t2 == 0)) || (var_v1_2 >= ipa_t1) || (ipa_t1 != M2C_FIELD(var_t3, s8 *, 1)) || (M2C_FIELD(var_t3, u16 *, 2) != 0)) && ((ipa_t2 != 0) || (var_v1_2 >= ipa_t1) || (ipa_t1 != M2C_FIELD(var_t3, s8 *, 4)) || ((M2C_FIELD(var_t3, u16 *, 6) + 1) != M2C_FIELD((D_801407FC + (ipa_t1 * 0x10)), u16 *, 0xA)))) {
                var_t4 = M2C_FIELD(var_t3, u16 *, 0xA);
                var_v0_2 = 0;
                if ((s32) var_t4 > 0) {
                    var_a1_3 = 0;
                    do {
                        temp_t5 = M2C_FIELD(var_t3, s32 *, 0xC) + var_a1_3;
                        temp_f2_2 = (f32) (M2C_FIELD(temp_t5, s16 *, 0) - M2C_FIELD(arg3, s16 *, 0));
                        temp_f12_2 = (f32) (M2C_FIELD(temp_t5, s16 *, 2) - M2C_FIELD(arg3, s16 *, 2));
                        temp_f14_2 = (f32) (M2C_FIELD(temp_t5, s16 *, 4) - M2C_FIELD(arg3, s16 *, 4));
                        temp_f16_2 = (temp_f2_2 * temp_f2_2) + (temp_f12_2 * temp_f12_2) + (temp_f14_2 * temp_f14_2);
                        if (temp_f16_2 < var_f0) {
                            *ipa_t0 = var_v0_2;
                            *arg0 = var_v1_2;
                            var_f0 = temp_f16_2;
                            var_t3 = D_801407FC + temp_a2_2;
                            var_t4 = M2C_FIELD(var_t3, u16 *, 0xA);
                        }
                        var_v0_2 += 1;
                        var_a1_3 += 6;
                    } while (var_v0_2 < (s32) var_t4);
                    var_a1 = D_801407F8;
                }
            }
            var_v1_2 += 1;
        } while (var_v1_2 < (s32) var_a1);
    }
}