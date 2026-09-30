M2C_UNK func_8008B32C(s32, s32, f32);               /* extern */
M2C_UNK math_utility(f32, f32, void *, s32, s16, s16); /* extern */
M2C_UNK model_data_load(s32, M2C_UNK, M2C_UNK);     /* extern */
M2C_UNK model_transform_setup(s32, M2C_UNK, M2C_UNK); /* extern */

/* Warning: Gap in callee-saved word stack region.
 * Saved: [0x14, 0x170], gap at: 0x18. */
void entity_tick_main(s32 arg0) {
    GameCar *temp_s0;
    GameCar *temp_s0_2;
    f32 temp_a1_2;
    f32 temp_a1_3;
    f32 temp_f0;
    f32 temp_f10;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f14_3;
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f22;
    f32 temp_f4;
    f32 temp_f6;
    f32 temp_f8;
    f32 temp_f8_2;
    f32 var_f12;
    f32 var_f12_2;
    f32 var_f16;
    f32 var_f22;
    f32 var_f2;
    s16 temp_a2;
    s16 temp_a3;
    s32 *var_a0;
    s32 temp_a1;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_lo_6;
    s32 temp_lo_7;
    s32 temp_lo_8;
    s32 temp_lo_9;
    s32 temp_t0;
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_t7_3;
    s32 temp_t7_4;
    s32 temp_t7_5;
    s32 temp_t7_6;
    s32 temp_t7_7;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 temp_t9_3;
    s32 temp_t9_4;
    s32 temp_t9_5;
    s32 temp_v1;
    s32 var_a1;
    s32 var_t9;
    void *temp_s1;
    void *temp_s2;
    void *temp_s2_2;
    void *temp_v1_2;

    unksp154 = *(s32 *)0x8011B464;
    if (arg0 != 0) {
        temp_s2 = ((s16) saved_reg_s5 << 6) + 0x80139320;
        model_data_load(M2C_FIELD(temp_s2, s32 *, 4), 1, 0xF);
        model_data_load(M2C_FIELD(temp_s2, s32 *, 8), 1, 0xF);
        model_data_load(M2C_FIELD(temp_s2, s32 *, 0xC), 1, 0xF);
        return;
    }
    temp_s2_2 = ((s16) saved_reg_s5 << 6) + 0x80139320;
    model_transform_setup(M2C_FIELD(temp_s2_2, s32 *, 4), 0, 0xF);
    model_transform_setup(M2C_FIELD(temp_s2_2, s32 *, 8), 0, 0xF);
    model_transform_setup(M2C_FIELD(temp_s2_2, s32 *, 0xC), 0, 0xF);
    if ((gameplay_mode == 2) && ((s16) saved_reg_s5 > 0)) {
        unksp157 = 0x80;
    } else if (gameplay_mode == 6) {
        temp_s0 = &player_array[(s16) saved_reg_s5];
        if (M2C_FIELD(temp_s0, s32 *, 0x38C) & 1) {
            unksp157 = temp_s0->pad381[0x20];
        }
    }
    temp_t0 = M2C_FIELD(temp_s2_2, s32 *, 4);
    temp_v1 = unksp154;
    M2C_FIELD((((s16) temp_t0 * 0x44) + 0x8012E700), s32 *, 0x3C) = temp_v1;
    M2C_FIELD(((M2C_FIELD(temp_s2_2, s16 *, 0xA) * 0x44) + 0x8012E700), s32 *, 0x3C) = temp_v1;
    temp_a2 = M2C_FIELD(temp_s2_2, s16 *, 0xE);
    M2C_FIELD(((temp_a2 * 0x44) + 0x8012E700), s32 *, 0x3C) = temp_v1;
    temp_a3 = M2C_FIELD(temp_s2_2, s16 *, 2);
    temp_f14 = *(f32 *)0x80123A00;
    temp_s0_2 = &player_array[(s16) saved_reg_s5];
    temp_s1 = M2C_FIELD(((temp_a3 * 0x44) + 0x8012E700), void **, 8);
    temp_f8 = (2.0f * M2C_FIELD(temp_s1, f32 *, 0xC)) + M2C_FIELD(temp_s1, f32 *, 0x24);
    unksp144 = temp_f8;
    temp_a1 = M2C_FIELD((((s16) temp_t0 * 0x44) + 0x8012E700), s32 *, 8);
    temp_f6 = (2.0f * M2C_FIELD(temp_s1, f32 *, 0x10)) + M2C_FIELD(temp_s1, f32 *, 0x28);
    unksp148 = temp_f6;
    temp_f12 = M2C_FIELD(temp_s1, f32 *, 0x14);
    temp_f10 = (2.0f * temp_f12) + M2C_FIELD(temp_s1, f32 *, 0x2C);
    unksp14C = temp_f10;
    temp_f4 = (M2C_FIELD(temp_s1, f32 *, 0x18) * temp_f14) + temp_f8;
    unksp144 = temp_f4;
    temp_f8_2 = (M2C_FIELD(temp_s1, f32 *, 0x1C) * temp_f14) + temp_f6;
    unksp148 = temp_f8_2;
    unksp158 = temp_a1;
    unksp14C = (M2C_FIELD(temp_s1, f32 *, 0x20) * temp_f14) + temp_f10;
    math_utility(temp_f12, temp_f14, temp_s1, temp_a1, temp_a2, temp_a3);
    M2C_FIELD(unksp158, f32 *, 0x24) = temp_f4;
    M2C_FIELD(unksp158, f32 *, 0x28) = unksp148;
    M2C_FIELD(unksp158, f32 *, 0x2C) = unksp14C;
    temp_a1_2 = M2C_FIELD(((M2C_FIELD(temp_s2_2, s16 *, 0xA) * 0x44) + 0x8012E700), f32 *, 8);
    unksp15C = temp_a1_2;
    math_utility(M2C_BITWISE(f32, temp_s1), temp_a1_2);
    M2C_FIELD(unksp15C, f32 *, 0x24) = unksp144;
    M2C_FIELD(unksp15C, f32 *, 0x28) = unksp148;
    M2C_FIELD(unksp15C, f32 *, 0x2C) = unksp14C;
    temp_a1_3 = M2C_FIELD(((M2C_FIELD(temp_s2_2, s16 *, 0xE) * 0x44) + 0x8012E700), f32 *, 8);
    unksp160 = temp_a1_3;
    math_utility(M2C_BITWISE(f32, temp_s1), temp_a1_3);
    var_a1 = 0;
    M2C_FIELD(unksp160, f32 *, 0x24) = unksp144;
    M2C_FIELD(unksp160, f32 *, 0x28) = temp_f8_2;
    M2C_FIELD(unksp160, f32 *, 0x2C) = unksp14C;
    if ((gameplay_mode == 6) && ((s8) temp_s0_2->pad381[3] == 3)) {
        temp_f0 = *(f32 *)0x80123A04;
        var_a1 = 1;
        M2C_FIELD(temp_s0_2, f32 *, 0x374) = temp_f0;
        M2C_FIELD(temp_s0_2, f32 *, 0x378) = temp_f0;
        M2C_FIELD(temp_s0_2, f32 *, 0x37C) = (f32) *(f32 *)0x80123A08;
    } else if (((s16) M2C_FIELD(temp_s0_2, s16 *, 0xF8) >> 2) < 0) {
        temp_t7 = (*(s32 *)0x8011735C * 0x41C64E6D) + 0x3039;
        *(s32 *)0x8011735C = temp_t7;
        temp_f20 = *(f32 *)0x80123A0C;
        M2C_FIELD(temp_s0_2, f32 *, 0x374) = (f32) (0.25f - (((f32) ((temp_t7 >> 0x10) & 0x7FFF) * temp_f20) / 32768.0f));
        temp_t9 = (*(s32 *)0x8011735C * 0x41C64E6D) + 0x3039;
        *(s32 *)0x8011735C = temp_t9;
        M2C_FIELD(temp_s0_2, f32 *, 0x378) = (f32) (0.25f - (((f32) ((temp_t9 >> 0x10) & 0x7FFF) * temp_f20) / 32768.0f));
        temp_t7_2 = (*(s32 *)0x8011735C * 0x41C64E6D) + 0x3039;
        *(s32 *)0x8011735C = temp_t7_2;
        M2C_FIELD(temp_s0_2, f32 *, 0x37C) = (f32) (*(f32 *)0x80123A14 - (((f32) ((temp_t7_2 >> 0x10) & 0x7FFF) * *(f32 *)0x80123A10) / 32768.0f));
    } else {
        temp_v1_2 = (D_8014A250_Record *) (((s16) saved_reg_s5 * 0x808) + (u8 *) &D_8014A250);
        if (M2C_FIELD(temp_v1_2, f32 *, 0x3D0) < *(f32 *)0x80123A18) {
            temp_f14_2 = *(f32 *)0x80123A1C;
            if (M2C_FIELD(temp_s0_2, f32 *, 0x374) < temp_f14_2) {
                temp_t7_3 = (*(void *)0x8011735C * 0x41C64E6D) + 0x3039;
                *(void *)0x8011735C = temp_t7_3;
                M2C_FIELD(temp_s0_2, f32 *, 0x374) = (f32) (0.25f - (((f32) ((temp_t7_3 >> 0x10) & 0x7FFF) * *(f32 *)0x80123A20) / 32768.0f));
                var_f22 = *(f32 *)0x80123A24;
                var_f16 = *(f32 *)0x80123A28;
                var_f2 = *(f32 *)0x80123A2C;
            } else {
                temp_t9_2 = (*(void *)0x8011735C * 0x41C64E6D) + 0x3039;
                *(void *)0x8011735C = temp_t9_2;
                var_f16 = *(f32 *)0x80123A30;
                var_f2 = *(f32 *)0x80123A34;
                var_f22 = *(f32 *)0x80123A38;
                M2C_FIELD(temp_s0_2, f32 *, 0x374) = (f32) (M2C_FIELD(temp_s0_2, f32 *, 0x374) - ((((f32) ((temp_t9_2 >> 0x10) & 0x7FFF) * var_f16) / 32768.0f) + var_f22));
                if (M2C_FIELD(temp_s0_2, f32 *, 0x374) < var_f2) {
                    M2C_FIELD(temp_s0_2, f32 *, 0x374) = var_f2;
                }
            }
            if (M2C_FIELD(temp_s0_2, f32 *, 0x378) < temp_f14_2) {
                temp_t7_4 = (*(void *)0x8011735C * 0x41C64E6D) + 0x3039;
                *(void *)0x8011735C = temp_t7_4;
                M2C_FIELD(temp_s0_2, f32 *, 0x378) = (f32) (0.25f - (((f32) ((temp_t7_4 >> 0x10) & 0x7FFF) * *(f32 *)0x80123A3C) / 32768.0f));
            } else {
                temp_t9_3 = (*(void *)0x8011735C * 0x41C64E6D) + 0x3039;
                *(void *)0x8011735C = temp_t9_3;
                M2C_FIELD(temp_s0_2, f32 *, 0x378) = (f32) (M2C_FIELD(temp_s0_2, f32 *, 0x378) - ((((f32) ((temp_t9_3 >> 0x10) & 0x7FFF) * var_f16) / 32768.0f) + var_f22));
                if (M2C_FIELD(temp_s0_2, f32 *, 0x378) < var_f2) {
                    M2C_FIELD(temp_s0_2, f32 *, 0x378) = var_f2;
                }
            }
            if (M2C_FIELD(temp_s0_2, f32 *, 0x37C) < *(f32 *)0x80123A40) {
                temp_t7_5 = (*(void *)0x8011735C * 0x41C64E6D) + 0x3039;
                *(void *)0x8011735C = temp_t7_5;
                M2C_FIELD(temp_s0_2, f32 *, 0x37C) = (f32) (*(f32 *)0x80123A48 - (((f32) ((temp_t7_5 >> 0x10) & 0x7FFF) * *(f32 *)0x80123A44) / 32768.0f));
            } else {
                temp_t9_4 = (*(void *)0x8011735C * 0x41C64E6D) + 0x3039;
                *(void *)0x8011735C = temp_t9_4;
                M2C_FIELD(temp_s0_2, f32 *, 0x37C) = (f32) (M2C_FIELD(temp_s0_2, f32 *, 0x37C) - ((((f32) ((temp_t9_4 >> 0x10) & 0x7FFF) * var_f16) / 32768.0f) + var_f22));
                if (M2C_FIELD(temp_s0_2, f32 *, 0x37C) < 0.5f) {
                    M2C_FIELD(temp_s0_2, f32 *, 0x37C) = 0.5f;
                }
            }
        } else {
            temp_f14_3 = (f32) M2C_FIELD(temp_v1_2, s16 *, 0x7D0);
            temp_f22 = *(f32 *)0x80123A50;
            var_f12 = ((temp_f14_3 * *(f32 *)0x80123A54) / temp_f22) + *(f32 *)0x80123A4C;
            if (var_f12 > 1.0f) {
                var_f12 = 1.0f;
            }
            temp_t7_6 = (*(void *)0x8011735C * 0x41C64E6D) + 0x3039;
            *(void *)0x8011735C = temp_t7_6;
            temp_f20_2 = *(f32 *)0x80123A58;
            M2C_FIELD(temp_s0_2, f32 *, 0x374) = (f32) (var_f12 - (((f32) ((temp_t7_6 >> 0x10) & 0x7FFF) * temp_f20_2) / 32768.0f));
            temp_t9_5 = (*(void *)0x8011735C * 0x41C64E6D) + 0x3039;
            *(void *)0x8011735C = temp_t9_5;
            var_f12_2 = (temp_f14_3 / temp_f22) + 0.5f;
            M2C_FIELD(temp_s0_2, f32 *, 0x378) = (f32) (var_f12 - (((f32) ((temp_t9_5 >> 0x10) & 0x7FFF) * temp_f20_2) / 32768.0f));
            if (var_f12_2 > 1.0f) {
                var_f12_2 = 1.0f;
            }
            temp_t7_7 = (*(void *)0x8011735C * 0x41C64E6D) + 0x3039;
            *(void *)0x8011735C = temp_t7_7;
            M2C_FIELD(temp_s0_2, f32 *, 0x37C) = (f32) (var_f12_2 - (((f32) ((temp_t7_7 >> 0x10) & 0x7FFF) * *(f32 *)0x80123A5C) / 32768.0f));
        }
    }
    M2C_FIELD(unksp158, f32 *, 0x18) = (f32) (M2C_FIELD(unksp158, f32 *, 0x18) * M2C_FIELD(temp_s0_2, f32 *, 0x374));
    M2C_FIELD(unksp158, f32 *, 0x1C) = (f32) (M2C_FIELD(unksp158, f32 *, 0x1C) * M2C_FIELD(temp_s0_2, f32 *, 0x374));
    M2C_FIELD(unksp158, f32 *, 0x20) = (f32) (M2C_FIELD(unksp158, f32 *, 0x20) * M2C_FIELD(temp_s0_2, f32 *, 0x374));
    M2C_FIELD(unksp15C, f32 *, 0x18) = (f32) (M2C_FIELD(unksp15C, f32 *, 0x18) * M2C_FIELD(temp_s0_2, f32 *, 0x378));
    M2C_FIELD(unksp15C, f32 *, 0x1C) = (f32) (M2C_FIELD(unksp15C, f32 *, 0x1C) * M2C_FIELD(temp_s0_2, f32 *, 0x378));
    M2C_FIELD(unksp15C, f32 *, 0x20) = (f32) (M2C_FIELD(unksp15C, f32 *, 0x20) * M2C_FIELD(temp_s0_2, f32 *, 0x378));
    if (var_a1 != 0) {
        func_8008B32C(unksp160, unksp160, M2C_FIELD(temp_s0_2, f32 *, 0x37C));
    } else {
        M2C_FIELD(unksp160, f32 *, 0) = (f32) (M2C_FIELD(unksp160, f32 *, 0) * M2C_FIELD(temp_s0_2, f32 *, 0x37C));
        M2C_FIELD(unksp160, f32 *, 4) = (f32) (M2C_FIELD(unksp160, f32 *, 4) * M2C_FIELD(temp_s0_2, f32 *, 0x37C));
        M2C_FIELD(unksp160, f32 *, 8) = (f32) (M2C_FIELD(unksp160, f32 *, 8) * M2C_FIELD(temp_s0_2, f32 *, 0x37C));
        M2C_FIELD(unksp160, f32 *, 0xC) = (f32) (M2C_FIELD(unksp160, f32 *, 0xC) * M2C_FIELD(temp_s0_2, f32 *, 0x37C));
        M2C_FIELD(unksp160, f32 *, 0x10) = (f32) (M2C_FIELD(unksp160, f32 *, 0x10) * M2C_FIELD(temp_s0_2, f32 *, 0x37C));
        M2C_FIELD(unksp160, f32 *, 0x14) = (f32) (M2C_FIELD(unksp160, f32 *, 0x14) * M2C_FIELD(temp_s0_2, f32 *, 0x37C));
    }
    if ((gameplay_mode == 6) && (M2C_FIELD((((s16) saved_reg_s5 * 0x808) + 0x80150000), s16 *, -0x56EC) >= 0)) {
        temp_lo = M2C_FIELD(temp_s2_2, s32 *, 4) * 0x44;
        temp_lo_2 = M2C_FIELD(temp_s2_2, s32 *, 8) * 0x44;
        M2C_FIELD(temp_lo, s32 *, 0x8012E700) = (s32) (M2C_FIELD(temp_lo, s32 *, 0x8012E700) | 0x80000000);
        temp_lo_3 = M2C_FIELD(temp_s2_2, s32 *, 0xC) * 0x44;
        M2C_FIELD(temp_lo_2, s32 *, 0x8012E700) = (s32) (M2C_FIELD(temp_lo_2, s32 *, 0x8012E700) | 0x80000000);
        M2C_FIELD(temp_lo_3, s32 *, 0x8012E700) = (s32) (M2C_FIELD(temp_lo_3, s32 *, 0x8012E700) | 0x80000000);
        return;
    }
    if ((*(s8 *)0x80140418 != 0) || (temp_s0_2->unkE8 & 8)) {
        temp_lo_4 = M2C_FIELD(temp_s2_2, s32 *, 4) * 0x44;
        temp_lo_5 = M2C_FIELD(temp_s2_2, s32 *, 8) * 0x44;
        M2C_FIELD(temp_lo_4, s32 *, 0x8012E700) = (s32) (M2C_FIELD(temp_lo_4, s32 *, 0x8012E700) ^ 0x80000000);
        temp_lo_6 = M2C_FIELD(temp_s2_2, s32 *, 0xC) * 0x44;
        M2C_FIELD(temp_lo_5, s32 *, 0x8012E700) = (s32) (M2C_FIELD(temp_lo_5, s32 *, 0x8012E700) ^ 0x80000000);
        var_a0 = temp_lo_6 + 0x8012E700;
        var_t9 = M2C_FIELD(temp_lo_6, s32 *, 0x8012E700) ^ 0x80000000;
    } else {
        temp_lo_7 = M2C_FIELD(temp_s2_2, s32 *, 4) * 0x44;
        temp_lo_8 = M2C_FIELD(temp_s2_2, s32 *, 8) * 0x44;
        M2C_FIELD(temp_lo_7, s32 *, 0x8012E700) = (s32) (M2C_FIELD(temp_lo_7, s32 *, 0x8012E700) & 0x7FFFFFFF);
        temp_lo_9 = M2C_FIELD(temp_s2_2, s32 *, 0xC) * 0x44;
        M2C_FIELD(temp_lo_8, s32 *, 0x8012E700) = (s32) (M2C_FIELD(temp_lo_8, s32 *, 0x8012E700) & 0x7FFFFFFF);
        var_a0 = temp_lo_9 + 0x8012E700;
        var_t9 = M2C_FIELD(temp_lo_9, s32 *, 0x8012E700) & 0x7FFFFFFF;
    }
    *var_a0 = var_t9;
}
