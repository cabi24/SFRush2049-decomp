? entity_flags_apply(?, ?, ?, ?);                   /* extern */
? func_800A61B0(f32 *, f32 *, ? *, s32);            /* extern */
? func_800AD5D0(s32, u32, u16 *, void *);           /* extern */
? func_800AD650(? *, void *, u16);                  /* extern */
? func_800C36A0(? *, void *);                       /* extern */
? func_800C54F0(f32, f32, s16, ?);                  /* extern */
? func_803914B4(f32, f32, s8, s8, s8, s8);          /* extern */
? math_utility(? *, void *);                        /* extern */

void camera_play_script(void *arg0, void *arg1, void *arg2) {
    u16 sp238;
    f32 sp230;
    f32 sp22C;
    f32 sp228;
    f32 sp224;
    f32 sp220;
    f32 sp21C;
    f32 sp218;
    f32 sp214;
    f32 sp210;
    f32 sp20C;
    f32 sp208;
    f32 sp204;
    f32 sp200;
    f32 sp1FC;
    f32 sp1F8;
    f32 sp1F4;
    f32 sp1F0;
    f32 sp1EC;
    f32 sp1BC;
    ? sp14C;
    ? sp13C;
    ? sp118;
    s32 sp114;
    s32 sp110;
    s32 sp10C;
    s32 sp108;
    f32 sp100;
    f32 spFC;
    f32 spF8;
    f32 spF0;
    f32 spEC;
    s32 spDC;
    s32 spD8;
    f32 spCC;
    f32 spC8;
    void *spBC;
    s8 *sp98;
    f32 *sp94;                                      /* compiler-managed */
    ? *var_v0_3;
    f32 *var_t0;
    f32 *var_v0;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f14_3;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f16_3;
    f32 temp_f18;
    f32 temp_f18_2;
    f32 temp_f18_3;
    f32 temp_f20;
    f32 temp_f20_2;
    f32 temp_f24;
    f32 temp_f24_2;
    f32 temp_f24_3;
    f32 temp_f26;
    f32 temp_f26_2;
    f32 temp_f26_3;
    f32 temp_f26_4;
    f32 temp_f2;
    f32 temp_f30;
    f32 var_f12;
    f32 var_f20;
    f32 var_f22;
    f32 var_f2;
    f32 var_f30;
    f32 var_f30_2;
    f32 var_f30_3;
    f32 var_f30_4;
    s16 temp_a0_2;
    s16 temp_a0_3;
    s16 temp_a0_4;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_t1;
    s32 temp_t6_2;
    s32 temp_t9;
    s32 temp_v0_3;
    s32 temp_v1_2;
    s32 var_a3;
    s32 var_s4;
    s8 *var_v1;
    s8 temp_t3;
    s8 temp_t4;
    s8 var_a3_3;
    u16 *temp_a0;
    u16 *var_a0;
    u16 *var_a0_2;
    u16 temp_t8;
    u32 temp_t6;
    u32 var_a3_2;
    void *temp_a1_2;
    void *temp_t0;
    void *temp_t2;
    void *temp_v0;
    void *temp_v0_2;
    void *temp_v0_4;
    void *temp_v0_5;
    void *temp_v0_6;
    void *temp_v0_7;
    void *temp_v1;
    void *temp_v1_3;
    void *temp_v1_4;
    void *temp_v1_5;
    void *temp_v1_6;
    void *temp_v1_7;
    void *temp_v1_8;
    void *var_s5;
    void *var_v0_2;
    void *var_v1_2;
    void *var_v1_3;

    sp114 = 0;
    temp_t6 = arg1->unk2 & 0xF;
    func_800AD5D0(arg1->unk16 + *(s32 *)0x80152568, temp_t6, &sp238, arg1);
    temp_v1 = (sp238 * 8) + *(s32 *)0x8015201C;
    sp228 = (f32) ((temp_v1->unk0 << 5) + ((s32) (temp_v1->unk6 & 0x7C00) >> 0xA)) * 0.03125f;
    sp22C = (f32) ((temp_v1->unk2 << 5) + ((s32) (temp_v1->unk6 & 0x3E0) >> 5)) * 0.03125f;
    sp230 = (f32) ((temp_v1->unk4 << 5) + (temp_v1->unk6 & 0x1F)) * 0.03125f;
    func_800AD650(&sp118, arg1 + 4, sp238);
    var_v1 = (s8 *)0x801174C4;
    var_t0 = &sp1BC;
    do {
        temp_v0 = arg0 + (*var_v1 * 0xC);
        sp21C = temp_v0->unk274 - sp228;
        sp220 = temp_v0->unk278 - sp22C;
        sp94 = var_t0;
        sp98 = var_v1;
        sp224 = temp_v0->unk27C - sp230;
        func_800A61B0(&sp21C, var_t0, &sp118);
        var_t0 = sp94 + 0xC;
        var_v1 = sp98 + 1;
    } while ((u32) var_t0 < (u32) &sp1EC);
    temp_t9 = arg1->unk0 & 0xF;
    if ((temp_t9 == 5) || (temp_t9 == 6)) {
        var_f20 = -3.0f;
    } else {
        if (((arg0->unk300 * sp12C) + ((sp124 * arg0->unk2F8) + (sp128 * arg0->unk2FC))) < *(f32 *)0x80123F8C) {
            var_f20 = -4.5f;
        } else {
            var_f20 = -0.5f;
        }
        spEC = var_f20;
    }
    func_800A61B0(arg0 + 0x2F8, &sp21C, &sp118);
    temp_f30 = sp21C * var_f20;
    temp_f26 = sp220 * var_f20;
    temp_f24 = sp224 * var_f20;
    sp1EC = temp_f30 + sp1BC;
    sp1F4 = temp_f24 + sp1C4;
    sp1F0 = temp_f26 + sp1C0;
    sp1F8 = temp_f30 + sp1C8;
    sp1FC = temp_f26 + sp1CC;
    sp200 = temp_f24 + sp1D0;
    sp204 = temp_f30 + sp1D4;
    sp208 = temp_f26 + sp1D8;
    sp210 = temp_f30 + sp1E0;
    sp20C = temp_f24 + sp1DC;
    sp100 = 0.0f;
    sp214 = temp_f26 + sp1E4;
    spDC = -1;
    var_s4 = 0;
    sp218 = temp_f24 + sp1E8;
    var_s5 = (void *)0x801174CC;
loop_10:
    temp_t3 = var_s5->unk0;
    temp_t4 = var_s5->unk8;
    temp_t0 = &sp1BC + (temp_t3 * 0xC);
    temp_f16 = temp_t0->unk4;
    temp_t2 = &sp1BC + (temp_t4 * 0xC);
    temp_f18 = temp_t2->unk4;
    if ((temp_f16 * temp_f18) < 0.0f) {
        temp_v1_2 = temp_t6 - 1;
        if (sp114 == 0) {
            sp114 = 1;
            var_a3 = temp_v1_2;
            if (temp_v1_2 > 0) {
                temp_t1 = -(temp_v1_2 & 3);
                temp_a1 = *(void *)0x8015201C;
                if (temp_t1 != 0) {
                    var_v0 = (temp_v1_2 * 8) + &sp13C;
                    var_a0 = &(&sp238)[temp_v1_2];
                    temp_a2 = temp_t1 + temp_v1_2;
                    var_a3 -= 1;
                    var_v1_2 = (*var_a0 * 8) + temp_a1;
                    var_f30 = (f32) ((var_v1_2->unk0 << 5) + ((s32) (var_v1_2->unk6 & 0x7C00) >> 0xA)) * 0.03125f;
                    if (temp_a2 != var_a3) {
                        do {
                            *var_v0 = var_f30;
                            var_a3 -= 1;
                            temp_t8 = var_a0->unk-2;
                            var_a0 -= 2;
                            temp_f26_2 = (f32) ((var_v1_2->unk4 << 5) + (var_v1_2->unk6 & 0x1F));
                            var_v1_2 = (temp_t8 * 8) + temp_a1;
                            var_v0 -= 8;
                            var_v0->unkC = (f32) (temp_f26_2 * 0.03125f);
                            var_f30 = (f32) ((var_v1_2->unk0 << 5) + ((s32) (var_v1_2->unk6 & 0x7C00) >> 0xA)) * 0.03125f;
                        } while (temp_a2 != var_a3);
                    }
                    *var_v0 = var_f30;
                    (var_v0 - 8)->unkC = (f32) ((f32) ((var_v1_2->unk4 << 5) + (var_v1_2->unk6 & 0x1F)) * 0.03125f);
                    if (var_a3 != 0) {
                        goto block_17;
                    }
                } else {
block_17:
                    var_a0_2 = &(&sp238)[var_a3];
                    var_v0_2 = ((var_a3 * 8) + &sp13C) - 0x20;
                    var_v1_3 = (*var_a0_2 * 8) + temp_a1;
                    var_f30_2 = (f32) ((var_v1_3->unk0 << 5) + ((s32) (var_v1_3->unk6 & 0x7C00) >> 0xA)) * 0.03125f;
                    if (var_v0_2 != &sp13C) {
                        do {
                            var_v0_2->unk20 = var_f30_2;
                            var_v0_2 -= 0x20;
                            temp_v1_3 = (var_a0_2->unk-2 * 8) + temp_a1;
                            var_a0_2 -= 8;
                            var_v0_2->unk44 = (f32) ((f32) ((var_v1_3->unk4 << 5) + (var_v1_3->unk6 & 0x1F)) * 0.03125f);
                            var_v0_2->unk38 = (f32) ((f32) ((temp_v1_3->unk0 << 5) + ((s32) (temp_v1_3->unk6 & 0x7C00) >> 0xA)) * 0.03125f);
                            temp_v1_4 = (var_a0_2->unk4 * 8) + temp_a1;
                            var_v0_2->unk3C = (f32) ((f32) ((temp_v1_3->unk4 << 5) + (temp_v1_3->unk6 & 0x1F)) * 0.03125f);
                            var_v0_2->unk30 = (f32) ((f32) ((temp_v1_4->unk0 << 5) + ((s32) (temp_v1_4->unk6 & 0x7C00) >> 0xA)) * 0.03125f);
                            temp_v1_5 = (var_a0_2->unk2 * 8) + temp_a1;
                            var_v0_2->unk34 = (f32) ((f32) ((temp_v1_4->unk4 << 5) + (temp_v1_4->unk6 & 0x1F)) * 0.03125f);
                            var_v0_2->unk28 = (f32) ((f32) ((temp_v1_5->unk0 << 5) + ((s32) (temp_v1_5->unk6 & 0x7C00) >> 0xA)) * 0.03125f);
                            var_v1_3 = (var_a0_2->unk0 * 8) + temp_a1;
                            var_v0_2->unk2C = (f32) ((f32) ((temp_v1_5->unk4 << 5) + (temp_v1_5->unk6 & 0x1F)) * 0.03125f);
                            var_f30_2 = (f32) ((var_v1_3->unk0 << 5) + ((s32) (var_v1_3->unk6 & 0x7C00) >> 0xA)) * 0.03125f;
                        } while (var_v0_2 != &sp13C);
                    }
                    var_v0_2->unk20 = var_f30_2;
                    temp_a0 = var_a0_2 - 8;
                    temp_v1_6 = (temp_a0->unk6 * 8) + temp_a1;
                    var_v0_2->unk24 = (f32) ((f32) ((var_v1_3->unk4 << 5) + (var_v1_3->unk6 & 0x1F)) * 0.03125f);
                    var_v0_2->unk18 = (f32) ((f32) ((temp_v1_6->unk0 << 5) + ((s32) (temp_v1_6->unk6 & 0x7C00) >> 0xA)) * 0.03125f);
                    temp_v1_7 = (temp_a0->unk4 * 8) + temp_a1;
                    var_v0_2->unk1C = (f32) ((f32) ((temp_v1_6->unk4 << 5) + (temp_v1_6->unk6 & 0x1F)) * 0.03125f);
                    var_v0_2->unk10 = (f32) ((f32) ((temp_v1_7->unk0 << 5) + ((s32) (temp_v1_7->unk6 & 0x7C00) >> 0xA)) * 0.03125f);
                    temp_v1_8 = (temp_a0->unk2 * 8) + temp_a1;
                    var_v0_2->unk14 = (f32) ((f32) ((temp_v1_7->unk4 << 5) + (temp_v1_7->unk6 & 0x1F)) * 0.03125f);
                    var_v0_2->unk8 = (f32) ((f32) ((temp_v1_8->unk0 << 5) + ((s32) (temp_v1_8->unk6 & 0x7C00) >> 0xA)) * 0.03125f);
                    var_v0_2->unkC = (f32) ((f32) ((temp_v1_8->unk4 << 5) + (temp_v1_8->unk6 & 0x1F)) * 0.03125f);
                }
            }
        }
        temp_f12 = temp_t0->unk0;
        temp_f14 = temp_t0->unk8;
        temp_f20 = temp_t2->unk0 - temp_f12;
        temp_f26_3 = temp_t2->unk8 - temp_f14;
        if (temp_f18 != temp_f16) {
            temp_f24_2 = temp_f18 - temp_f16;
            var_f22 = 0.0f;
            spF0 = temp_f20;
            var_f2 = sp144;
            spFC = temp_f16;
            spF8 = temp_f18;
            spEC = temp_f26_3;
            temp_f0 = fabsf(temp_f16 / temp_f24_2);
            spCC = temp_f12 + (temp_f20 * temp_f0);
            spC8 = temp_f14 + (temp_f26_3 * temp_f0);
            if (temp_f24_2 > 0.0f) {
                var_f30_3 = temp_f0;
                spF0 = -spF0;
                spEC = -spEC;
                sp94 = temp_f24_2;
            } else {
                sp94 = temp_f24_2;
                var_f30_3 = 1.0f - temp_f0;
            }
            var_f12 = 0.0f;
            if (!((-var_f2 * spC8) > 0.0f)) {
                var_a3_2 = 2;
                var_v0_3 = &sp14C;
                if (spEC != 0.0f) {
                    temp_f0_2 = -spC8 / spEC;
                    if ((temp_f0_2 > 0.0f) && (temp_f0_2 < var_f30_3)) {
                        var_f30_3 = temp_f0_2;
                    }
                }
                if (temp_t6 >= 3U) {
loop_30:
                    temp_f26_4 = var_v0_3->unk0;
                    temp_f24_3 = var_v0_3->unk4;
                    temp_f16_2 = temp_f26_4 - var_f2;
                    var_a3_2 += 1;
                    var_v0_3 += 8;
                    temp_f14_2 = temp_f24_3 - var_f22;
                    temp_f20_2 = temp_f16_2 * (spC8 - var_f22);
                    temp_f18_2 = (spCC - var_f2) * temp_f14_2;
                    if (!(temp_f20_2 < temp_f18_2)) {
                        temp_f0_3 = temp_f16_2 * spEC;
                        temp_f2 = temp_f14_2 * spF0;
                        var_f2 = temp_f26_4;
                        if (temp_f2 != temp_f0_3) {
                            var_f12 = (temp_f18_2 - temp_f20_2) / (temp_f0_3 - temp_f2);
                            if ((var_f12 > 0.0f) && (var_f12 < var_f30_3)) {
                                var_f30_3 = var_f12;
                            }
                        }
                        var_f22 = temp_f24_3;
                        if (var_a3_2 >= temp_t6) {
                            goto block_36;
                        }
                        goto loop_30;
                    }
                    goto block_69;
                }
block_36:
                temp_f16_3 = var_f22 * spCC;
                temp_f18_3 = var_f2 * spC8;
                if (!(temp_f16_3 < temp_f18_3)) {
                    temp_f0_4 = var_f2 * spEC;
                    temp_f14_3 = var_f22 * spF0;
                    temp_t6_2 = arg1->unk0 & 0xF;
                    if (temp_f14_3 != temp_f0_4) {
                        var_f12 = (temp_f16_3 - temp_f18_3) / (temp_f0_4 - temp_f14_3);
                        if ((var_f12 > 0.0f) && (var_f12 < var_f30_3)) {
                            var_f30_3 = var_f12;
                        }
                    }
                    if (temp_t6_2 == 4) {
                        temp_a0_2 = arg0->unk7C6;
                        temp_v0_2 = (temp_a0_2 * 0x3B8) + 0x80152818;
                        if ((temp_v0_2->unk359 == 0) && (temp_v0_2->unk358 == 0) && (arg0->unk6C4 == -1)) {
                            temp_v0_3 = *(s32 *)0x8014A110;
                            if ((temp_v0_3 == 6) || (temp_v0_3 == 4)) {
                                sp110 = (s32) temp_t3;
                                sp10C = (s32) temp_t4;
                                func_803914B4(var_f12, temp_f14_3, (s8) temp_a0_2, (s8) temp_a0_2, -1, (s8) temp_a0_2);
                            }
                            arg0->unk640 = 1;
                        }
                        goto block_62;
                    }
                    if (temp_t6_2 == 7) {
                        temp_a0_3 = arg0->unk7C6;
                        temp_v0_4 = (temp_a0_3 * 0x3B8) + 0x80152818;
                        if ((temp_v0_4->unk359 == 0) && (temp_v0_4->unk358 == 0) && (arg0->unk6C4 == -1)) {
                            arg0->unk6CD = 1;
                            sp10C = (s32) temp_t4;
                            sp110 = (s32) temp_t3;
                            func_800C54F0(var_f12, temp_f14_3, temp_a0_3, 1);
                        }
                        goto block_62;
                    }
                    if (temp_t6_2 == 3) {
                        temp_a0_4 = arg0->unk7C6;
                        temp_v0_5 = (temp_a0_4 * 0x3B8) + 0x80152818;
                        if ((temp_v0_5->unk359 == 0) && (temp_v0_5->unk358 == 0) && (arg0->unk6C4 == -1)) {
                            arg0->unk6CD = 1;
                            func_800C54F0(var_f12, temp_f14_3, temp_a0_4, 1);
                            if (((arg0->unk7C6 * 8) + 0x80150000)->unk3E8F == 6) {
                                if (*(s8 *)0x8010FFC0 == 0) {
                                    return;
                                }
                                entity_flags_apply(0x15, 0, 1, 2);
                            }
                        }
                    } else {
block_62:
                        var_f30_4 = var_f30_3 * sp94;
                        if (var_f30_4 < 0.0f) {
                            var_f30_4 = -var_f30_4;
                        }
                        if (sp100 < var_f30_4) {
                            var_a3_3 = temp_t4;
                            if ((-spFC / spF8) < 1.0f) {
                                var_a3_3 = temp_t3;
                            }
                            temp_v0_6 = arg0 + ((var_a3_3 + 0x80110000)->unk74C4 * 0xC);
                            sp228 = temp_v0_6->unk274 - temp_v0_6->unk2A4;
                            sp22C = temp_v0_6->unk278 - temp_v0_6->unk2A8;
                            sp108 = (s32) var_a3_3;
                            sp230 = temp_v0_6->unk27C - temp_v0_6->unk2AC;
                            func_800A61B0(&sp228, &sp21C, &sp118, (s32) var_a3_3);
                            if (!(sp220 >= 0.0f)) {
                                sp100 = var_f30_4;
                                spD8 = var_s4;
                                spDC = sp108;
                            }
                        }
                        goto block_69;
                    }
                } else {
                    goto block_69;
                }
            } else {
                goto block_69;
            }
        } else {
            goto block_69;
        }
    } else {
block_69:
        var_s4 += 1;
        var_s5 += 1;
        if (var_s4 >= 8) {
            if (spDC >= 0) {
                temp_v0_7 = arg2->unk0;
                if (temp_v0_7 != NULL) {
                    temp_a1_2 = arg2 + 4;
                    if (arg2->unk2C < sp100) {
                        func_800C36A0(&sp118, temp_a1_2);
                        arg2->unk0 = NULL;
                        camera_play_script(arg0, arg1, arg2);
                        return;
                    }
                    arg2->unk0 = arg1;
                    math_utility(&sp118, temp_a1_2);
                    arg2->unk28 = (s16) spDC;
                    arg2->unk2A = (s16) spD8;
                    arg2->unk2C = sp100;
                    spBC = temp_v0_7;
                    func_800C36A0();
                    arg2->unk0 = NULL;
                    camera_play_script(arg0, spBC, arg2);
                    return;
                }
                arg2->unk0 = arg1;
                math_utility(&sp118, arg2 + 4);
                arg2->unk28 = (s16) spDC;
                arg2->unk2A = (s16) spD8;
                arg2->unk2C = sp100;
            }
        } else {
            goto loop_10;
        }
    }
}
