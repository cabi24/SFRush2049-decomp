/* Diagnostic alternate helper body, not standalone or a match claim.
 * Typed m2c control flow retained to separate geometry from semantics. */
void func_800D348C(CameraCar *ipa_s0, s32 *ipa_s2, s32 *ipa_s3) {
    s32 sp84;
    s32 sp80;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f12_3;
    f32 temp_f12_4;
    f32 temp_f12_5;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f14_3;
    f32 temp_f14_4;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f26;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f2_5;
    f32 var_f18;
    f32 var_f18_2;
    f32 var_f18_3;
    f32 var_f18_4;
    f32 var_f20;
    f32 var_f22;
    f32 var_f24;
    s16 temp_a0;
    s16 temp_a0_2;
    s16 temp_a2;
    s16 temp_v0_5;
    s16 temp_v0_7;
    s16 temp_v0_8;
    s16 temp_v0_9;
    s32 var_a3_4;
    s32 var_a3_5;
    s32 temp_a1;
    f32 randomOffset;
    s32 temp_v0;
    s32 temp_v0_4;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_a3;
    s32 var_a3_2;
    s32 var_a3_3;
    s32 var_t5;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    void *temp_v0_2;
    void *temp_v0_3;
    void *temp_v0_6;
    void *temp_v1;
    void *temp_v1_2;
    void *var_a0;
    void *var_a2_2;
    void *var_v0;
    void *var_v0_2;

    temp_f26 = 1.0e20f;
    sp84 = -1;
    sp80 = 0;
    var_f18 = temp_f26;
    if (D_8014A110 == 6) {
        func_8038CA24((s32) M2C_FIELD(ipa_s0, s16 *, 0x7C6));
        var_f24 = 0.0f;
        var_t5 = 0;
        var_a3 = 0;
        randomOffset = func_8008B2E4((f32)(u32)D_801407F0.count);
        if ((s32) M2C_FIELD(&D_801407F0, u16 *, 0) > 0) {
            do {
                temp_v0 = (s32) randomOffset + var_a3;
                var_f18_2 = temp_f26;
                var_a2 = temp_v0;
                var_a1 = 0;
                if (temp_v0 >= (s32) M2C_FIELD(&D_801407F0, u16 *, 0)) {
                    var_a2 = temp_v0 - M2C_FIELD(&D_801407F0, u16 *, 0);
                }
                var_a3 += 1;
                if (D_8014A108 > 0) {
                    do {
                        if (var_a1 != M2C_FIELD(ipa_s0, s16 *, 0x7C6)) {
                            temp_v1 = (u8 *) D_801407F0.points + (var_a2 * 6);
                            temp_v0_2 = (u8 *) &D_8014A250 + (var_a1 * 0x808);
                            if (M2C_FIELD(temp_v0_2, s16 *, 0x6C4) >= 0) {
                                var_f20 = M2C_FIELD(temp_v0_2, f32 *, 0x660);
                                var_f22 = M2C_FIELD(temp_v0_2, f32 *, 0x668);
                            } else {
                                var_f20 = M2C_FIELD(temp_v0_2, f32 *, 0x22C);
                                var_f22 = M2C_FIELD(temp_v0_2, f32 *, 0x234);
                            }
                            temp_f0 = (f32) M2C_FIELD(temp_v1, s16 *, 0) - var_f20;
                            temp_f2 = (f32) M2C_FIELD(temp_v1, s16 *, 4) - var_f22;
                            temp_f12 = (temp_f0 * temp_f0) + (temp_f2 * temp_f2);
                            if (temp_f12 < var_f18_2) {
                                var_f18_2 = temp_f12;
                            }
                        }
                        var_a1 += 1;
                    } while (var_a1 < D_8014A108);
                }
                if (var_f24 < var_f18_2) {
                    var_t5 = var_a2;
                    var_f24 = var_f18_2;
                }
            } while (var_a3 < (s32) M2C_FIELD(&D_801407F0, u16 *, 0));
        }
        sp80 = var_t5;
    } else {
        var_a3_2 = 0;
        if (D_8014A110 == 5) {
            sp80 = (s32) M2C_FIELD(ipa_s0, s16 *, 0x7C4);
        } else {
            temp_f20 = M2C_FIELD(ipa_s0, f32 *, 0x66C);
            temp_f24 = M2C_FIELD(ipa_s0, f32 *, 0x670);
            temp_f22 = M2C_FIELD(ipa_s0, f32 *, 0x674);
            if ((s32) M2C_FIELD(&D_801407F0, u16 *, 0) > 0) {
                var_v0 = M2C_FIELD(&D_801407F0, void **, 4);
                do {
                    temp_f0_2 = (f32) M2C_FIELD(var_v0, s16 *, 0) - temp_f20;
                    temp_f12_2 = (f32) M2C_FIELD(var_v0, s16 *, 2) - temp_f24;
                    temp_f2_2 = (f32) M2C_FIELD(var_v0, s16 *, 4) - temp_f22;
                    temp_f14 = (temp_f0_2 * temp_f0_2) + (temp_f12_2 * temp_f12_2) + (temp_f2_2 * temp_f2_2);
                    if (temp_f14 < var_f18) {
                        var_f18 = temp_f14;
                        sp80 = var_a3_2;
                    }
                    var_a3_2 += 1;
                    var_v0 = (u8 *) var_v0 + 6;
                } while (var_a3_2 < (s32) M2C_FIELD(&D_801407F0, u16 *, 0));
            }
            if ((D_8014A110 != 1) && (D_8014A110 != 4) && (D_8014A110 != 5)) {
                if (var_f18 > 1600.0f) {
                    var_a1_2 = 0;
                    if ((s32) M2C_FIELD(&D_801407F0, u8 *, 8) > 0) {
                        var_a2_2 = M2C_FIELD(&D_801407F0, void **, 0xC);
                        do {
                            var_a3_3 = 0;
                            var_v1 = 0;
                            if ((s32) M2C_FIELD(var_a2_2, u16 *, 0xA) > 0) {
                                do {
                                    temp_v0_3 = M2C_FIELD(var_a2_2, s32 *, 0xC) + var_v1;
                                    temp_f0_3 = (f32) M2C_FIELD(temp_v0_3, s16 *, 0) - temp_f20;
                                    temp_f12_3 = (f32) M2C_FIELD(temp_v0_3, s16 *, 2) - temp_f24;
                                    temp_f2_3 = (f32) M2C_FIELD(temp_v0_3, s16 *, 4) - temp_f22;
                                    temp_f14_2 = (temp_f0_3 * temp_f0_3) + (temp_f12_3 * temp_f12_3) + (temp_f2_3 * temp_f2_3);
                                    if (temp_f14_2 < var_f18) {
                                        sp80 = var_a3_3;
                                        sp84 = var_a1_2;
                                        var_f18 = temp_f14_2;
                                    }
                                    var_a3_3 += 1;
                                    var_v1 += 6;
                                } while (var_a3_3 < (s32) M2C_FIELD(var_a2_2, u16 *, 0xA));
                            }
                            var_a1_2 += 1;
                            var_a2_2 = (u8 *) var_a2_2 + 0x10;
                        } while (var_a1_2 < (s32) M2C_FIELD(&D_801407F0, u8 *, 8));
                    }
                }
                if ((sp84 >= 0) && (*((u8 *) M2C_FIELD(&D_801407F0, void **, 0xC) + (sp84 * 0x10)) == 0)) {
                    func_800D3430(sp84, sp80, &sp84, &sp80, 1);
                }
                if ((sp84 < 0) || (*((u8 *) M2C_FIELD(&D_801407F0, void **, 0xC) + (sp84 * 0x10)) != 2)) {
                    if (sp84 >= 0) {
                        temp_a0 = M2C_FIELD(ipa_s0, s16 *, 0x7E2);
                        temp_v1_2 = (u8 *) &D_80151CE8 + (temp_a0 * 0x50);
                        temp_v0_4 = sp84 + 5;
                        if (M2C_FIELD(((u8 *) temp_v1_2 + (sp84 * 2)), s16 *, 0x38) >= 0) {
                            temp_a1 = temp_v0_4 * 2;
                            var_a0 = (u8 *) &D_80151CE8 + (M2C_FIELD(ipa_s0, s16 *, 0x7E4) * 0x50);
                            temp_a2 = M2C_FIELD(((u8 *) var_a0 + temp_a1), s16 *, 0x2E);
                            if (temp_a2 >= 0) {
                                temp_v0_5 = M2C_FIELD(((u8 *) temp_v1_2 + temp_a1), s16 *, 0x2E);
                                var_a3_4 = temp_v0_5;
                                if ((sp80 < temp_v0_5) || (temp_a2 < sp80)) {
                                    var_f18_3 = temp_f26;
                                    if (temp_a2 >= temp_v0_5) {
                                        var_v1_2 = temp_v0_5 * 6;
                                        do {
                                            temp_v0_6 = M2C_FIELD(((u8 *) M2C_FIELD(&D_801407F0, void **, 0xC) + (sp84 * 0x10)), s32 *, 0xC) + var_v1_2;
                                            temp_f0_4 = (f32) M2C_FIELD(temp_v0_6, s16 *, 0) - temp_f20;
                                            temp_f12_4 = (f32) M2C_FIELD(temp_v0_6, s16 *, 2) - temp_f24;
                                            temp_f2_4 = (f32) M2C_FIELD(temp_v0_6, s16 *, 4) - temp_f22;
                                            temp_f14_3 = (temp_f0_4 * temp_f0_4) + (temp_f12_4 * temp_f12_4) + (temp_f2_4 * temp_f2_4);
                                            if (temp_f14_3 < var_f18_3) {
                                                sp80 = (s32) var_a3_4;
                                                var_f18_3 = temp_f14_3;
                                                var_a0 = (u8 *) &D_80151CE8 + (M2C_FIELD(ipa_s0, s16 *, 0x7E4) * 0x50);
                                            }
                                            var_a3_4 += 1;
                                            var_v1_2 += 6;
                                        } while (M2C_FIELD(((u8 *) var_a0 + (temp_v0_4 * 2)), s16 *, 0x2E) >= var_a3_4);
                                    }
                                }
                            } else {
                                temp_v0_7 = M2C_FIELD(((u8 *) temp_v1_2 + temp_a1), s16 *, 0x2E);
                                if (sp80 < temp_v0_7) {
                                    sp80 = (s32) temp_v0_7;
                                }
                            }
                        } else if ((M2C_FIELD(((u8 *) &D_80151CE8 + (M2C_FIELD(ipa_s0, s16 *, 0x7E4) * 0x50) + (temp_v0_4 * 2)), s16 *, 0x2E) < 0) && (temp_a0 != M2C_FIELD(((u8 *) M2C_FIELD(&D_801407F0, void **, 0xC) + (sp84 * 0x10)), s8 *, 8))) {
                            sp84 = -1;
                            sp80 = -1;
                        }
                    }
                    if (sp84 < 0) {
                        temp_v0_8 = M2C_FIELD(ipa_s0, s16 *, 0x7E4);
                        temp_a0_2 = M2C_FIELD(ipa_s0, s16 *, 0x7E2);
                        if (temp_v0_8 < temp_a0_2) {
                            var_v1_3 = M2C_FIELD(&D_801407F0, u16 *, 0);
                        } else {
                            var_v1_3 = (u16) M2C_FIELD(((u8 *) &D_80151CE8 + (temp_v0_8 * 0x50)), s16 *, 0x2E);
                        }
                        temp_v0_9 = M2C_FIELD(((u8 *) &D_80151CE8 + (temp_a0_2 * 0x50)), s16 *, 0x2E);
                        var_a3_5 = temp_v0_9;
                        if ((sp80 < temp_v0_9) || (sp80 >= (s32) var_v1_3)) {
                            var_f18_4 = temp_f26;
                            if (temp_v0_9 < (s32) var_v1_3) {
                                var_v0_2 = (u8 *) D_801407F0.points + (temp_v0_9 * 6);
                                do {
                                    temp_f0_5 = (f32) M2C_FIELD(var_v0_2, s16 *, 0) - temp_f20;
                                    temp_f12_5 = (f32) M2C_FIELD(var_v0_2, s16 *, 2) - temp_f24;
                                    temp_f2_5 = (f32) M2C_FIELD(var_v0_2, s16 *, 4) - temp_f22;
                                    temp_f14_4 = (temp_f0_5 * temp_f0_5) + (temp_f12_5 * temp_f12_5) + (temp_f2_5 * temp_f2_5);
                                    if (temp_f14_4 < var_f18_4) {
                                        var_f18_4 = temp_f14_4;
                                        sp80 = (s32) var_a3_5;
                                    }
                                    var_a3_5 += 1;
                                    var_v0_2 = (u8 *) var_v0_2 + 6;
                                } while (var_a3_5 < (s32) var_v1_3);
                            }
                        }
                    }
                }
            }
        }
    }
    *ipa_s2 = sp84;
    *ipa_s3 = sp80;
}
