M2C_UNK func_800DE860();                            /* extern */
f32 func_800F92C8(f32, M2C_UNK, s8, void *, M2C_UNK, s16); /* extern */

void render_large_objects(void) {
    M2C_UNK spBC;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f30;
    f32 temp_f4;
    f32 var_f0;
    f32 var_f0_2;
    f32 var_f12;
    f32 var_f22;
    f32 var_f24;
    f32 var_f26;
    f32 var_f28;
    f32 var_f2;
    s16 *temp_t8;
    s16 *temp_v0_5;
    s16 *temp_v0_7;
    s16 *temp_v1;
    s16 *temp_v1_2;
    s16 *temp_v1_4;
    s16 temp_a0;
    s16 temp_a2;
    s16 temp_a2_2;
    s16 temp_a3;
    s16 temp_a3_2;
    s16 temp_t0;
    s16 temp_t1;
    s16 temp_t1_2;
    s16 temp_t4;
    s16 temp_t4_2;
    s16 temp_t4_3;
    s16 temp_v0;
    s16 temp_v0_4;
    s16 temp_v0_6;
    s16 temp_v0_8;
    s16 var_a3;
    s16 var_a3_2;
    s16 var_a3_3;
    s16 var_a3_4;
    s16 var_s3;
    s16 var_s6;
    s16 var_t0;
    s16 var_t1;
    s16 var_t3;
    s16 var_t3_2;
    s16 var_t3_3;
    s16 var_t3_4;
    s16 var_t3_5;
    s16 var_t4;
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_4;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t0_2;
    s32 temp_t6_2;
    s32 temp_t7;
    s32 temp_v0_3;
    s32 var_a0;
    s32 var_v1_3;
    s8 temp_t5;
    s8 temp_t6;
    s8 temp_t6_3;
    s8 temp_t7_2;
    s8 temp_v1_3;
    void *temp_a1;
    void *temp_a1_2;
    void *temp_t2;
    void *temp_v0_2;

    func_800DE860();
    temp_t5 = *(s8 *)0x80152744;
    var_t3 = 0;
    var_s3 = 0;
    var_s6 = 0;
    if (temp_t5 > 0) {
        do {
            temp_v0 = M2C_FIELD(((var_t3 * 0x808) + 0x8014A250), s16 *, 0x7C6);
            temp_t7 = temp_v0 * 2;
            temp_t6 = M2C_FIELD(((temp_v0 * 0x3B8) + 0x80152818), s8 *, 0xEE);
            var_t3 += 1;
            *((s16 *) ((u8 *) &sp178[0] + temp_t7)) = (s16) temp_t6;
        } while (var_t3 < temp_t5);
        var_t3 = 0;
    }
    if (temp_t5 > 0) {
        do {
            var_t4 = 0;
            var_a3 = 0;
            if (temp_t5 > 0) {
loop_7:
                var_t4 = M2C_FIELD(((var_a3 * 0x808) + 0x8014A250), s16 *, 0x7C6);
                var_a3 += 1;
                if (var_t3 != (&sp178[0])[var_t4]) {
                    if (var_a3 < temp_t5) {
                        goto loop_7;
                    }
                }
            }
            (&sp188[0])[var_t3] = var_t4;
            temp_t8 = &(&sp1A0[0])[var_s3];
            if (M2C_FIELD(((var_t4 * 0x808) + 0x8014A250), s8 *, 0x7CC) == 2) {
                (&sp1AC[0])[var_s6] = var_t4;
                var_s6 += 1;
            } else {
                var_s3 += 1;
                *temp_t8 = var_t4;
            }
            var_t3 += 1;
        } while (var_t3 < temp_t5);
        var_t3 = 0;
    }
    if (var_s3 > 0) {
        do {
            M2C_FIELD((((&sp1A0[0])[var_t3] * 0x3B8) + 0x80152818), s16 *, 0x356) = var_t3;
            var_t3 += 1;
        } while (var_t3 < var_s3);
    }
    if (*(s32 *)0x801174B4 & 8) {
        var_t3_2 = 0;
        if (temp_t5 > 0) {
            do {
                temp_t4 = M2C_FIELD(((var_t3_2 * 0x808) + 0x8014A250), s16 *, 0x7C6);
                var_t3_2 += 1;
                temp_a1 = (temp_t4 * 0x808) + 0x8014A250;
                if (M2C_FIELD(temp_a1, s8 *, 0x7CC) == 1) {
                    M2C_FIELD(temp_a1, f32 *, 0x7EC) = 1.0f;
                }
            } while (var_t3_2 < temp_t5);
        }
    } else if (var_s3 != 0) {
        var_t3_3 = 0;
        if (*(f32 *)0x801543CC < 5.0f) {
            if (var_s3 > 0) {
                do {
                    temp_lo = (&sp1A0[0])[var_t3_3] * 0x808;
                    temp_t6_2 = ((var_s3 - var_t3_3) - 1) << 0x10;
                    var_t3_3 += 1;
                    temp_v0_2 = temp_lo + 0x8014A250;
                    M2C_FIELD(temp_v0_2, f32 *, 0x7EC) = 1.0f;
                    M2C_FIELD(temp_v0_2, f32 *, 0x7F0) = (f32) (((f32) (temp_t6_2 >> 0x10) * *(f32 *)0x80124640) + *(f32 *)0x8012463C);
                } while (var_t3_3 < var_s3);
            }
        } else {
            if (temp_t5 > 0) {
                do {
                    temp_t4_2 = M2C_FIELD(((var_t3_3 * 0x808) + 0x8014A250), s16 *, 0x7C6);
                    var_a3_2 = var_t3_3;
                    (&sp194[0])[temp_t4_2] = 0;
                    if (var_t3_3 < temp_t5) {
                        temp_t2 = (u8 *) &spBC + (temp_t4_2 * 0x18);
                        do {
                            var_v1 = 0;
                            temp_t1 = M2C_FIELD(((var_a3_2 * 0x808) + 0x8014A250), s16 *, 0x7C6);
                            if (temp_t4_2 == temp_t1) {
                                *((u8 *) temp_t2 + (temp_t1 * 4)) = *(f32 *)0x80124644;
                            } else {
                                do {
                                    temp_v0_3 = var_v1 * 4;
                                    temp_f4 = M2C_FIELD(((temp_t4_2 * 0x3B8) + 0x80152818 + temp_v0_3), f32 *, 8) - M2C_FIELD(((temp_t1 * 0x3B8) + 0x80152818 + temp_v0_3), f32 *, 8);
                                    var_v1 += 1;
                                    *((f32 *) ((u8 *) &sp154[0] + temp_v0_3)) = temp_f4;
                                } while (var_v1 < 3);
                                temp_f16 = (sp15C * sp15C) + ((sp154[0] * sp154[0]) + (sp158 * sp158));
                                *((u8 *) &spBC + (temp_t1 * 0x18) + (temp_t4_2 * 4)) = temp_f16;
                                *((u8 *) temp_t2 + (temp_t1 * 4)) = temp_f16;
                            }
                            var_a3_2 += 1;
                        } while (var_a3_2 < temp_t5);
                    }
                    var_t3_3 += 1;
                } while (var_t3_3 < temp_t5);
                var_t3_3 = 0;
            }
            if (var_s3 > 0) {
                do {
                    temp_v0_4 = (&sp1A0[0])[var_t3_3];
                    temp_lo_2 = temp_v0_4 * 0x808;
                    var_t3_3 += 1;
                    M2C_FIELD((temp_lo_2 + 0x8014A250), s16 *, 0x7E6) = temp_v0_4;
                } while (var_t3_3 < var_s3);
                var_t3_3 = 0;
            }
            if (var_s6 > 0) {
                do {
                    temp_t0 = var_t3_3 + 1;
                    var_a3_3 = temp_t0;
                    if (temp_t0 < var_s6) {
                        temp_a2 = (&sp1AC[0])[var_t3_3];
                        do {
                            temp_a0 = (&sp1AC[0])[var_a3_3];
                            temp_v0_5 = &(&sp194[0])[temp_a2];
                            if (*((u8 *) &spBC + (temp_a2 * 0x18) + (temp_a0 * 4)) < *(f32 *)0x80124648) {
                                temp_v1 = &(&sp194[0])[temp_a0];
                                *temp_v0_5 += 0xA;
                                *temp_v1 += 0xA;
                            }
                            var_a3_3 += 1;
                        } while (var_a3_3 < var_s6);
                    }
                    var_t3_3 = temp_t0;
                } while (temp_t0 < var_s6);
                var_t3_3 = 0;
            }
            if (var_s6 > 0) {
                do {
                    temp_a2_2 = (&sp1AC[0])[var_t3_3];
                    if ((&sp194[0])[temp_a2_2] < 0xA) {
                        temp_v0_6 = (&sp178[0])[temp_a2_2];
                        var_v1_2 = -1;
                        var_t1 = -1;
                        if (temp_v0_6 > 0) {
                            temp_a3 = M2C_FIELD(&(&sp188[0])[temp_v0_6], s16 *, -2);
                            if ((M2C_FIELD(((temp_a3 * 0x808) + 0x8014A250), s8 *, 0x7CC) == 1) && ((&sp194[0])[temp_a3] == 0)) {
                                var_t1 = temp_a3;
                            }
                        }
                        if (temp_v0_6 < (temp_t5 - 1)) {
                            temp_a3_2 = M2C_FIELD(&(&sp188[0])[temp_v0_6], s16 *, 2);
                            if ((M2C_FIELD(((temp_a3_2 * 0x808) + 0x8014A250), s8 *, 0x7CC) == 1) && ((&sp194[0])[temp_a3_2] == 0)) {
                                var_v1_2 = temp_a3_2;
                            }
                        }
                        temp_v0_7 = &(&sp194[0])[temp_a2_2];
                        if ((var_t1 != -1) && (temp_t0_2 = temp_a2_2 * 4, (var_v1_2 != -1))) {
                            if (*((u8 *) &spBC + (var_v1_2 * 0x18) + temp_t0_2) < *((u8 *) &spBC + (var_t1 * 0x18) + temp_t0_2)) {
                                var_t1 = var_v1_2;
                            }
                        } else if (var_t1 == -1) {
                            var_t1 = var_v1_2;
                        }
                        if (var_t1 != -1) {
                            temp_v1_2 = &(&sp194[0])[var_t1];
                            *temp_v0_7 += 1;
                            *temp_v1_2 += 1;
                            M2C_FIELD(((var_t1 * 0x808) + 0x8014A250), s16 *, 0x7E6) = temp_a2_2;
                        }
                    }
                    var_t3_3 += 1;
                } while (var_t3_3 < var_s6);
            }
            var_t0 = 0;
            if (temp_t5 > 0) {
                do {
                    temp_t4_3 = M2C_FIELD(((var_t0 * 0x808) + 0x8014A250), s16 *, 0x7C6);
                    temp_a1_2 = (temp_t4_3 * 0x808) + 0x8014A250;
                    if (M2C_FIELD(temp_a1_2, s16 *, 0x7CA) != 0) {
                        temp_v1_3 = *(s8 *)0x80152030;
                        temp_t1_2 = M2C_FIELD(temp_a1_2, s16 *, 0x7E6);
                        temp_v0_8 = (&sp178[0])[temp_t4_3];
                        var_a0 = *(s32 *)0x8014A110;
                        var_f22 = 1.0f - ((f32) temp_v1_3 / 5.0f);
                        if (temp_v1_3 == 5) {
                            var_f28 = 0.0f;
                        } else {
                            temp_f0 = (f32) (*(void *)0x80152744 - 1);
                            if (temp_v0_8 == 0) {
                                var_f28 = 0.5f / temp_f0;
                            } else {
                                var_f28 = (f32) temp_v0_8 / temp_f0;
                            }
                        }
                        if (var_a0 == 3) {
                            var_a0 = (s32) *(s8 *)0x8014978C;
                            var_v1_3 = 0;
                            if (*(s8 *)0x80152570 != 0) {
                                var_v1_3 = 6;
                            }
                            var_f22 *= ((f32) M2C_FIELD(((M2C_FIELD(temp_a1_2, s16 *, 0x7C6) * 0x4C) + (var_a0 * 2) + (var_v1_3 * 2) + 0x80150000), s16 *, 0x4484) * 0.125f) + 0.75f;
                        }
                        if (temp_t1_2 != temp_t4_3) {
                            if ((M2C_FIELD(((temp_t4_3 * 0x3B8) + 0x80152818), f32 *, 0x100) - M2C_FIELD(((temp_t1_2 * 0x3B8) + 0x80152818), f32 *, 0x100)) < -20.0f) {
                                var_f24 = 1.0f - (((*(f32 *)0x80124660 * var_f22) + *(f32 *)0x80124664) * var_f28);
                                var_f12 = func_800F92C8(-300.0f, 0xC2700000, (s8) var_a0, temp_a1_2);
                                var_f26 = ((*(f32 *)0x80124668 * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x8012466C * var_f22) + *(f32 *)0x80124670));
                                var_f2 = var_f26;
                            } else {
                                if ((&sp194[0])[temp_t1_2] < 2) {
                                    var_f26 = ((*(f32 *)0x8012467C * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x80124680 * var_f22) + *(f32 *)0x80124684));
                                    var_f2 = func_800F92C8(200.0f, 0x42700000, (s8) var_a0, temp_a1_2);
                                    var_f24 = 1.0f - (((*(f32 *)0x80124688 * var_f22) + *(f32 *)0x8012468C) * var_f28);
                                } else {
                                    temp_f14 = *(f32 *)0x80124690;
                                    var_f26 = ((*(f32 *)0x80124694 * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x80124698 * var_f22) + temp_f14));
                                    var_f2 = var_f26;
                                    var_f24 = 1.0f - (((*(f32 *)0x8012469C * var_f22) + temp_f14) * var_f28);
                                }
                                goto block_124;
                            }
                        } else if (temp_v0_8 == 0) {
                            if ((M2C_FIELD(((temp_t4_3 * 0x3B8) + 0x80152818), f32 *, 0x100) - M2C_FIELD(((sp18A * 0x3B8) + 0x80152818), f32 *, 0x100)) < 200.0f) {
                                var_f24 = 1.0f - (((*(f32 *)0x801246A0 * var_f22) + *(f32 *)0x801246A4) * var_f28);
                                var_f12 = func_800F92C8(200.0f, 0, 0x3B8, temp_a1_2, 0x80152818);
                                var_f26 = ((*(f32 *)0x801246B8 * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x801246BC * var_f22) + *(f32 *)0x801246C0));
                                var_f2 = var_f26;
                            } else {
                                var_f26 = ((*(f32 *)0x801246CC * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x801246D0 * var_f22) + *(f32 *)0x801246D4));
                                var_f2 = func_800F92C8(500.0f, 0x43480000, 0x3B8, temp_a1_2, 0x80152818);
                                var_f24 = 1.0f - (((*(f32 *)0x801246D8 * var_f22) + *(f32 *)0x801246DC) * var_f28);
                                var_f12 = var_f24;
                            }
                        } else if ((temp_v0_8 + 1) == *(void *)0x80152744) {
                            if ((M2C_FIELD(((temp_t4_3 * 0x3B8) + 0x80152818), f32 *, 0x100) - M2C_FIELD(((M2C_FIELD(&(&sp188[0])[*(void *)0x80152744], s16 *, -4) * 0x3B8) + 0x80152818), f32 *, 0x100)) < -150.0f) {
                                var_f24 = 1.0f - (((*(f32 *)0x801246E0 * var_f22) + *(f32 *)0x801246E4) * var_f28);
                                var_f12 = func_800F92C8(-150.0f, 0xC3960000, 0x3B8, temp_a1_2);
                                var_f26 = ((*(f32 *)0x801246F8 * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x801246FC * var_f22) + *(f32 *)0x80124700));
                                var_f2 = var_f26;
                            } else {
                                var_f26 = ((*(f32 *)0x80124704 * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x80124708 * var_f22) + *(f32 *)0x8012470C));
                                var_f2 = func_800F92C8(-150.0f, 0, 0x3B8, temp_a1_2);
                                var_f24 = 1.0f - (((*(f32 *)0x80124718 * var_f22) + *(f32 *)0x8012471C) * var_f28);
                                var_f12 = var_f24;
                            }
                        } else {
                            temp_v1_4 = &(&sp188[0])[temp_v0_8];
                            var_t3_4 = 0;
                            temp_f0_2 = M2C_FIELD(((temp_t4_3 * 0x3B8) + 0x80152818), f32 *, 0x100);
                            var_a3_4 = 0;
                            temp_f16_2 = temp_f0_2 - M2C_FIELD(((M2C_FIELD(temp_v1_4, s16 *, -2) * 0x3B8) + 0x80152818), f32 *, 0x100);
                            temp_f30 = temp_f0_2 - M2C_FIELD(((M2C_FIELD(temp_v1_4, s16 *, 2) * 0x3B8) + 0x80152818), f32 *, 0x100);
                            if ((temp_f16_2 > -150.0f) && (temp_f30 < 150.0f)) {
                                temp_f0_3 = (temp_f16_2 + temp_f30) / 2.0f;
                                if (temp_f0_3 < 0.0f) {
                                    var_f24 = 1.0f - (((*(f32 *)0x80124720 * var_f22) + *(f32 *)0x80124724) * var_f28);
                                    var_f12 = func_800F92C8(temp_f16_2 - temp_f0_3, 0xC3480000, 0x3B8, temp_a1_2, 0x80152818, 0);
                                    var_f26 = ((*(f32 *)0x80124738 * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x8012473C * var_f22) + *(f32 *)0x80124740));
                                    var_f2 = var_f26;
                                } else {
                                    var_f26 = ((*(f32 *)0x80124744 * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x80124748 * var_f22) + *(f32 *)0x8012474C));
                                    var_f2 = func_800F92C8(temp_f30 - temp_f0_3, 0x43480000, 0x3B8, temp_a1_2, 0x80152818, 0);
                                    var_f24 = 1.0f - (((*(f32 *)0x80124758 * var_f22) + *(f32 *)0x8012475C) * var_f28);
                                    var_f12 = var_f24;
                                }
                            } else {
                                var_v1_4 = 0;
                                if (temp_v0_8 > 0) {
                                    do {
                                        temp_t7_2 = M2C_FIELD((((&sp188[0])[var_t3_4] * 0x808) + 0x8014A250), s8 *, 0x7CC);
                                        var_t3_4 += 1;
                                        if (temp_t7_2 == 2) {
                                            var_a3_4 += 1;
                                        }
                                    } while (var_t3_4 < temp_v0_8);
                                }
                                var_t3_5 = temp_v0_8;
                                if (temp_v0_8 < *(void *)0x80152744) {
                                    do {
                                        temp_t6_3 = M2C_FIELD((((&sp188[0])[var_t3_5] * 0x808) + 0x8014A250), s8 *, 0x7CC);
                                        var_t3_5 += 1;
                                        if (temp_t6_3 == 2) {
                                            var_v1_4 += 1;
                                        }
                                    } while (var_t3_5 < *(void *)0x80152744);
                                }
                                if (var_a3_4 >= var_v1_4) {
                                    if (temp_f16_2 < -150.0f) {
                                        var_f24 = 1.0f - (((*(f32 *)0x80124760 * var_f22) + *(f32 *)0x80124764) * var_f28);
                                        var_f12 = func_800F92C8(-150.0f, 0xC3960000, 0x3B8, temp_a1_2, 0x80152818, var_a3_4);
                                        var_f26 = ((*(f32 *)0x80124778 * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x8012477C * var_f22) + *(f32 *)0x80124780));
                                        var_f2 = var_f26;
                                    } else {
                                        var_f26 = ((*(f32 *)0x80124784 * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x80124788 * var_f22) + *(f32 *)0x8012478C));
                                        var_f2 = func_800F92C8(-150.0f, 0, 0x3B8, temp_a1_2, 0x80152818, var_a3_4);
                                        var_f24 = 1.0f - (((*(f32 *)0x80124798 * var_f22) + *(f32 *)0x8012479C) * var_f28);
                                        var_f12 = var_f24;
                                    }
                                } else if (temp_f30 < 150.0f) {
                                    var_f24 = 1.0f - (((*(f32 *)0x801247A0 * var_f22) + *(f32 *)0x801247A4) * var_f28);
                                    var_f12 = func_800F92C8(150.0f, 0, 0x3B8, temp_a1_2, 0x80152818, var_a3_4);
                                    var_f26 = ((*(f32 *)0x801247B8 * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x801247BC * var_f22) + *(f32 *)0x801247C0));
                                    var_f2 = var_f26;
                                } else {
                                    var_f26 = ((*(f32 *)0x801247CC * var_f22) + 1.0f) - (var_f28 * ((*(f32 *)0x801247D0 * var_f22) + *(f32 *)0x801247D4));
                                    var_f2 = func_800F92C8(500.0f, 0x43160000, 0x3B8, temp_a1_2, 0x80152818, var_a3_4);
                                    var_f24 = 1.0f - (((*(f32 *)0x801247D8 * var_f22) + *(f32 *)0x801247DC) * var_f28);
block_124:
                                    var_f12 = var_f24;
                                }
                            }
                        }
                        if ((*(void *)0x8013FECB != 0) || (*(s8 *)0x80152718 != 0)) {
                            var_f12 = var_f24;
                        }
                        if ((*(void *)0x80152015 != 0) && (*M2C_ERROR(/* Read from unset register $t2 */) < M2C_FIELD(((M2C_FIELD(&(&sp1AC[0])[var_s6], s16 *, -2) * 0x3B8) + 0x80150000), s8 *, 0x2906))) {
                            var_f2 = var_f26;
                            var_f12 = var_f24;
                        }
                        temp_f0_4 = M2C_FIELD(M2C_ERROR(/* Read from unset register $a1 */), f32 *, 0x7EC);
                        if (var_f2 < temp_f0_4) {
                            var_f0 = temp_f0_4 - *(f32 *)0x801247E0;
                            if (var_f0 < var_f2) {
                                goto block_135;
                            }
                        } else {
                            var_f0 = temp_f0_4 + *(f32 *)0x801247E4;
                            if (var_f2 < var_f0) {
block_135:
                                var_f0 = var_f2;
                            }
                        }
                        temp_f14_2 = *(f32 *)0x801247E8;
                        M2C_FIELD(M2C_ERROR(/* Read from unset register $a1 */), f32 *, 0x7EC) = var_f0;
                        temp_f0_5 = M2C_FIELD(M2C_ERROR(/* Read from unset register $a1 */), f32 *, 0x7F0);
                        if (var_f12 < temp_f0_5) {
                            var_f0_2 = temp_f0_5 - temp_f14_2;
                            if (var_f0_2 < var_f12) {
                                goto block_140;
                            }
                        } else {
                            var_f0_2 = temp_f0_5 + temp_f14_2;
                            if (var_f12 < var_f0_2) {
block_140:
                                var_f0_2 = var_f12;
                            }
                        }
                        M2C_FIELD(M2C_ERROR(/* Read from unset register $a1 */), f32 *, 0x7F0) = var_f0_2;
                    }
                    var_t0 = M2C_ERROR(/* Read from unset register $t0 */) + 1;
                } while (var_t0 < *(void *)0x80152744);
            }
        }
    }
}
