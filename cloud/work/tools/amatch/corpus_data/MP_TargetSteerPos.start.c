s32 MP_TargetSteerPos(void *ipa_s0, f32 ipa_f20, f32 ipa_f22) {
    f32 temp_f4;
    f32 temp_f8;
    s32 temp_v0;
    s32 var_a1;
    s32 var_a1_2;

    temp_v0 = func_80020174(M2C_FIELD(ipa_s0, u16 *, 0xE), 0xFF, 0xFF);
    M2C_FIELD(ipa_s0, s32 *, 0x10) = temp_v0;
    if (temp_v0 == -1) {
        return 0;
    }
    temp_f4 = ipa_f20 * 127.0f;
    if (M2C_ERROR(/* cfc1 */) & 0x78) {
        if (!(M2C_ERROR(/* cfc1 */) & 0x78)) {
            var_a1 = (s32) (temp_f4 - 2.1474836e9f) | 0x80000000;
        } else {
            goto block_5;
        }
    } else {
        var_a1 = (s32) temp_f4;
        if (var_a1 < 0) {
block_5:
            var_a1 = -1;
        }
    }
    func_8001fea4(M2C_FIELD(ipa_s0, s32 *, 0x10), var_a1 & 0xFF);
    temp_f8 = (ipa_f22 + 1.0f) * 0.5f * 127.0f;
    if (M2C_ERROR(/* cfc1 */) & 0x78) {
        if (!(M2C_ERROR(/* cfc1 */) & 0x78)) {
            var_a1_2 = (s32) (temp_f8 - 2.1474836e9f) | 0x80000000;
        } else {
            goto block_10;
        }
    } else {
        var_a1_2 = (s32) temp_f8;
        if (var_a1_2 < 0) {
block_10:
            var_a1_2 = -1;
        }
    }
    func_8001fff4(M2C_FIELD(ipa_s0, s32 *, 0x10), var_a1_2 & 0xFF);
    M2C_FIELD(ipa_s0, s8 *, 1) = 1;
    return 1;
}