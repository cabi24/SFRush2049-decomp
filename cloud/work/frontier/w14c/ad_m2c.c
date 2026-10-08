void assign_drones(void) {
    s32 sp84;
    s32 sp80;
    s32 sp40;
    f32 *var_v0_2;
    f32 temp_f0;
    f32 temp_f16;
    f32 temp_f2;
    f32 temp_f4;
    f32 temp_f4_2;
    f32 temp_f6;
    f32 var_f10;
    s32 temp_s6;
    s32 temp_t8;
    s32 temp_v0_3;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_s1;
    s32 var_t0;
    s32 var_t1;
    s32 var_t6;
    s8 temp_v0;
    s8 var_ra;
    s8 var_s7;
    void **temp_v0_9;
    void **var_v0;
    void *temp_v0_2;
    void *temp_v0_4;
    void *temp_v0_5;
    void *temp_v0_6;
    void *temp_v0_7;
    void *temp_v0_8;
    void *temp_v1;
    void *temp_v1_2;
    void *temp_v1_3;
    void *temp_v1_4;
    void *temp_v1_5;
    void *var_a0;
    void *var_a0_2;
    void *var_s0;
    void *var_s4;
    void *var_t2;

    temp_v0 = *(s8 *)0x8014978C;
    var_ra = temp_v0;
    sp80 = (s32) temp_v0;
    if (*(s8 *)0x80152570 != 0) {
        var_ra = temp_v0 + 6;
        sp80 = temp_v0 + 0x13;
    }
    var_s4 = (void *)0x8014A118;
    var_s7 = 0;
    if (*(s16 *)0x8014A108 > 0) {
loop_4:
        var_v0 = var_s4->unk48;
        var_t1 = 0;
        if (var_v0 == NULL) {
            var_v0 = (var_s4->unk1 * 4) + 0x80146150;
            var_s4->unk48 = var_v0;
        }
        temp_s6 = var_ra * 0x60;
        if ((*var_v0)->unk2C != 0) {
            sp40 = (s32) unksp83;
            do {
                if (var_t1 == 0) {
                    var_s0 = *(*var_s4->unk48)->unk2C + temp_s6 + 0x8C;
                } else {
                    var_s0 = temp_s6 + 0x80150F88;
                }
                var_t0 = 0;
                var_s1 = 0;
                temp_v0_2 = (var_s4->unk0 * 0x3B8) + 0x80152818;
                var_t2 = var_s0;
                if (temp_v0_2->unkEF != 0) {
                    temp_f2 = temp_v0_2->unkF0;
loop_13:
                    temp_f0 = var_t2->unk2C;
                    if ((temp_f0 == 0.0f) || (temp_f2 < temp_f0)) {
                        var_a1 = 4;
                        if (var_t0 < 4) {
                            temp_v0_3 = -((4 - var_t0) & 3);
                            if (temp_v0_3 != 0) {
                                var_a2 = 4 * 4;
                                var_a0 = var_s0 + var_a2;
                                do {
                                    temp_f4 = var_a0->unk28;
                                    var_a0 -= 4;
                                    var_a0->unk30 = temp_f4;
                                    if (var_t1 == 1) {
                                        temp_v1 = (var_ra * 0x3C) + 0x80151690 + var_a2;
                                        temp_v0_4 = var_a1 + 0x80151AC0;
                                        temp_v0_4->unkA = (s8) temp_v0_4->unk9;
                                        temp_v1->unk28 = (s32) temp_v1->unk24;
                                    }
                                    var_a1 -= 1;
                                    var_a2 -= 4;
                                } while ((temp_v0_3 + 4) != var_a1);
                                if (var_t0 != var_a1) {
                                    goto block_22;
                                }
                            } else {
block_22:
                                var_a2_2 = var_a1 * 4;
                                var_a0_2 = var_s0 + var_a2_2;
                                do {
                                    temp_v0_5 = var_a1 + 0x80151AC0;
                                    var_a0_2->unk2C = (f32) var_a0_2->unk28;
                                    if (var_t1 == 1) {
                                        temp_v1_2 = (var_ra * 0x3C) + 0x80151690 + var_a2_2;
                                        temp_v0_5->unkA = (s8) temp_v0_5->unk9;
                                        temp_v1_2->unk28 = (s32) temp_v1_2->unk24;
                                    }
                                    temp_v0_6 = var_a1 + 0x80151AC0;
                                    var_a0_2->unk28 = (f32) var_a0_2->unk24;
                                    if (var_t1 == 1) {
                                        temp_v1_3 = (var_ra * 0x3C) + 0x80151690 + var_a2_2;
                                        temp_v1_3->unk24 = (s32) temp_v1_3->unk20;
                                        temp_v0_6->unk9 = (s8) temp_v0_6->unk8;
                                    }
                                    temp_v0_7 = var_a1 + 0x80151AC0;
                                    var_a0_2->unk24 = (f32) var_a0_2->unk20;
                                    if (var_t1 == 1) {
                                        temp_v1_4 = (var_ra * 0x3C) + 0x80151690 + var_a2_2;
                                        temp_v1_4->unk20 = (s32) temp_v1_4->unk1C;
                                        temp_v0_7->unk8 = (s8) temp_v0_7->unk7;
                                    }
                                    temp_f16 = var_a0_2->unk1C;
                                    var_a0_2 -= 0x10;
                                    var_a0_2->unk30 = temp_f16;
                                    if (var_t1 == 1) {
                                        temp_v1_5 = (var_ra * 0x3C) + 0x80151690 + var_a2_2;
                                        temp_v0_8 = var_a1 + 0x80151AC0;
                                        temp_v0_8->unk7 = (s8) temp_v0_8->unk6;
                                        temp_v1_5->unk1C = (s32) temp_v1_5->unk18;
                                    }
                                    var_a1 -= 4;
                                    var_a2_2 -= 0x10;
                                } while (var_t0 != var_a1);
                            }
                        }
                        var_t2->unk2C = temp_f2;
                        if (var_t1 == 1) {
                            (var_a1 + 0x80151AC0)->unkA = var_s7;
                            temp_v0_9 = var_s4->unk48;
                            if ((*temp_v0_9)->unk8 != 0) {
                                ((var_ra * 0x3C) + 0x80151690 + var_s1)->unk28 = temp_v0_9;
                            }
                        }
                    } else {
                        var_t0 += 1;
                        var_s1 += 4;
                        var_t2 += 4;
                        if (var_t0 != 5) {
                            goto loop_13;
                        }
                    }
                }
                var_t1 += 1;
                var_s0->unk46 = (u16) (var_s0->unk46 + 1);
                var_s0->unk54 = (u16) (var_s0->unk54 + 1);
                var_a1_2 = 0;
                var_s0->unk4E = (u16) (var_s0->unk4E + var_s4->unk40);
                var_s0->unk44 = (u16) (var_s0->unk44 + var_s7->unk80144018);
                if ((s32) var_s7->unk80144018 > 0) {
                    var_v0_2 = (var_s7 << 5) + 0x80149A78;
                    do {
                        temp_f4_2 = *var_v0_2;
                        var_a1_2 += 1;
                        var_v0_2 += 4;
                        var_s0->unk40 = (f32) (var_s0->unk40 + temp_f4_2);
                    } while (var_a1_2 < (s32) var_s7->unk80144018);
                }
                temp_t8 = var_s0->unk58;
                var_f10 = (f32) temp_t8;
                if (temp_t8 < 0) {
                    var_f10 += 4294967296.0f;
                }
                temp_f6 = var_f10 + (temp_v0_2->unk108 / 528.0f);
                if (M2C_ERROR(/* cfc1 */) & 0x78) {
                    if (!(M2C_ERROR(/* cfc1 */) & 0x78)) {
                        var_t6 = (s32) (temp_f6 - 2.1474836e9f) | 0x80000000;
                    } else {
                        goto block_44;
                    }
                } else {
                    var_t6 = (s32) temp_f6;
                    if (var_t6 < 0) {
block_44:
                        var_t6 = -1;
                    }
                }
                var_s0->unk58 = var_t6;
            } while (var_t1 != 2);
            sp84 = (s32) var_ra;
            (? (*)(?, ?, void **, u8))0x800CD8EC(0, 0x44040000, var_s4->unk48, unksp43);
            var_s7 += 1;
            var_s4 += 0x4C;
            if (var_s7 < *(void *)0x8014A108) {
                goto loop_4;
            }
        }
    }
}
