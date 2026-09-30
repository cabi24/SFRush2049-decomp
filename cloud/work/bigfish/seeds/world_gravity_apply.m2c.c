f32 audio_channel_alloc(s16, s32);                  /* extern */
M2C_UNK func_80098A54(f32 *);                       /* extern */
M2C_UNK func_800B9F60(f32, f32, M2C_UNK, s32, M2C_UNK, s32 *); /* extern */
s32 func_800CF604(s16, M2C_UNK);                    /* extern */
M2C_UNK func_800D3430(s32, s32, s32 *, s32 *, s32); /* extern */

void world_gravity_apply(void) {
    s16 spEE;
    s32 spCC;
    s32 spC4;
    s32 spC0;
    s32 spBC;
    s16 spB4;
    s16 spB2;
    s16 spB0;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    f32 sp98;
    GameCar *temp_s2;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f12_3;
    f32 temp_f12_4;
    f32 temp_f12_5;
    f32 temp_f12_6;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f14_3;
    f32 temp_f14_4;
    f32 temp_f14_5;
    f32 temp_f14_6;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f2_5;
    f32 temp_f2_6;
    f32 var_f20;
    f32 var_f20_2;
    s16 temp_a0_2;
    s16 temp_s0;
    s16 temp_t7;
    s16 temp_v0;
    s16 temp_v0_3;
    s16 temp_v0_5;
    s16 temp_v0_7;
    s16 temp_v0_9;
    s16 temp_v1;
    s16 temp_v1_2;
    s16 temp_v1_4;
    s16 temp_v1_6;
    s16 var_s0_2;
    s16 var_s0_4;
    s16 var_s0_6;
    s16 var_s1;
    s16 var_s1_2;
    s16 var_v0_3;
    s32 temp_t0;
    s32 temp_t5;
    s32 temp_t8;
    s32 temp_v1_3;
    s32 temp_v1_5;
    s32 var_a1;
    s32 var_s0;
    s32 var_s0_3;
    s32 var_s0_5;
    s32 var_v1;
    s32 var_v1_2;
    s8 temp_v0_11;
    u16 temp_a1;
    u16 var_v1_3;
    u8 temp_a2;
    void *temp_a0;
    void *temp_s0_2;
    void *temp_s0_3;
    void *temp_s8;
    void *temp_v0_10;
    void *temp_v0_2;
    void *temp_v0_4;
    void *temp_v0_6;
    void *temp_v0_8;
    void *var_a0;
    void *var_v0;
    void *var_v0_2;

    if ((s8) D_80152744 > 0) {
        temp_f24 = *(f32 *)0x80124574;
        temp_f22 = *(f32 *)0x80124578;
        spEE = 0;
        do {
            temp_s0 = M2C_FIELD(((D_8014A250_Record *) ((u8 *) &D_8014A250 + (spEE * 0x808))), s16 *, 0x7C6);
            temp_s8 = (D_8014A250_Record *) ((u8 *) &D_8014A250 + (temp_s0 * 0x808));
            if (func_800CF604(temp_s0, 0x808) != 0) {
                temp_s2 = &player_array[temp_s0];
                if ((s8) temp_s2->pad0EC[3] != 1) {
                    var_f20 = *(f32 *)0x8012457C;
                    var_s0 = 0;
                    spB0 = (s16) (s32) M2C_FIELD(temp_s8, f32 *, 0x22C);
                    spB2 = (s16) (s32) M2C_FIELD(temp_s8, f32 *, 0x230);
                    spC0 = 0;
                    spC4 = -1;
                    spB4 = (s16) (s32) M2C_FIELD(temp_s8, f32 *, 0x234);
                    spCC = (s32) M2C_FIELD(temp_s2, s16 *, 0xFE);
                    do {
                        temp_v0_2 = M2C_FIELD((void *)0x801407F0, s32 *, 4) + (spCC * 6);
                        temp_f0 = (f32) (M2C_FIELD(temp_v0_2, s16 *, 0) - spB0);
                        temp_f2 = (f32) (M2C_FIELD(temp_v0_2, s16 *, 2) - spB2);
                        temp_f12 = (f32) (M2C_FIELD(temp_v0_2, s16 *, 4) - spB4);
                        temp_f14 = (temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f12 * temp_f12);
                        if (temp_f14 < var_f20) {
                            var_f20 = temp_f14;
                            spC0 = spCC;
                        }
                        func_800B9F60(temp_f12, temp_f14, -1, spCC, 0, &spCC);
                        var_s0 += 1;
                    } while (var_s0 < 5);
                    if (temp_f24 < var_f20) {
                        temp_v1 = M2C_FIELD(temp_s2, s16 *, 0xFA);
                        if (temp_v1 >= 0) {
                            temp_a0 = M2C_FIELD((void *)0x801407F0, void **, 0xC);
                            temp_v0_3 = M2C_FIELD(temp_s2, s16 *, 0xFC);
                            temp_a1 = M2C_FIELD(((u8 *) temp_a0 + (temp_v1 * 0x10)), u16 *, 0xA);
                            if ((temp_a1 - temp_v0_3) >= 6) {
                                var_s1 = temp_v0_3 + 5;
                            } else {
                                var_s1 = (s16) temp_a1;
                            }
                            var_s0_2 = temp_v0_3;
                            if (temp_v0_3 < var_s1) {
                                var_v1 = temp_v0_3 * 6;
                                do {
                                    temp_v0_4 = M2C_FIELD(((u8 *) temp_a0 + (M2C_FIELD(temp_s2, s16 *, 0xFA) * 0x10)), s32 *, 0xC) + var_v1;
                                    temp_f0_2 = (f32) (M2C_FIELD(temp_v0_4, s16 *, 0) - spB0);
                                    temp_f2_2 = (f32) (M2C_FIELD(temp_v0_4, s16 *, 2) - spB2);
                                    temp_f12_2 = (f32) (M2C_FIELD(temp_v0_4, s16 *, 4) - spB4);
                                    temp_f14_2 = (temp_f0_2 * temp_f0_2) + (temp_f2_2 * temp_f2_2) + (temp_f12_2 * temp_f12_2);
                                    if (temp_f14_2 < var_f20) {
                                        spC0 = (s32) var_s0_2;
                                        var_f20 = temp_f14_2;
                                        spC4 = (s32) M2C_FIELD(temp_s2, s16 *, 0xFA);
                                    }
                                    var_s0_2 += 1;
                                    var_v1 += 6;
                                } while (var_s0_2 < var_s1);
                            }
                        } else {
                            temp_v0_5 = M2C_FIELD(temp_s2, s16 *, 0xFC);
                            var_s0_3 = 0;
                            if (temp_v0_5 != M2C_FIELD(temp_s2, s16 *, 0xFE)) {
                                spCC = (s32) temp_v0_5;
                                do {
                                    temp_v0_6 = M2C_FIELD((void *)0x801407F0, s32 *, 4) + (spCC * 6);
                                    temp_f0_3 = (f32) (M2C_FIELD(temp_v0_6, s16 *, 0) - spB0);
                                    temp_f2_3 = (f32) (M2C_FIELD(temp_v0_6, s16 *, 2) - spB2);
                                    temp_f12_3 = (f32) (M2C_FIELD(temp_v0_6, s16 *, 4) - spB4);
                                    temp_f14_3 = (temp_f0_3 * temp_f0_3) + (temp_f2_3 * temp_f2_3) + (temp_f12_3 * temp_f12_3);
                                    if (temp_f14_3 < var_f20) {
                                        var_f20 = temp_f14_3;
                                        spC0 = spCC;
                                        spC4 = -1;
                                    }
                                    func_800B9F60(temp_f12_3, temp_f14_3, -1, spCC, 0, &spCC);
                                    var_s0_3 += 1;
                                } while (var_s0_3 < 5);
                            }
                        }
                    }
                    if (*(f32 *)0x80124580 < var_f20) {
                        temp_v0_7 = M2C_FIELD(temp_s8, s16 *, 0x7E4);
                        temp_v1_2 = M2C_FIELD(temp_s8, s16 *, 0x7E2);
                        if (temp_v0_7 < temp_v1_2) {
                            var_s1_2 = M2C_FIELD((void *)0x801407F0, s16 *, 0);
                        } else {
                            var_s1_2 = M2C_FIELD(((temp_v0_7 * 0x50) + 0x80151CE8), s16 *, 0x2E);
                        }
                        var_s0_4 = M2C_FIELD(((temp_v1_2 * 0x50) + 0x80151CE8), s16 *, 0x2E);
                        if (var_s0_4 < var_s1_2) {
                            var_v0 = M2C_FIELD((void *)0x801407F0, s32 *, 4) + (var_s0_4 * 6);
loop_30:
                            temp_f0_4 = (f32) (M2C_FIELD(var_v0, s16 *, 0) - spB0);
                            temp_f2_4 = (f32) (M2C_FIELD(var_v0, s16 *, 2) - spB2);
                            temp_f12_4 = (f32) (M2C_FIELD(var_v0, s16 *, 4) - spB4);
                            temp_f14_4 = (temp_f0_4 * temp_f0_4) + (temp_f2_4 * temp_f2_4) + (temp_f12_4 * temp_f12_4);
                            if (!(temp_f14_4 < var_f20) || (var_f20 = temp_f14_4, spC0 = (s32) var_s0_4, spC4 = -1, !(temp_f14_4 <= temp_f24))) {
                                var_s0_4 += 1;
                                var_v0 = (u8 *) var_v0 + 6;
                                if (var_s0_4 < var_s1_2) {
                                    goto loop_30;
                                }
                            }
                        }
                        if (var_s1_2 == var_s0_4) {
                            temp_a2 = M2C_FIELD((void *)0x801407F0, u8 *, 8);
                            var_a1 = 0;
                            if ((s32) temp_a2 > 0) {
                                var_a0 = M2C_FIELD((void *)0x801407F0, void **, 0xC);
loop_36:
                                if (M2C_FIELD(var_a0, u8 *, 0) != 2) {
                                    var_s0_5 = 0;
                                    var_v1_2 = 0;
                                    if ((s32) M2C_FIELD(var_a0, u16 *, 0xA) > 0) {
loop_38:
                                        temp_v0_8 = M2C_FIELD(var_a0, s32 *, 0xC) + var_v1_2;
                                        temp_f0_5 = (f32) (M2C_FIELD(temp_v0_8, s16 *, 0) - spB0);
                                        temp_f2_5 = (f32) (M2C_FIELD(temp_v0_8, s16 *, 2) - spB2);
                                        temp_f12_5 = (f32) (M2C_FIELD(temp_v0_8, s16 *, 4) - spB4);
                                        temp_f14_5 = (temp_f0_5 * temp_f0_5) + (temp_f2_5 * temp_f2_5) + (temp_f12_5 * temp_f12_5);
                                        if ((temp_f14_5 < var_f20) && (var_f20 = temp_f14_5, spC0 = var_s0_5, spC4 = var_a1, (temp_f14_5 <= temp_f22))) {

                                        } else {
                                            var_s0_5 += 1;
                                            var_v1_2 += 6;
                                            if (var_s0_5 < (s32) M2C_FIELD(var_a0, u16 *, 0xA)) {
                                                goto loop_38;
                                            }
                                        }
                                    }
                                    if (var_s0_5 >= (s32) M2C_FIELD(var_a0, u16 *, 0xA)) {
                                        goto block_43;
                                    }
                                } else {
block_43:
                                    var_a1 += 1;
                                    var_a0 = (u8 *) var_a0 + 0x10;
                                    if (var_a1 < (s32) temp_a2) {
                                        goto loop_36;
                                    }
                                }
                            }
                            if (var_a1 == temp_a2) {
                                var_s0_6 = M2C_FIELD(temp_s2, s16 *, 0xFE);
                                temp_v1_3 = M2C_FIELD((void *)0x801407F0, s32 *, 4);
                                var_v0_2 = temp_v1_3 + (var_s0_6 * 6);
loop_46:
                                temp_f0_6 = (f32) (M2C_FIELD(var_v0_2, s16 *, 0) - spB0);
                                temp_f2_6 = (f32) (M2C_FIELD(var_v0_2, s16 *, 2) - spB2);
                                temp_f12_6 = (f32) (M2C_FIELD(var_v0_2, s16 *, 4) - spB4);
                                temp_f14_6 = (temp_f0_6 * temp_f0_6) + (temp_f2_6 * temp_f2_6) + (temp_f12_6 * temp_f12_6);
                                if (!(temp_f14_6 < var_f20) || (var_f20 = temp_f14_6, spC0 = (s32) var_s0_6, spC4 = -1, !(temp_f14_6 <= temp_f24))) {
                                    var_s0_6 += 1;
                                    if (var_s0_6 >= (s32) (u16) M2C_FIELD((void *)0x801407F0, s16 *, 0)) {
                                        var_s0_6 = 0;
                                    }
                                    if (var_s0_6 != M2C_FIELD(temp_s2, s16 *, 0xFE)) {
                                        var_v0_2 = temp_v1_3 + (var_s0_6 * 6);
                                        goto loop_46;
                                    }
                                }
                            }
                        }
                    }
                    M2C_FIELD(temp_s2, s16 *, 0xFA) = (s16) spC4;
                    M2C_FIELD(temp_s2, s16 *, 0xFC) = (s16) spC0;
                    if (!(*(f32 *)0x80124584 < var_f20)) {
                        if (spC4 >= 0) {
                            func_800D3430(spC4, spC0, &spC4, &spC0, 0);
                        }
                        temp_v1_4 = M2C_FIELD(temp_s8, s16 *, 0x7E2);
                        if (spC0 >= M2C_FIELD(((temp_v1_4 * 0x50) + 0x80151CE8), s16 *, 0x2E)) {
                            temp_v0_9 = M2C_FIELD(temp_s8, s16 *, 0x7E4);
                            if (temp_v0_9 < temp_v1_4) {
                                var_v1_3 = (u16) M2C_FIELD((void *)0x801407F0, s16 *, 0);
                            } else {
                                var_v1_3 = (u16) M2C_FIELD(((temp_v0_9 * 0x50) + 0x80151CE8), s16 *, 0x2E);
                            }
                            if (spC0 < (s32) var_v1_3) {
                                M2C_FIELD(temp_s2, s16 *, 0xFE) = (s16) spC0;
                                if (M2C_FIELD(temp_s2, s16 *, 0xFA) >= 0) {
                                    var_f20_2 = 0.0f;
                                } else {
                                    func_800B9F60(nanf, M2C_BITWISE(f32, spC0), 0, (s32) &spBC);
                                    temp_v1_5 = M2C_FIELD((void *)0x801407F0, s32 *, 4);
                                    temp_v0_10 = temp_v1_5 + (spBC * 6);
                                    temp_s0_2 = temp_v1_5 + (spC0 * 6);
                                    spA4 = (f32) (M2C_FIELD(temp_v0_10, s16 *, 0) - M2C_FIELD(temp_s0_2, s16 *, 0));
                                    spA8 = (f32) (M2C_FIELD(temp_v0_10, s16 *, 2) - M2C_FIELD(temp_s0_2, s16 *, 2));
                                    spA8 = 0.0f;
                                    spAC = (f32) (M2C_FIELD(temp_v0_10, s16 *, 4) - M2C_FIELD(temp_s0_2, s16 *, 4));
                                    func_80098A54(&spA4);
                                    temp_s0_3 = M2C_FIELD((void *)0x801407F0, s32 *, 4) + (spC0 * 6);
                                    sp98 = M2C_FIELD(temp_s8, f32 *, 0x22C) - (f32) M2C_FIELD(temp_s0_3, s16 *, 0);
                                    sp9C = M2C_FIELD(temp_s8, f32 *, 0x230) - (f32) M2C_FIELD(temp_s0_3, s16 *, 2);
                                    spA0 = M2C_FIELD(temp_s8, f32 *, 0x234) - (f32) M2C_FIELD(temp_s0_3, s16 *, 4);
                                    var_f20_2 = (spAC * spA0) + (sp98 * spA4);
                                }
                                M2C_FIELD(temp_s2, f32 *, 0x100) = 0.0f;
                                temp_v0_11 = M2C_FIELD(temp_s8, s8 *, 0x7E8);
                                if (temp_v0_11 > 0) {
                                    M2C_FIELD(temp_s2, f32 *, 0x100) = (f32) (M2C_FIELD(temp_s2, f32 *, 0x100) + (*(f32 *)0x80152800 + ((f32) (temp_v0_11 - 1) * *(f32 *)0x801543AC)));
                                    temp_v1_6 = M2C_FIELD(temp_s8, s16 *, 0x7E2);
                                    temp_a0_2 = M2C_FIELD((void *)0x80151CE8, s16 *, 4);
                                    if (temp_v1_6 < temp_a0_2) {
                                        var_v0_3 = M2C_FIELD((void *)0x80151CE8, s16 *, 8);
                                    } else {
                                        var_v0_3 = temp_v1_6;
                                    }
                                    spCC = (s32) temp_a0_2;
                                    if (temp_a0_2 < var_v0_3) {
                                        do {
                                            M2C_FIELD(temp_s2, f32 *, 0x100) = (f32) (M2C_FIELD(temp_s2, f32 *, 0x100) + M2C_FIELD(((spCC * 0x50) + 0x80151CE8), f32 *, 0x58));
                                            temp_t8 = spCC + 1;
                                            spCC = temp_t8;
                                        } while (temp_t8 < var_v0_3);
                                    }
                                    if (var_v0_3 != M2C_FIELD(temp_s8, s16 *, 0x7E2)) {
                                        temp_t7 = M2C_FIELD((void *)0x80151CE8, s16 *, 2);
                                        spCC = (s32) temp_t7;
                                        if (temp_t7 < M2C_FIELD(temp_s8, s16 *, 0x7E2)) {
                                            do {
                                                M2C_FIELD(temp_s2, f32 *, 0x100) = (f32) (M2C_FIELD(temp_s2, f32 *, 0x100) + M2C_FIELD(((spCC * 0x50) + 0x80151CE8), f32 *, 0x58));
                                                temp_t5 = spCC + 1;
                                                spCC = temp_t5;
                                            } while (temp_t5 < M2C_FIELD(temp_s8, s16 *, 0x7E2));
                                        }
                                    }
                                } else {
                                    spCC = 0;
                                    if (M2C_FIELD(temp_s8, s16 *, 0x7E2) > 0) {
                                        do {
                                            M2C_FIELD(temp_s2, f32 *, 0x100) = (f32) (M2C_FIELD(temp_s2, f32 *, 0x100) + M2C_FIELD(((spCC * 0x50) + 0x80151CE8), f32 *, 0x58));
                                            temp_t0 = spCC + 1;
                                            spCC = temp_t0;
                                        } while (temp_t0 < M2C_FIELD(temp_s8, s16 *, 0x7E2));
                                    }
                                }
                                M2C_FIELD(temp_s2, f32 *, 0x100) = (f32) (M2C_FIELD(temp_s2, f32 *, 0x100) + audio_channel_alloc(M2C_FIELD(((M2C_FIELD(temp_s8, s16 *, 0x7E2) * 0x50) + 0x80151CE8), s16 *, 0x2E), spC0));
                                M2C_FIELD(temp_s2, f32 *, 0x100) = (f32) (M2C_FIELD(temp_s2, f32 *, 0x100) + var_f20_2);
                            }
                        }
                    }
                }
            }
            temp_v0 = spEE + 1;
            spEE = temp_v0;
        } while (temp_v0 < (s8) D_80152744);
    }
}
