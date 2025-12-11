? func_80008590(s32 *, ?);                          /* extern */
s32 func_8000E850(void *, ?);                       /* extern */
s32 func_8000E8D0(s32, s32, s32, s32 *);            /* extern */
s32 func_8000F124(void *);                          /* extern */
s32 func_8000F2D0();                                /* extern */
s32 func_8000F3A4(void *, u16 (*)[], ?, s32);       /* extern */
s32 func_8000F680(s32, s32, s32, s32 *, s32);       /* extern */
s32 func_8000B918(void *arg0, u16 (*arg1)[]);       /* static */
s32 func_8000BBF0(void *arg0, s32 arg1, u16 (*arg2)[]); /* static */

s32 func_8000B3E0(void *arg0) {
    u16 sp4BC;
    u16 sp2A2;
    s32 sp29C;
    s32 sp74;
    u16 sp60;
    s32 temp_a3;
    s32 temp_t8;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    s32 var_a0;
    s32 var_a1;
    s32 var_fp;
    s32 var_s0;
    s32 var_s4;
    s32 var_s4_2;
    s32 var_s4_3;
    s32 var_s6;
    s32 var_s7;
    s32 var_s7_2;
    s32 var_v0;
    u16 *temp_a0;
    u16 *var_a0_2;
    u16 *var_v0_2;
    u16 *var_v0_3;
    u16 *var_v1;
    u16 *var_v1_2;
    u16 *var_v1_3;
    u16 temp_at;
    u16 temp_at_2;
    u16 temp_t1;
    u16 temp_t3;
    u16 temp_t4;
    u16 temp_t5;
    u16 temp_t6;
    u8 temp_v1;
    u8 var_s1;

    sp74 = 0;
    var_fp = 0xFE;
    var_v0 = func_8000F2D0();
    var_s7_2 = var_v0;
    if (var_v0 == 2) {
        var_v0 = func_8000F124(arg0);
        var_s7_2 = var_v0;
    }
    if (var_v0 != 0) {
        return var_s7_2;
    }
    temp_v0_7 = func_8000B918(arg0, (u16 (*)[]) &sp78[0]);
    if (temp_v0_7 != 0) {
        return temp_v0_7;
    }
    var_s4 = 0;
    if (arg0->unk50 > 0) {
loop_7:
        temp_v0_6 = func_8000E8D0(arg0->unk4, arg0->unk8, (arg0->unk5C + var_s4) & 0xFFFF, &sp29C);
        var_s7 = temp_v0_6;
        if (temp_v0_6 != 0) {
            return temp_v0_6;
        }
        if ((sp2A0 != 0) || (sp29C != 0)) {
            if ((sp2A0 == 0) || (sp29C == 0)) {
                var_a0 = -1;
                goto block_29;
            }
            var_a0 = 0;
            var_s0 = 0;
            sp4BC = sp2A2;
            if (((s32) sp4BC >= arg0->unk60) && ((s32) (u8) sp4BC < (s32) arg0->unk64) && ((s32) unksp4BD > 0)) {
                var_s1 = 0xFF;
                if ((s32) unksp4BD < 0x80) {
loop_18:
                    if ((u8) sp4BC != var_s1) {
                        var_s1 = (u8) sp4BC;
                        if ((u8) sp4BC != var_fp) {
                            var_s7 = func_8000F3A4(arg0, (u16 (*)[]) &sp2BC[0], 0, (u8) sp4BC & 0xFF);
                            var_fp = (u8) sp4BC & 0xFF;
                        }
                        if ((var_s7 != 0) && (var_s7 != 3)) {
                            return var_s7;
                        }
                        goto block_24;
                    }
block_24:
                    var_a0 = func_8000BBF0(arg0, subroutine_arg1, (u16 (*)[]) &sp78[0]) - var_s0;
                    if ((var_a0 != 0) || (var_s0 = 1, sp4BC = (&sp2BC[0])[unksp4BD], (((s32) sp4BC < arg0->unk60) != 0)) || ((s32) (u8) sp4BC >= (s32) arg0->unk64) || ((s32) unksp4BD <= 0) || ((s32) unksp4BD >= 0x80)) {
                        goto block_29;
                    }
                    goto loop_18;
                }
            }
block_29:
            if ((var_a0 != 0) || (sp4BC != 1)) {
                func_80008590(&sp29C, 0x20);
                if (arg0->unk65 != 0) {
                    temp_v0_5 = func_8000E850(arg0, 0);
                    if (temp_v0_5 != 0) {
                        return temp_v0_5;
                    }
                }
                temp_v0_4 = func_8000F680(arg0->unk4, arg0->unk8, (arg0->unk5C + var_s4) & 0xFFFF, &sp29C, 0);
                if (temp_v0_4 != 0) {
                    return temp_v0_4;
                }
                sp74 += 1;
                goto block_37;
            }
            goto block_37;
        }
block_37:
        var_s4 += 1;
        if (var_s4 >= arg0->unk50) {
            var_s4 = 0;
            goto block_39;
        }
        goto loop_7;
    }
block_39:
    if (arg0->unk50 > 0) {
loop_40:
        temp_v0_3 = func_8000E8D0(arg0->unk4, arg0->unk8, (arg0->unk5C + var_s4) & 0xFFFF, &sp29C);
        if (temp_v0_3 != 0) {
            return temp_v0_3;
        }
        temp_a0 = &(&sp27C[0])[var_s4];
        if ((sp2A0 != 0) && (sp29C != 0) && ((s32) sp2A2 >= (s32) arg0->unk62)) {
            *temp_a0 = sp2A2;
        } else {
            *temp_a0 = 0;
        }
        var_s4 += 1;
        if (var_s4 >= arg0->unk50) {
            goto block_48;
        }
        goto loop_40;
    }
block_48:
    var_s6 = 0;
    if ((s32) arg0->unk64 > 0) {
loop_50:
        temp_v0_2 = func_8000F3A4(arg0, (u16 (*)[]) &sp2BC[0], 0, var_s6 & 0xFF);
        if ((temp_v0_2 != 0) && (temp_v0_2 != 3)) {
            return temp_v0_2;
        }
        var_s4_2 = 0;
        if (var_s6 > 0) {
            var_a1 = 1;
        } else {
            var_a1 = arg0->unk60;
        }
        temp_a3 = var_a1 & 3;
        if (var_a1 > 0) {
            if (temp_a3 != 0) {
                var_v1 = &(&sp3BC[0])[0];
                var_v0_2 = &(&sp2BC[0])[0];
                do {
                    temp_t1 = *var_v0_2;
                    var_s4_2 += 1;
                    var_v1 += 2;
                    var_v0_2 += 2;
                    var_v1->unk-2 = temp_t1;
                } while (temp_a3 != var_s4_2);
                if (var_s4_2 != var_a1) {
                    goto block_61;
                }
            } else {
block_61:
                var_v1_2 = &(&sp3BC[0])[var_s4_2];
                var_v0_3 = &(&sp2BC[0])[var_s4_2];
                do {
                    temp_t4 = var_v0_3->unk2;
                    temp_t6 = var_v0_3->unk4;
                    temp_t5 = var_v0_3->unk6;
                    temp_t3 = var_v0_3->unk0;
                    var_s4_2 += 4;
                    var_v1_2 += 8;
                    var_v0_3 += 8;
                    var_v1_2->unk-6 = temp_t4;
                    var_v1_2->unk-4 = temp_t6;
                    var_v1_2->unk-2 = temp_t5;
                    var_v1_2->unk-8 = temp_t3;
                } while (var_s4_2 != var_a1);
            }
        }
        if (var_s4_2 < 0x80) {
            var_v1_3 = &(&sp3BC[0])[var_s4_2];
            do {
                var_v1_3 += 2;
                var_v1_3->unk-2 = 3;
            } while ((u32) var_v1_3 < (u32) &sp4BC);
        }
        var_s4_3 = 0;
        if (arg0->unk50 > 0) {
            var_a0_2 = &sp27C[0];
            do {
                if ((var_s6 == var_a0_2->unk0) && ((s32) var_a0_2->unk0 >= (s32) arg0->unk62)) {
loop_70:
                    temp_v1 = var_a0_2->unk1;
                    temp_at = (&sp2BC[0])[temp_v1];
                    sp60 = temp_at;
                    (&sp3BC[0])[temp_v1] = temp_at;
                    temp_at_2 = sp60;
                    var_a0_2->unk0 = temp_at_2;
                    if (var_s6 == var_a0_2->unk0) {
                        if ((temp_at_2 & 0xFFFF) >= (s32) arg0->unk62) {
                            goto loop_70;
                        }
                    }
                }
                var_s4_3 += 1;
                var_a0_2 += 2;
            } while (var_s4_3 < arg0->unk50);
        }
        temp_v0 = func_8000F3A4(arg0, (u16 (*)[]) &sp3BC[0], 1, var_s6 & 0xFF);
        if (temp_v0 != 0) {
            return temp_v0;
        }
        temp_t8 = (var_s6 + 1) & 0xFF;
        var_s6 = temp_t8;
        if (temp_t8 >= (s32) arg0->unk64) {
            goto block_76;
        }
        goto loop_50;
    }
block_76:
    if (sp74 != 0) {
        arg0->unk0 = (s32) (arg0->unk0 | 2);
        return 0;
    }
    arg0->unk0 = (s32) (arg0->unk0 & ~2);
    return 0;
}

s32 func_8000B918(void *arg0, u16 (*arg1)[]) {
    ? sp15A;
    u16 sp158;
    s32 temp_v0;
    s32 var_s0;
    s32 var_s1;
    s32 var_s7;
    s32 var_v0;
    s32 var_v0_2;
    u16 (*var_v1)[];
    u16 *var_a0;
    u16 *var_a1;
    u16 temp_at;
    void *temp_v0_2;
    void *temp_v0_3;
    void *temp_v0_4;

    var_v0 = 0;
    var_v1 = arg1;
    do {
        var_v0 += 4;
        var_v1->unk102 = 0;
        var_v1->unk103 = 0;
        var_v1->unk104 = 0;
        var_v1 += 4;
        var_v1->unkFD = 0;
    } while (var_v0 != 0x100);
    arg1->unk100 = 0xFF;
    var_s7 = 0;
    var_s0 = 0;
    if ((s32) arg0->unk64 > 0) {
loop_4:
        if (var_s0 > 0) {
            var_s1 = 1;
        } else {
            var_s1 = arg0->unk60;
        }
        temp_v0 = func_8000F3A4(arg0, (u16 (*)[]) &sp58[0], 0, var_s7 & 0xFF);
        if ((temp_v0 != 0) && (temp_v0 != 3)) {
            return temp_v0;
        }
        var_v0_2 = var_s1;
        if (var_s1 < 0x80) {
            if ((0x80 - var_s1) & 1) {
                sp158 = (&sp58[0])[var_s1];
                if (((s32) sp158 >= arg0->unk60) && (var_s0 != (u8) sp158)) {
                    temp_v0_2 = arg1 + (((s32) (unksp159 & 0x7F) / 4) + (((s32) (u8) sp158 % 8) << 5));
                    temp_v0_2->unk101 = (u8) (temp_v0_2->unk101 | (1 << (var_s0 % 8)));
                }
                var_v0_2 = var_s1 + 1;
                if (var_v0_2 != 0x80) {
                    goto block_16;
                }
            } else {
block_16:
                var_a1 = &(&sp58[0])[var_v0_2];
                var_a0 = var_a1 + 2;
                do {
                    sp158 = *var_a1;
                    if (((s32) sp158 >= arg0->unk60) && (var_s0 != (u8) sp158)) {
                        temp_v0_3 = arg1 + (((s32) (unksp159 & 0x7F) / 4) + (((s32) (u8) sp158 % 8) << 5));
                        temp_v0_3->unk101 = (u8) (temp_v0_3->unk101 | (1 << (var_s0 % 8)));
                    }
                    temp_at = *var_a0;
                    var_a0 += 4;
                    sp158 = temp_at;
                    if (((s32) sp158 >= arg0->unk60) && (var_s0 != (u8) sp158)) {
                        temp_v0_4 = arg1 + (((s32) (unksp159 & 0x7F) / 4) + (((s32) (u8) sp158 % 8) << 5));
                        temp_v0_4->unk101 = (u8) (temp_v0_4->unk101 | (1 << (var_s0 % 8)));
                    }
                    var_a1 += 4;
                } while (var_a0 != &sp15A);
            }
        }
        var_s0 = (var_s7 + 1) & 0xFF;
        var_s7 = var_s0;
        if (var_s0 >= (s32) arg0->unk64) {
            goto block_25;
        }
        goto loop_4;
    }
block_25:
    return 0;
}

s32 func_8000BBF0(void *arg0, u8 arg1, u16 (*arg2)[]) {
    s32 temp_v0;
    s32 var_s0;
    s32 var_s1;
    s32 var_v0;
    s32 var_v0_2;
    u16 *var_v1;
    u8 var_s2;

    var_s0 = 0;
    var_s2 = 0;
    if ((s32) arg0->unk64 > 0) {
        var_v0 = 0;
loop_2:
        if (var_v0 > 0) {
            var_s1 = 1;
        } else {
            var_s1 = arg0->unk60;
        }
        if ((arg1 == var_v0) || ((arg2 + (((s32) arg1 / 4) + (((s32) arg1 % 8) << 5)))->unk101 & (1 << (var_v0 % 8)))) {
            if (arg2->unk100 != var_v0) {
                temp_v0 = func_8000F3A4(arg0, arg2, 0, var_s2 & 0xFF);
                if ((temp_v0 != 0) && (temp_v0 != 3)) {
                    return temp_v0;
                }
                arg2->unk100 = var_s2;
                goto block_12;
            }
block_12:
            if (var_s0 < 2) {
                var_v0_2 = var_s1 * 2;
                if (var_s1 < 0x80) {
                    var_v1 = &arg2->unk0[var_s1];
loop_15:
                    var_v0_2 += 2;
                    if ((u16) arg1 == *var_v1) {
                        var_s0 += 1;
                    }
                    var_v1 += 2;
                    if ((var_s0 < 2) && (var_v0_2 < 0x100)) {
                        goto loop_15;
                    }
                }
            }
            if (var_s0 >= 2) {
                return 2;
            }
            goto block_21;
        }
block_21:
        var_v0 = (var_s2 + 1) & 0xFF;
        var_s2 = (u8) var_v0;
        if (var_v0 >= (s32) arg0->unk64) {
            goto block_22;
        }
        goto loop_2;
    }
block_22:
    return var_s0;
}
