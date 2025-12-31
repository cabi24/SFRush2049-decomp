? func_80002790(u32 (*)[], ?, ?);                   /* extern */
s32 func_80007270(? *, ? *, ?);                     /* extern */
? func_80007C68(u8 *, u8 *, s32);                   /* extern */
? func_80008460(u8 *, s32);                         /* extern */
? func_800084E0(? *, ?, s32);                       /* extern */
? func_80008630(? *, ?, ?, s32, ? *, s32, ? *);     /* extern */
? func_800960D4(s32);                               /* extern */
s32 sound_play_menu(?, ?, u32);                       /* extern */
s32 func_800066D4();                                /* static */
extern s32 D_8002B040;
extern ? D_8002B08C;
extern ? D_8002B0CC;
extern ? D_8002B10C;
extern ? D_8002B148;
extern ? D_8002B184;
extern u32 D_8002B1A8;
extern u32 D_8002B1AC;
extern ? D_80035428;
extern s32 D_80035480;
extern ? D_80035488;
extern s32 D_800354A0;
extern u32 D_800354B0;
extern u32 D_800354B4;
extern u8 *D_800354B8;
extern u32 D_800354BC;
extern u32 D_800354C0;
extern s32 D_800354C4;
extern s32 D_800354C8;
extern s32 D_800354CC;
extern s32 D_800354D0;
extern s32 D_800354D4;
extern ? D_800354D8;
extern ? D_80084A50;
extern ? D_80085A50;

void func_80004A10(void) {
    ? sp30;
    ? *var_s0;

    if (func_80007270(&D_80035428, &sp30, 0) == -1) {
        do {

        } while (func_80007270(&D_80035428, &sp30, 0) == -1);
    }
    if (D_80035480 == 1) {
        var_s0 = &D_80085A50;
    } else {
        var_s0 = &D_80084A50;
    }
    func_800084E0(var_s0, 0x1000);
    func_80008630(&D_80035488, 0, 0, D_800354A0, var_s0, 0x1000, &D_80035428);
    D_800354A0 += 0x1000;
    D_80035480 ^= 1;
}

/* Warning: Gap in callee-saved word stack region.
 * Saved: [0x28, 0x2c, 0x30, 0x34, 0x38, 0x3c, 0x44, 0x4c], gap at: 0x48. */
s32 func_80004AFC(s32 arg0, void *arg1) {
    ? sp84;
    ? *sp80;
    s32 sp74;
    s32 sp60;
    s32 sp5C;
    s32 sp58;
    void *sp54;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_s5;
    s32 var_a1;
    s32 var_s5;
    s32 var_t0;
    s32 var_t0_2;
    u8 *var_a0;
    u8 temp_t8;
    u8 temp_v0;
    void *var_s0;
    void *var_s7;

    var_s7 = arg1;
    sp5C = 0;
    D_80035480 = 1;
    var_s0 = var_s7;
    sp80 = &D_80084A50;
    func_800084E0(&D_80084A50, 0x1000);
    func_80008630(&D_80035488, 0, 0, arg0, &D_80084A50, 0x1000, &D_80035428);
    D_800354A0 = arg0 + 0x1000;
    var_t0 = 0;
    var_a1 = 0;
loop_1:
    if (var_a1 == 0) {
        if (var_t0 <= 0) {
            sp54 = var_s0;
            func_80004A10();
            var_t0 = 0x1000;
        }
        var_t0 -= 1;
        var_a1 = 8;
    }
    var_a1 -= 1;
    if (M2C_ERROR(/* Read from unset register $a2 */) & 1) {
        if (var_t0 <= 0) {
            sp60 = var_a1;
            sp58 = M2C_ERROR(/* Read from unset register $a2 */);
            sp54 = var_s0;
            func_80004A10();
            var_t0 = 0x1000;
        }
        var_t0 -= 1;
        var_s7 += 1;
        var_s7->unk-1 = (u8) *M2C_ERROR(/* Read from unset register $t1 */);
block_20:
        goto loop_1;
    }
    if (var_t0 <= 0) {
        sp60 = var_a1;
        sp58 = M2C_ERROR(/* Read from unset register $a2 */);
        sp54 = var_s0;
        func_80004A10();
        var_t0 = 0x1000;
    }
    temp_v0 = *M2C_ERROR(/* Read from unset register $t1 */);
    var_t0_2 = var_t0 - 1;
    temp_a0 = (temp_v0 & 0xF0) * 0x10;
    temp_s5 = temp_v0 & 0xF;
    if (var_t0_2 <= 0) {
        sp74 = temp_a0;
        sp60 = var_a1;
        sp58 = M2C_ERROR(/* Read from unset register $a2 */);
        sp54 = var_s0;
        func_80004A10(temp_a0, var_a1);
        var_t0_2 = 0x1000;
    }
    var_t0 = var_t0_2 - 1;
    temp_a0_2 = (temp_a0 + *M2C_ERROR(/* Read from unset register $t1 */)) & 0xFFF;
    if (temp_a0_2 != 0) {
block_18:
        var_s5 = temp_s5 + 1;
        var_a0 = var_s7 - temp_a0_2;
        do {
            temp_t8 = *var_a0;
            var_s5 -= 1;
            var_s7 += 1;
            var_a0 += 1;
            var_s7->unk-1 = temp_t8;
        } while (var_s5 >= 0);
        goto block_20;
    }
    if (temp_s5 != 0) {
        goto block_18;
    }
    if (func_80007270(&D_80035428, &sp84, 0) == -1) {
        do {

        } while (func_80007270(&D_80035428, &sp84, 0) == -1);
    }
    return var_s7 - var_s0;
}

void func_80004D50(s32 arg0, s32 arg1) {
    D_800354C4 = arg0;
    D_800354CC = arg1;
    D_800354C8 = 0;
}

u32 *func_80004D6C(s32 arg0) {
    D_800354C8 += arg0;
    return (D_800354C8 - arg0) + D_800354C4;
}

s32 func_80004D98(u32 *arg0, u32 arg1, u32 arg2, ? *arg3, ? *arg4, u32 **arg5, u32 *arg6) {
    u32 sp16C;
    s32 sp168;
    s32 sp12C;
    u32 sp120;
    u32 *sp108;
    u32 *sp100;                                     /* compiler-managed */
    s8 spFD;
    s8 spFC;
    u32 *spBC;
    u32 *spB8;
    ? sp78;
    s32 sp74;
    s32 sp68;
    u32 *sp50;
    ? *var_a2;
    s32 *temp_t3_3;
    s32 *temp_t3_4;
    s32 *var_t3_2;
    s32 *var_v0_2;
    s32 *var_v0_3;
    s32 *var_v1;
    s32 *var_v1_4;
    s32 temp_a1;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_s1;
    s32 temp_s4;
    s32 temp_t9_2;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s0_2;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s2;
    s32 var_s3;
    s32 var_v0;
    s32 var_v1_2;
    s32 var_v1_3;
    s8 temp_a1_2;
    u32 **var_a3;
    u32 *temp_v0_12;
    u32 *temp_v0_2;
    u32 *temp_v0_3;
    u32 *temp_v0_4;
    u32 *temp_v0_5;
    u32 *temp_v0_6;
    u32 *temp_v0_7;
    u32 *temp_v1_2;
    u32 *temp_v1_3;
    u32 *var_a2_2;
    u32 *var_t3;
    u32 *var_t3_3;
    u32 *var_t3_4;
    u32 temp_a0;
    u32 temp_a0_2;
    u32 temp_a0_3;
    u32 temp_s0_4;
    u32 temp_s6;
    u32 temp_t0;
    u32 temp_t2;
    u32 temp_t6;
    u32 temp_t8;
    u32 temp_v0_10;
    u32 temp_v0_11;
    u32 temp_v0_13;
    u32 temp_v0_8;
    u32 temp_v0_9;
    u32 var_a1;
    u32 var_fp;
    u32 var_s0;
    u32 var_s0_3;
    u32 var_s0_4;
    u32 var_s0_5;
    u32 var_s1_3;
    u32 var_s1_4;
    u32 var_s4;
    u32 var_s4_2;
    u32 var_s4_3;
    u32 var_s4_4;
    u32 var_s5;
    u32 var_t0;
    u32 var_t4;
    void *temp_a2;
    void *temp_a2_2;
    void *temp_a2_3;
    void *temp_t3;
    void *temp_t3_2;
    void *temp_t8_2;
    void *temp_t9;

    func_80002790((u32 (*)[]) &sp128[0], 0, 0x44);
    temp_v0 = -(arg1 & 3);
    var_t3 = arg0;
    var_s4 = arg1;
    if (temp_v0 != 0) {
        do {
            temp_t6 = *var_t3;
            var_s4 -= 1;
            var_t3 += 4;
            temp_v0_2 = &(&sp128[0])[temp_t6];
            *temp_v0_2 += 1;
        } while ((temp_v0 + arg1) != var_s4);
        if (var_s4 != 0) {
            goto loop_4;
        }
    } else {
        do {
loop_4:
            temp_t3 = var_t3 + 4 + 4;
            temp_v0_3 = &(&sp128[0])[*var_t3];
            var_s4 -= 4;
            *temp_v0_3 += 1;
            temp_t3_2 = temp_t3 + 4;
            temp_v0_4 = &(&sp128[0])[temp_t3->unk-4];
            *temp_v0_4 += 1;
            var_t3 = temp_t3_2 + 4;
            temp_v0_5 = &(&sp128[0])[temp_t3_2->unk-4];
            *temp_v0_5 += 1;
            temp_v0_6 = &(&sp128[0])[var_t3->unk-4];
            *temp_v0_6 += 1;
        } while (var_s4 != 0);
    }
    if (arg1 == sp128[0]) {
        *arg5 = NULL;
        *arg6 = 0;
        return 0;
    }
    var_s0 = 1;
    var_v1 = &sp12C;
    var_s5 = *arg6;
loop_8:
    if (*var_v1 == 0) {
        var_s0 += 1;
        var_v1 += 4;
        if (var_s0 != 0x11) {
            goto loop_8;
        }
    }
    var_fp = var_s0;
    if (var_s5 < var_s0) {
        var_s5 = var_s0;
    }
    var_s4_2 = 0x10;
    var_v0_2 = &sp168;
loop_13:
    if (*var_v0_2 == 0) {
        var_s4_2 -= 1;
        var_v0_2 -= 4;
        if (var_s4_2 != 0) {
            goto loop_13;
        }
    }
    if (var_s4_2 < var_s5) {
        var_s5 = var_s4_2;
    }
    var_s1 = 1 << var_s0;
    *arg6 = var_s5;
    if (var_s0 < var_s4_2) {
loop_18:
        temp_s1 = var_s1 - *var_v1;
        if (temp_s1 < 0) {
            return 2;
        }
        var_s0 += 1;
        var_s1 = temp_s1 * 2;
        var_v1 += 4;
        if (var_s0 >= var_s4_2) {
            goto block_21;
        }
        goto loop_18;
    }
block_21:
    temp_v1 = *var_v0_2;
    var_s1_2 = var_s1 - temp_v1;
    if (var_s1_2 < 0) {
        return 2;
    }
    *var_v0_2 = temp_v1 + var_s1_2;
    temp_s4 = var_s4_2 - 1;
    sp74 = 0;
    var_s0_2 = 0;
    var_t3_2 = &sp12C;
    var_a2 = &sp78;
    var_v0_3 = var_v0_2 - 4;
    if (temp_s4 != 0) {
        temp_a1 = -(temp_s4 & 3);
        if (temp_a1 != 0) {
            do {
                var_v0_3 -= 4;
                var_a2 += 4;
                var_s0_2 += *var_t3_2;
                var_a2->unk-4 = var_s0_2;
                var_t3_2 += 4;
            } while (&(&sp128[0])[temp_a1 + temp_s4] != var_v0_3);
            if (var_v0_3 != &sp128[0]) {
                goto block_28;
            }
        } else {
block_28:
            do {
                temp_a2 = var_a2 + 4;
                temp_t3_3 = var_t3_2 + 4;
                temp_s0 = var_s0_2 + var_t3_2->unk0;
                temp_a2->unk-4 = temp_s0;
                temp_a2_2 = temp_a2 + 4;
                temp_t3_4 = temp_t3_3 + 4;
                temp_s0_2 = temp_s0 + var_t3_2->unk4;
                temp_a2_2->unk-4 = temp_s0_2;
                temp_a2_3 = temp_a2_2 + 4;
                temp_s0_3 = temp_s0_2 + temp_t3_3->unk4;
                temp_a2_3->unk-4 = temp_s0_3;
                var_v0_3 -= 0x10;
                var_a2 = temp_a2_3 + 4;
                var_s0_2 = temp_s0_3 + temp_t3_4->unk4;
                var_a2->unk-4 = var_s0_2;
                var_t3_2 = temp_t3_4 + 4 + 4;
            } while (var_v0_3 != &sp128[0]);
        }
    }
    sp120 = var_s4_2;
    temp_v0_7 = func_80004D6C(0x480);
    var_t4 = var_s4_2;
    spB8 = temp_v0_7;
    var_t3_3 = arg0;
    var_s4_3 = 0;
    do {
        temp_s0_4 = *var_t3_3;
        sp68 = var_s1_2;
        var_t3_3 += 4;
        if (temp_s0_4 != 0) {
            temp_v1_2 = &(&sp70[0])[temp_s0_4];
            temp_v0_7[*temp_v1_2] = var_s4_3;
            *temp_v1_2 += 1;
        }
        var_s4_3 += 1;
        var_s1_2 = sp68;
    } while (var_s4_3 < arg1);
    var_s4_4 = 0;
    sp70[0] = 0;
    var_t3_4 = temp_v0_7;
    var_s3 = -1;
    var_s2 = -(s32) var_s5;
    spBC = NULL;
    var_a2_2 = NULL;
    var_s1_3 = 0;
    if ((s32) var_t4 >= (s32) var_fp) {
        sp50 = &(&sp128[0])[var_fp];
        var_a3 = arg5;
loop_36:
        temp_t0 = *sp50;
        var_a1 = temp_t0;
        var_t0 = temp_t0 - 1;
        if (var_a1 != 0) {
loop_37:
            var_v1_2 = var_s2 + var_s5;
            if (var_v1_2 < (s32) var_fp) {
                temp_s6 = var_t0 + 1;
loop_39:
                var_s2 = var_v1_2;
                temp_v0_8 = var_t4 - var_s2;
                var_s1_4 = temp_v0_8;
                var_s3 += 1;
                if (var_s5 < temp_v0_8) {
                    var_s1_4 = var_s5;
                }
                temp_v0_9 = var_fp - var_s2;
                temp_a0 = 1 << temp_v0_9;
                var_s0_3 = temp_v0_9;
                if (temp_s6 < temp_a0) {
                    var_s0_3 = temp_v0_9 + 1;
                    var_a2_2 = &sp128[0] + (var_fp * 4);
                    var_v1_3 = (temp_a0 - var_t0) - 1;
                    if (var_s0_3 < var_s1_4) {
loop_43:
                        temp_a0_2 = var_a2_2->unk4;
                        temp_v0_10 = var_v1_3 * 2;
                        var_a1 = temp_v0_10;
                        var_a2_2 += 4;
                        if (temp_a0_2 < var_a1) {
                            var_s0_3 += 1;
                            var_v1_3 = temp_v0_10 - temp_a0_2;
                            if (var_s0_3 < var_s1_4) {
                                goto loop_43;
                            }
                        }
                    }
                }
                temp_v0_11 = 1 << var_s0_3;
                var_s1_3 = temp_v0_11;
                arg5 = var_a3;
                sp16C = var_t0;
                sp108 = var_t3_4;
                sp120 = var_t4;
                temp_v0_12 = func_80004D6C((temp_v0_11 * 8) + 8, var_a1, var_a2_2, var_a3);
                if (temp_v0_12 == NULL) {
                    return 3;
                }
                temp_v1_3 = temp_v0_12 + 8;
                *arg5 = temp_v1_3;
                var_a1 = (u32) &(&spBC)[var_s3];
                temp_v0_12->unk4 = 0;
                var_a2_2 = temp_v1_3;
                var_a1->unk0 = var_a2_2;
                var_a3 = (u32 **) (temp_v0_12 + 4);
                if (var_s3 != 0) {
                    (&sp70[0])[var_s3] = var_s4_4;
                    spFC = var_s0_3 + 0x10;
                    sp100 = var_a2_2;
                    spFD = (s8) var_s5;
                    temp_t9 = var_a1->unk-4 + ((var_s4_4 >> (var_s2 - var_s5)) * 8);
                    temp_t9->unk0 = (s32) spFC.unk0;
                    temp_t9->unk4 = (s32) spFC.unk4;
                }
                var_v1_2 = var_s2 + var_s5;
                if (var_v1_2 >= (s32) var_fp) {
                    goto block_50;
                }
                goto loop_39;
            }
block_50:
            var_v1_4 = &sp70[0] + (var_s3 * 4);
            temp_t2 = 1 << (var_fp + 0x1F);
            temp_a1_2 = var_fp - var_s2;
            spFD = temp_a1_2;
            if ((u32) var_t3_4 >= (u32) &spB8[arg1]) {
                spFC = 0x63;
            } else {
                temp_a0_3 = *var_t3_4;
                if (temp_a0_3 < arg2) {
                    if (temp_a0_3 < 0x100U) {
                        spFC = 0x10;
                    } else {
                        spFC = 0xF;
                    }
                    temp_t8 = *var_t3_4;
                    var_t3_4 += 4;
                    sp100 = (s16) temp_t8;
                } else {
                    temp_t9_2 = arg2 * 2;
                    var_t3_4 += 4;
                    spFC = (s8) *(arg4 + (temp_a0_3 * 2) + -temp_t9_2);
                    sp100 = *(arg3 + (var_t3_4->unk-4 * 2) + -temp_t9_2);
                }
            }
            temp_v0_13 = var_s4_4 >> var_s2;
            var_s0_4 = temp_v0_13;
            if (temp_v0_13 < var_s1_3) {
                do {
                    temp_t8_2 = var_a2_2 + (var_s0_4 * 8);
                    temp_t8_2->unk0 = (s32) spFC.unk0;
                    var_s0_4 += 1 << temp_a1_2;
                    temp_t8_2->unk4 = (s32) spFC.unk4;
                } while (var_s0_4 < var_s1_3);
            }
            var_s0_5 = temp_t2;
            if (var_s4_4 & temp_t2) {
                do {
                    var_s4_4 ^= var_s0_5;
                    var_s0_5 = var_s0_5 >> 1;
                } while (var_s4_4 & var_s0_5);
            }
            var_s4_4 ^= var_s0_5;
            if ((var_s4_4 & ((1 << var_s2) - 1)) != *var_v1_4) {
                do {
                    var_s2 -= var_s5;
                    var_s3 -= 1;
                    var_v1_4 -= 4;
                } while ((var_s4_4 & ((1 << var_s2) - 1)) != var_v1_4->unk-4);
            }
            var_a1 = var_t0;
            var_t0 -= 1;
            if (var_a1 == 0) {
                goto block_66;
            }
            goto loop_37;
        }
block_66:
        var_fp += 1;
        sp50 += 4;
        if ((s32) var_t4 < (s32) var_fp) {
            arg5 = var_a3;
            goto block_68;
        }
        goto loop_36;
    }
block_68:
    var_v0 = sp68 != 0;
    if (var_v0 != 0) {
        var_v0 = var_t4 != 1;
    }
    return var_v0;
}

s32 func_80005464(u32 *arg0, u32 *arg1, u32 arg2, u32 arg3) {
    s32 sp48;
    s32 sp44;
    s32 temp_s6;
    s32 temp_t0;
    s32 temp_t0_2;
    s32 temp_t1;
    s32 temp_t2;
    s32 temp_t7;
    s32 temp_t9;
    s32 temp_v0_7;
    s32 temp_v1;
    s32 var_s6;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s32 var_v1_5;
    s32 var_v1_6;
    u32 temp_s3;
    u32 temp_s3_2;
    u32 var_s0;
    u32 var_s0_2;
    u32 var_s0_3;
    u32 var_s2;
    u32 var_s2_2;
    u32 var_s2_3;
    u8 *temp_v0;
    u8 *temp_v0_4;
    u8 *var_s5;
    u8 *var_s5_2;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_5;
    u8 temp_v0_6;
    u8 var_s3;
    u8 var_s3_2;

    var_s2 = D_800354BC;
    var_s0 = D_800354C0;
    sp44 = (s32) *(&D_8002B184 + (arg3 * 2));
    sp48 = (s32) *(&D_8002B184 + (arg2 * 2));
loop_1:
    if (var_s0 < arg2) {
        do {
            if ((u32) D_800354B0 < (u32) D_800354B4) {
                D_800354B0 += 2;
                var_v1 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
            } else {
                var_v1 = func_800066D4();
            }
            temp_t0 = var_v1 << var_s0;
            var_s0 += 0x10;
            var_s2 |= temp_t0;
        } while (var_s0 < arg2);
    }
    temp_v0 = ((var_s2 & sp48) * 8) + arg0;
    var_s3 = *temp_v0;
    var_s5 = temp_v0;
    if (var_s3 >= 0x11U) {
loop_7:
        if (var_s3 == 0x63) {
            return 1;
        }
        temp_v0_2 = var_s5->unk1;
        temp_s3 = var_s3 - 0x10;
        var_s0 -= temp_v0_2;
        var_s2 = var_s2 >> temp_v0_2;
        if (var_s0 < temp_s3) {
            do {
                if ((u32) D_800354B0 < (u32) D_800354B4) {
                    D_800354B0 += 2;
                    var_v1_2 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
                } else {
                    var_v1_2 = func_800066D4();
                }
                temp_t1 = var_v1_2 << var_s0;
                var_s0 += 0x10;
                var_s2 |= temp_t1;
            } while (var_s0 < temp_s3);
        }
        var_s5 = ((*(&D_8002B184 + (temp_s3 * 2)) & var_s2) * 8) + var_s5->unk4;
        var_s3 = *var_s5;
        if (var_s3 < 0x11U) {
            goto block_15;
        }
        goto loop_7;
    }
block_15:
    temp_v0_3 = var_s5->unk1;
    var_s2 = var_s2 >> temp_v0_3;
    var_s0 -= temp_v0_3;
    if (var_s3 == 0x10) {
        *D_800354B8 = (u8) var_s5->unk4;
        D_800354B8 += 1;
        goto loop_1;
    }
    if (var_s3 != 0xF) {
        if (var_s0 < var_s3) {
            do {
                if ((u32) D_800354B0 < (u32) D_800354B4) {
                    D_800354B0 += 2;
                    var_v1_3 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
                } else {
                    var_v1_3 = func_800066D4();
                }
                temp_t7 = var_v1_3 << var_s0;
                var_s0 += 0x10;
                var_s2 |= temp_t7;
            } while (var_s0 < var_s3);
        }
        var_s0_2 = var_s0 - var_s3;
        var_s2_2 = var_s2 >> var_s3;
        temp_s6 = (*(&D_8002B184 + (var_s3 * 2)) & var_s2) + var_s5->unk4;
        if (var_s0_2 < arg3) {
            do {
                if ((u32) D_800354B0 < (u32) D_800354B4) {
                    D_800354B0 += 2;
                    var_v1_4 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
                } else {
                    var_v1_4 = func_800066D4();
                }
                temp_t9 = var_v1_4 << var_s0_2;
                var_s0_2 += 0x10;
                var_s2_2 |= temp_t9;
            } while (var_s0_2 < arg3);
        }
        temp_v0_4 = ((var_s2_2 & sp44) * 8) + arg1;
        var_s3_2 = *temp_v0_4;
        var_s5_2 = temp_v0_4;
        if (var_s3_2 >= 0x11U) {
loop_29:
            if (var_s3_2 == 0x63) {
                return 1;
            }
            temp_v0_5 = var_s5_2->unk1;
            temp_s3_2 = var_s3_2 - 0x10;
            var_s0_2 -= temp_v0_5;
            var_s2_2 = var_s2_2 >> temp_v0_5;
            if (var_s0_2 < temp_s3_2) {
                do {
                    if ((u32) D_800354B0 < (u32) D_800354B4) {
                        D_800354B0 += 2;
                        var_v1_5 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
                    } else {
                        var_v1_5 = func_800066D4();
                    }
                    temp_t0_2 = var_v1_5 << var_s0_2;
                    var_s0_2 += 0x10;
                    var_s2_2 |= temp_t0_2;
                } while (var_s0_2 < temp_s3_2);
            }
            var_s5_2 = ((*(&D_8002B184 + (temp_s3_2 * 2)) & var_s2_2) * 8) + var_s5_2->unk4;
            var_s3_2 = *var_s5_2;
            if (var_s3_2 < 0x11U) {
                goto block_37;
            }
            goto loop_29;
        }
block_37:
        temp_v0_6 = var_s5_2->unk1;
        var_s0_3 = var_s0_2 - temp_v0_6;
        var_s2_3 = var_s2_2 >> temp_v0_6;
        if (var_s0_3 < var_s3_2) {
            do {
                if ((u32) D_800354B0 < (u32) D_800354B4) {
                    D_800354B0 += 2;
                    var_v1_6 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
                } else {
                    var_v1_6 = func_800066D4();
                }
                temp_t2 = var_v1_6 << var_s0_3;
                var_s0_3 += 0x10;
                var_s2_3 |= temp_t2;
            } while (var_s0_3 < var_s3_2);
        }
        var_s0 = var_s0_3 - var_s3_2;
        temp_v0_7 = -(s32) var_s5_2->unk4 - (*(&D_8002B184 + (var_s3_2 * 2)) & var_s2_3);
        var_s2 = var_s2_3 >> var_s3_2;
        if (temp_v0_7 < -8) {
            func_80007C68(D_800354B8, &D_800354B8[temp_v0_7], temp_s6);
            D_800354B8 = &D_800354B8[temp_s6];
        } else {
            var_s6 = temp_s6 - 1;
            if (temp_s6 != 0) {
                do {
                    temp_v1 = var_s6;
                    var_s6 -= 1;
                    *D_800354B8 = D_800354B8[temp_v0_7];
                    D_800354B8 += 1;
                } while (temp_v1 != 0);
            }
        }
        goto loop_1;
    }
    D_800354BC = var_s2;
    D_800354C0 = var_s0;
    return 0;
}

s32 func_8000595C(void) {
    s32 temp_s5;
    s32 temp_t1;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s5;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    u32 var_s0;
    u32 var_s0_2;
    u32 var_s0_3;
    u32 var_s2;
    u32 var_s2_2;
    u32 var_s2_3;

    temp_v0 = D_800354C0 & 7;
    var_s0 = D_800354C0 - temp_v0;
    var_s2 = (u32) D_800354BC >> temp_v0;
    if (var_s0 < 0x10U) {
        do {
            if ((u32) D_800354B0 < (u32) D_800354B4) {
                D_800354B0 += 2;
                var_v1 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
            } else {
                var_v1 = func_800066D4();
            }
            temp_t1 = var_v1 << var_s0;
            var_s0 += 0x10;
            var_s2 |= temp_t1;
        } while (var_s0 < 0x10U);
    }
    var_s0_2 = var_s0 - 0x10;
    temp_s5 = var_s2 & 0xFFFF;
    var_s2_2 = var_s2 >> 0x10;
    if (var_s0_2 < 0x10U) {
        do {
            if ((u32) D_800354B0 < (u32) D_800354B4) {
                D_800354B0 += 2;
                var_v1_2 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
            } else {
                var_v1_2 = func_800066D4();
            }
            temp_t8 = var_v1_2 << var_s0_2;
            var_s0_2 += 0x10;
            var_s2_2 |= temp_t8;
        } while (var_s0_2 < 0x10U);
    }
    if (temp_s5 != (~var_s2_2 & 0xFFFF)) {
        return 1;
    }
    var_s5 = temp_s5 - 1;
    var_s2_3 = var_s2_2 >> 0x10;
    var_s0_3 = var_s0_2 - 0x10;
    if (temp_s5 != 0) {
        do {
            if (var_s0_3 < 8U) {
                do {
                    if ((u32) D_800354B0 < (u32) D_800354B4) {
                        D_800354B0 += 2;
                        var_v1_3 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
                    } else {
                        var_v1_3 = func_800066D4();
                    }
                    temp_t7 = var_v1_3 << var_s0_3;
                    var_s0_3 += 0x10;
                    var_s2_3 |= temp_t7;
                } while (var_s0_3 < 8U);
            }
            temp_v1 = var_s5;
            *D_800354B8 = (u8) var_s2_3;
            var_s2_3 = var_s2_3 >> 8;
            var_s5 -= 1;
            D_800354B8 += 1;
            var_s0_3 -= 8;
        } while (temp_v1 != 0);
    }
    D_800354BC = var_s2_3;
    D_800354C0 = var_s0_3;
    return 0;
}

s32 func_80005B7C(void) {
    u32 *sp40;
    u32 *sp3C;
    u32 sp38;
    u32 sp34;
    u32 *sp30;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_4;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_v1;
    u32 *temp_v0_3;
    u32 *var_v0;
    u32 *var_v1_2;

    temp_v0_3 = func_80004D6C(0x480);
    sp30 = temp_v0_3;
    var_a0 = 0;
    var_v1_2 = temp_v0_3;
    do {
        var_a0 += 4;
        var_v1_2->unk0 = 8;
        var_v1_2->unk4 = 8;
        var_v1_2->unk8 = 8;
        var_v1_2->unkC = 8;
        var_v1_2 += 0x10;
    } while (var_a0 != 0x90);
    if (var_a0 < 0x100) {
        do {
            var_a0 += 1;
            *var_v1_2 = 9;
            var_v1_2 += 4;
        } while (var_a0 < 0x100);
    }
    if (var_a0 < 0x118) {
        do {
            var_a0 += 1;
            *var_v1_2 = 7;
            var_v1_2 += 4;
        } while (var_a0 < 0x118);
    }
    if (var_a0 < 0x120) {
        do {
            var_a0 += 1;
            *var_v1_2 = 8;
            var_v1_2 += 4;
        } while (var_a0 < 0x120);
    }
    sp38 = 7;
    temp_v0_2 = func_80004D98(sp30, 0x120U, 0x101U, &D_8002B08C, &D_8002B0CC, &sp40, &sp38);
    if (temp_v0_2 != 0) {
        D_800354C8 = 0;
        return temp_v0_2;
    }
    var_a0_2 = 0;
    var_v0 = sp30;
    do {
        var_a0_2 += 1;
        *var_v0 = 5;
        var_v0 += 4;
    } while (var_a0_2 < 0x1E);
    sp34 = 5;
    temp_v0 = func_80004D98(sp30, 0x1EU, 0U, &D_8002B10C, &D_8002B148, &sp3C, &sp34);
    if (temp_v0 >= 2) {
        D_800354C8 = 0;
        return temp_v0;
    }
    temp_v0_4 = func_80005464(sp40, sp3C, sp38, sp34);
    if (temp_v0_4 != 0) {
        if (temp_v0_4 < 0) {
            var_v1 = temp_v0_4;
        } else {
            var_v1 = 1;
        }
        return var_v1;
    }
    D_800354C8 = 0;
    return 0;
}

s32 func_80005D44(void) {
    s32 sp88;
    u32 sp84;
    u32 *sp80;
    u32 *sp7C;
    u32 sp78;
    u32 sp74;
    u32 sp6C;
    u32 sp68;
    s32 *var_s4;
    s32 *var_s4_2;
    s32 temp_at;
    s32 temp_at_2;
    s32 temp_at_3;
    s32 temp_s0_3;
    s32 temp_s0_4;
    s32 temp_s0_5;
    s32 temp_s6;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t3;
    s32 temp_t5;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    s32 var_v1_5;
    s32 var_v1_6;
    s32 var_v1_7;
    s32 var_v1_8;
    s32 var_v1_9;
    u16 temp_s0_2;
    u16 var_s6;
    u32 *temp_s7;
    u32 *temp_t0_2;
    u32 *var_v0;
    u32 *var_v0_2;
    u32 *var_v0_3;
    u32 temp_fp;
    u32 var_s0;
    u32 var_s1;
    u32 var_s1_2;
    u32 var_s1_3;
    u32 var_s1_4;
    u32 var_s3;
    u32 var_s3_2;
    u32 var_s3_3;
    u32 var_s3_4;
    u32 var_s4_3;
    u8 temp_s0;

    var_s1 = D_800354C0;
    var_s3 = D_800354BC;
    temp_s7 = func_80004D6C(0x4F0);
    if (var_s1 < 5U) {
        do {
            if ((u32) D_800354B0 < (u32) D_800354B4) {
                D_800354B0 += 2;
                var_v1_2 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
            } else {
                var_v1_2 = func_800066D4();
            }
            temp_t1 = var_v1_2 << var_s1;
            var_s1 += 0x10;
            var_s3 |= temp_t1;
        } while (var_s1 < 5U);
    }
    var_s1_2 = var_s1 - 5;
    var_s3_2 = var_s3 >> 5;
    sp6C = (var_s3 & 0x1F) + 0x101;
    if (var_s1_2 < 5U) {
        do {
            if ((u32) D_800354B0 < (u32) D_800354B4) {
                D_800354B0 += 2;
                var_v1_3 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
            } else {
                var_v1_3 = func_800066D4();
            }
            temp_t0 = var_v1_3 << var_s1_2;
            var_s1_2 += 0x10;
            var_s3_2 |= temp_t0;
        } while (var_s1_2 < 5U);
    }
    var_s1_3 = var_s1_2 - 5;
    var_s3_3 = var_s3_2 >> 5;
    sp68 = (var_s3_2 & 0x1F) + 1;
    if (var_s1_3 < 4U) {
        do {
            if ((u32) D_800354B0 < (u32) D_800354B4) {
                D_800354B0 += 2;
                var_v1_4 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
            } else {
                var_v1_4 = func_800066D4();
            }
            temp_t9 = var_v1_4 << var_s1_3;
            var_s1_3 += 0x10;
            var_s3_3 |= temp_t9;
        } while (var_s1_3 < 4U);
    }
    var_s3_4 = var_s3_3 >> 4;
    temp_s6 = (var_s3_3 & 0xF) + 4;
    var_s1_4 = var_s1_3 - 4;
    if ((sp6C >= 0x11FU) || (sp68 >= 0x1FU)) {
        return 1;
    }
    var_s0 = 0;
    if (temp_s6 != 0) {
        var_s4 = &D_8002B040;
        do {
            if (var_s1_4 < 3U) {
                do {
                    if ((u32) D_800354B0 < (u32) D_800354B4) {
                        D_800354B0 += 2;
                        var_v1_5 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
                    } else {
                        var_v1_5 = func_800066D4();
                    }
                    temp_t8 = var_v1_5 << var_s1_4;
                    var_s1_4 += 0x10;
                    var_s3_4 |= temp_t8;
                } while (var_s1_4 < 3U);
            }
            var_s0 += 1;
            temp_s7[*var_s4] = var_s3_4 & 7;
            var_s3_4 = var_s3_4 >> 3;
            var_s1_4 -= 3;
            var_s4 += 4;
        } while (var_s0 != temp_s6);
    }
    if (var_s0 < 0x13U) {
        var_s4_2 = &(&D_8002B040)[var_s0];
        do {
            temp_t6 = *var_s4_2;
            var_s4_2 += 4;
            temp_s7[temp_t6] = 0;
        } while ((u32) var_s4_2 < (u32) &D_8002B08C);
    }
    sp78 = 7;
    temp_v0_3 = func_80004D98(temp_s7, 0x13U, 0x13U, NULL, NULL, &sp80, &sp78);
    if (temp_v0_3 != 0) {
        D_800354C8 = 0;
        return temp_v0_3;
    }
    temp_fp = sp6C + sp68;
    sp84 = temp_fp;
    var_s6 = 0;
    var_s4_3 = 0;
    sp88 = (s32) *(&D_8002B184 + (sp78 * 2));
    if (temp_fp != 0) {
loop_33:
        if (var_s1_4 < sp78) {
            do {
                if ((u32) D_800354B0 < (u32) D_800354B4) {
                    D_800354B0 += 2;
                    var_v1_6 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
                } else {
                    var_v1_6 = func_800066D4();
                }
                temp_t3 = var_v1_6 << var_s1_4;
                var_s1_4 += 0x10;
                var_s3_4 |= temp_t3;
            } while (var_s1_4 < sp78);
        }
        temp_t0_2 = ((var_s3_4 & sp88) * 8) + sp80;
        sp7C = temp_t0_2;
        temp_s0 = temp_t0_2->unk1;
        var_s3_4 = var_s3_4 >> temp_s0;
        var_s1_4 -= temp_s0;
        temp_s0_2 = temp_t0_2->unk4;
        if (temp_s0_2 < 0x10U) {
            temp_s7[var_s4_3] = (u32) temp_s0_2;
            var_s4_3 += 1;
            var_s6 = temp_s0_2;
            goto block_74;
        }
        if (temp_s0_2 == 0x10) {
            if (var_s1_4 < 2U) {
                do {
                    if ((u32) D_800354B0 < (u32) D_800354B4) {
                        D_800354B0 += 2;
                        var_v1_7 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
                    } else {
                        var_v1_7 = func_800066D4();
                    }
                    temp_t7 = var_v1_7 << var_s1_4;
                    var_s1_4 += 0x10;
                    var_s3_4 |= temp_t7;
                } while (var_s1_4 < 2U);
            }
            temp_s0_3 = (var_s3_4 & 3) + 3;
            temp_at = temp_fp < (u32) (var_s4_3 + temp_s0_3);
            var_s3_4 = var_s3_4 >> 2;
            var_s1_4 -= 2;
            if (temp_at != 0) {
                return 1;
            }
            var_s0_2 = temp_s0_3 - 1;
            if (temp_s0_3 != 0) {
                var_v0 = &temp_s7[var_s4_3];
                do {
                    temp_v1 = var_s0_2;
                    var_s0_2 -= 1;
                    *var_v0 = (u32) var_s6;
                    var_s4_3 += 1;
                    var_v0 += 4;
                } while (temp_v1 != 0);
            }
            goto block_74;
        }
        if (temp_s0_2 == 0x11) {
            if (var_s1_4 < 3U) {
                do {
                    if ((u32) D_800354B0 < (u32) D_800354B4) {
                        D_800354B0 += 2;
                        var_v1_8 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
                    } else {
                        var_v1_8 = func_800066D4();
                    }
                    temp_t6_2 = var_v1_8 << var_s1_4;
                    var_s1_4 += 0x10;
                    var_s3_4 |= temp_t6_2;
                } while (var_s1_4 < 3U);
            }
            temp_s0_4 = (var_s3_4 & 7) + 3;
            temp_at_2 = temp_fp < (u32) (var_s4_3 + temp_s0_4);
            var_s3_4 = var_s3_4 >> 3;
            var_s1_4 -= 3;
            if (temp_at_2 != 0) {
                return 1;
            }
            var_s0_3 = temp_s0_4 - 1;
            var_s6 = 0;
            if (temp_s0_4 != 0) {
                var_v0_2 = &temp_s7[var_s4_3];
                do {
                    temp_v1_2 = var_s0_3;
                    var_s0_3 -= 1;
                    *var_v0_2 = 0;
                    var_s4_3 += 1;
                    var_v0_2 += 4;
                } while (temp_v1_2 != 0);
            }
            goto block_74;
        }
        if (var_s1_4 < 7U) {
            do {
                if ((u32) D_800354B0 < (u32) D_800354B4) {
                    D_800354B0 += 2;
                    var_v1_9 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
                } else {
                    var_v1_9 = func_800066D4();
                }
                temp_t5 = var_v1_9 << var_s1_4;
                var_s1_4 += 0x10;
                var_s3_4 |= temp_t5;
            } while (var_s1_4 < 7U);
        }
        temp_s0_5 = (var_s3_4 & 0x7F) + 0xB;
        temp_at_3 = temp_fp < (u32) (var_s4_3 + temp_s0_5);
        var_s3_4 = var_s3_4 >> 7;
        var_s1_4 -= 7;
        if (temp_at_3 != 0) {
            return 1;
        }
        var_s0_4 = temp_s0_5 - 1;
        var_s6 = 0;
        if (temp_s0_5 != 0) {
            var_v0_3 = &temp_s7[var_s4_3];
            do {
                temp_v1_3 = var_s0_4;
                var_s0_4 -= 1;
                *var_v0_3 = 0;
                var_s4_3 += 1;
                var_v0_3 += 4;
            } while (temp_v1_3 != 0);
        }
block_74:
        if (var_s4_3 >= sp84) {
            goto block_75;
        }
        goto loop_33;
    }
block_75:
    D_800354C8 = 0x4F0;
    D_800354BC = var_s3_4;
    D_800354C0 = var_s1_4;
    sp78 = D_8002B1A8;
    temp_v0_2 = func_80004D98(temp_s7, sp6C, 0x101U, &D_8002B08C, &D_8002B0CC, &sp80, &sp78);
    if (temp_v0_2 != 0) {
        D_800354C8 = 0;
        return temp_v0_2;
    }
    sp74 = D_8002B1AC;
    temp_v0 = func_80004D98(&temp_s7[sp6C], sp68, 0U, &D_8002B10C, &D_8002B148, &sp7C, &sp74);
    if (temp_v0 != 0) {
        D_800354C8 = 0;
        return temp_v0;
    }
    temp_v0_4 = func_80005464(sp80, sp7C, sp78, sp74);
    if (temp_v0_4 != 0) {
        if (temp_v0_4 < 0) {
            var_v1 = temp_v0_4;
        } else {
            var_v1 = 1;
        }
        return var_v1;
    }
    D_800354C8 = 0;
    return 0;
}

s32 func_800064D4(s32 *arg0) {
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_v0;
    s32 var_v1;
    s32 var_v1_2;
    u32 var_s0;
    u32 var_s0_2;
    u32 var_s2;
    u32 var_s2_2;

    var_s0 = D_800354C0;
    var_s2 = D_800354BC;
    if (var_s0 == 0) {
        do {
            if ((u32) D_800354B0 < (u32) D_800354B4) {
                D_800354B0 += 2;
                var_v1 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
            } else {
                var_v1 = func_800066D4();
            }
            temp_t1 = var_v1 << var_s0;
            var_s0 += 0x10;
            var_s2 |= temp_t1;
        } while (var_s0 == 0);
    }
    var_s0_2 = var_s0 - 1;
    var_s2_2 = var_s2 >> 1;
    *arg0 = var_s2 & 1;
    if (var_s0_2 < 2U) {
        do {
            if ((u32) D_800354B0 < (u32) D_800354B4) {
                D_800354B0 += 2;
                var_v1_2 = (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
            } else {
                var_v1_2 = func_800066D4();
            }
            temp_t0 = var_v1_2 << var_s0_2;
            var_s0_2 += 0x10;
            var_s2_2 |= temp_t0;
        } while (var_s0_2 < 2U);
    }
    temp_v0 = var_s2_2 & 3;
    D_800354BC = var_s2_2 >> 2;
    D_800354C0 = var_s0_2 - 2;
    if (temp_v0 == 2) {
        return func_80005D44();
    }
    if (temp_v0 == 0) {
        return func_8000595C();
    }
    if (temp_v0 == 1) {
        return func_80005B7C();
    }
    return 1;
}

s32 func_80006678(void) {
    s32 sp24;

    D_800354C0 = 0;
    D_800354BC = 0;
loop_1:
    if (func_800064D4(&sp24) != 0) {
        return 1;
    }
    if (sp24 != 0) {
        return 0;
    }
    goto loop_1;
}

s32 func_800066D4(void) {
    ? sp3C;
    ? *var_s1;

    if (func_80007270(&D_80035428, &sp3C, 0) == -1) {
        do {

        } while (func_80007270(&D_80035428, &sp3C, 0) == -1);
    }
    if (D_800354D4 != 0) {
        var_s1 = &D_80085A50;
        D_800354B0 = (u32) &D_80084A50;
        D_800354D4 = 0;
    } else {
        var_s1 = &D_80084A50;
        D_800354D4 = 1;
        D_800354B0 = (u32) &D_80085A50;
    }
    D_800354B4 = D_800354B0 + 0x1000;
    D_800354D0 += 0x1000;
    func_800084E0((? *) D_800354B0, 0x1000);
    func_80008630(&D_800354D8, 0, 0, D_800354D0, var_s1, 0x1000, &D_80035428);
    D_800354B0 += 2;
    return (D_800354B0->unk-1 << 8) | D_800354B0->unk-2;
}

s32 func_80006814(s32 arg0, u8 *arg1, s32 arg2) {
    ? sp44;
    s32 sp40;
    s32 temp_v0;

    D_800354B8 = arg1;
    D_800354D0 = arg0;
    D_800354B0 = (u32) &D_80084A50;
    D_800354B4 = D_800354B0;
    D_800354D4 = 1;
    func_800084E0((? *) D_800354B0, 0x1000, arg0);
    func_80008630(&D_800354D8, 0, 0, D_800354D0, (? *) D_800354B0, 0x1000, &D_80035428);
    if (arg2 != 0) {
        temp_v0 = sound_play_menu(0, 0x2EE0);
        sp40 = temp_v0;
        func_80004D50(temp_v0, 0x2EE0);
    } else {
        func_80004D50(0x803FD120, 0x2EE0);
    }
    func_80006678();
    if (arg2 != 0) {
        func_800960D4(sp40);
    }
    if (func_80007270(&D_80035428, &sp44, 0) == -1) {
        do {

        } while (func_80007270(&D_80035428, &sp44, 0) == -1);
    }
    func_80008460(arg1, D_800354B8 - arg1);
    return D_800354B8 - arg1;
}

s32 func_8000697C(u32 arg0, s32 arg1, u8 *arg2) {
    s32 sp1C;
    s32 temp_v0;

    D_800354B8 = arg2;
    D_800354B0 = arg0;
    D_800354B4 = D_800354B0 + arg1;
    temp_v0 = sound_play_menu(0, 0x2EE0, arg0);
    sp1C = temp_v0;
    func_80004D50(temp_v0, 0x2EE0);
    func_80006678();
    func_800960D4(sp1C);
    return D_800354B8 - arg2;
}
