s32 func_80002CD0(s32, s32, void *);                /* static */
u8 *func_80004674(f64 arg0, s32 *arg2, u8 *arg3, u8 *arg4, s32 arg5, u8 *arg6); /* static */
u8 *func_80004878(u8 *arg0, s32 arg1, s32 arg2);    /* static */
extern f64 D_8002D6C0;
extern f64 D_8002D6C8;

f64 func_800028A0(f64 arg0, f64 *arg2) {
    f64 sp8;
    f64 sp0;
    f64 var_fa1;

    sp0 = arg0;
    if (sp0 > 0.0) {
        var_fa1 = sp0;
    } else {
        var_fa1 = -sp0;
    }
    if (var_fa1 >= 4503599627370496.0) {
        *arg2 = sp0;
        return 0.0;
    }
    sp8 = var_fa1 + 4503599627370496.0;
    sp8 -= 4503599627370496.0;
    if (var_fa1 < sp8) {
        sp8 -= 1.0;
    }
    if (sp8 > 0.0) {

    } else {
        sp8 = -sp8;
    }
    if ((sp0 - sp8) == 1.0) {
        sp8 += 1.0;
    }
    if (sp0 >= 0.0) {
        *arg2 = sp8;
        return sp0 - sp8;
    }
    *arg2 = -sp8;
    return sp0 + sp8;
}

f32 func_80002A64(f32 arg0, f32 *arg1) {
    f32 sp8;
    f32 sp4;
    f32 var_fa1;

    sp4 = arg0;
    if (sp4 > 0.0f) {
        var_fa1 = sp4;
    } else {
        var_fa1 = -sp4;
    }
    if (var_fa1 >= 8.388608e6f) {
        *arg1 = sp4;
        return 0.0f;
    }
    sp8 = var_fa1 + 8.388608e6f;
    sp8 -= 8.388608e6f;
    if (var_fa1 < sp8) {
        sp8 -= 1.0f;
    }
    if (sp8 > 0.0f) {

    } else {
        sp8 = -sp8;
    }
    if ((sp4 - sp8) == 1.0f) {
        sp8 += 1.0f;
    }
    if (sp4 >= 0.0f) {
        *arg1 = sp8;
        return sp4 - sp8;
    }
    *arg1 = -sp8;
    return sp4 + sp8;
}

s32 func_80002BF0(f64 arg0) {
    f64 sp0;                                        /* compiler-managed */
    s32 var_v0;

    sp0 = arg0;
    if (((u32) ((bitwise s32) sp0 * 2) >> 0x15) == 0x7FF) {
        sp0 = (bitwise u16) sp0 & 0x800F;
        var_v0 = 0;
        if ((bitwise f64) sp0 != 0.0) {
            var_v0 = 1;
        }
        return var_v0;
    }
    return 0;
}

s32 func_80002C60(f64 arg0) {
    f64 sp0;                                        /* compiler-managed */
    s32 var_v0;

    sp0 = arg0;
    if (((u32) ((bitwise s32) sp0 * 2) >> 0x15) == 0x7FF) {
        sp0 = (bitwise u16) sp0 & 0x800F;
        var_v0 = 0;
        if ((bitwise f64) sp0 == 0.0) {
            var_v0 = 1;
        }
        return var_v0;
    }
    return 0;
}

/*
Decompilation failure in function func_80002CD0:

Found jr instruction at 34A0.s line 423, but the corresponding jump table is not provided.

Please include it in the input .s file(s), or in an additional file.

*/

s32 func_80003C3C(f64 arg0, s32 arg2, s32 arg3, u8 *arg4, s32 arg5, u8 *arg6, u32 arg7) {
    s32 sp64;
    s32 sp60;
    s32 sp5C;
    f64 sp50;
    f64 sp48;
    f64 var_fs0;
    s32 temp_t0_2;
    s32 temp_t0_3;
    s32 temp_t5_2;
    s32 temp_t7;
    s32 temp_t8_2;
    u8 *temp_t0;
    u8 *var_s0_2;
    u8 *var_s0_3;
    u8 *var_s0_4;
    u8 *var_s1;
    u8 *var_s1_2;
    u8 temp_t1;
    u8 temp_t5;
    u8 temp_t6;
    u8 temp_t8;
    u8 temp_t9;
    u8 temp_t9_2;
    void *var_s0;

    sp5C = 0;
    sp60 = 0;
    sp64 = 0;
    if (arg0 < 0.0) {
        arg0 = -arg0;
        *arg4 = 0x2D;
    } else {
        *arg4 = 0;
    }
    var_fs0 = func_800028A0(arg0, &sp50);
    temp_t0 = arg6 + 1;
    arg6 = temp_t0;
    var_s1 = temp_t0;
    if (arg0 >= 1.0) {
        var_s0 = arg7 - 1;
        if (sp50 != 0.0) {
            do {
                sp48 = func_800028A0(sp50 / 10.0, &sp50);
                var_s0 -= 1;
                var_s0->unk1 = (s8) ((s32) ((sp48 + D_8002D6C0) * 10.0) + 0x30);
                sp60 += 1;
            } while (sp50 != 0.0);
        }
    } else {
        var_s0 = arg7 - 1;
    }
    switch (arg5) {                                 /* irregular */
    case 0x66:
        if (sp60 != 0) {
            var_s0_2 = var_s0 + 1;
            if ((u32) var_s0_2 < arg7) {
                do {
                    temp_t8 = *var_s0_2;
                    var_s0_2 += 1;
                    var_s1 += 1;
                    var_s1->unk-1 = temp_t8;
                } while ((u32) var_s0_2 < arg7);
            }
        } else {
            arg6->unk1 = 0x30;
            var_s1 += 1;
        }
        if ((arg2 != 0) || (arg3 & 8)) {
            *var_s1 = 0x2E;
            var_s1 += 1;
        }
        if (var_fs0 != 0.0) {
            if (arg2 != 0) {
loop_24:
                var_fs0 = func_800028A0(var_fs0 * 10.0, &sp48);
                var_s1 += 1;
                var_s1->unk-1 = (s8) ((s32) sp48 + 0x30);
                temp_t0_2 = arg2 - 1;
                arg2 = temp_t0_2;
                if (temp_t0_2 != 0) {
                    if (var_fs0 != 0.0) {
                        goto loop_24;
                    }
                }
            }
            if (var_fs0 != 0.0) {
                arg6 = func_80004674(var_fs0, NULL, arg6, var_s1 - 1, 0, arg4);
            }
        }
        arg2 -= 1;
        if (arg2 != 0) {
            do {
                *var_s1 = 0x30;
                var_s1 += 1;
                arg2 -= 1;
            } while (arg2 != 0);
        }
        break;
    case 0x45:
    case 0x65:
block_31:
        if (sp60 != 0) {
            var_s0_3 = var_s0 + 1;
            var_s1_2 = &arg6[1].unk1;
            var_s1_2->unk-1 = (u8) var_s0->unk1;
            if ((arg2 != 0) || (arg3 & 8)) {
                var_s1_2->unk0 = 0x2E;
                var_s1_2 += 1;
            }
            if (arg2 != 0) {
                var_s0_3 += 1;
                if ((u32) var_s0_3 < arg7) {
loop_37:
                    var_s1_2 += 1;
                    var_s1_2->unk-1 = (u8) *var_s0_3;
                    temp_t8_2 = arg2 - 1;
                    arg2 = temp_t8_2;
                    if (temp_t8_2 != 0) {
                        var_s0_3 += 1;
                        if ((u32) var_s0_3 < arg7) {
                            goto loop_37;
                        }
                    }
                }
            }
            if ((arg2 == 0) && ((u32) (var_s0_3 + 1) < arg7)) {
                var_fs0 = 0.0;
                arg6 = func_80004674(0.0, &sp60, arg6, var_s1_2 - 1, (s32) var_s0_3->unk1, arg4);
            }
            sp60 -= 1;
        } else if (var_fs0 != 0.0) {
            sp60 = -1;
loop_45:
            var_fs0 = func_800028A0(var_fs0 * 10.0, &sp48);
            if (sp48 == 0.0) {
                sp60 -= 1;
                goto loop_45;
            }
            var_s1_2 = &arg6[1].unk1;
            var_s1_2->unk-1 = (u8) ((s32) sp48 + 0x30);
            if ((arg2 != 0) || (arg3 & 8)) {
                var_s1_2->unk0 = 0x2E;
                var_s1_2 += 1;
            }
        } else {
            arg6->unk1 = 0x30;
            var_s1_2 = &arg6[1].unk1;
            if ((arg2 != 0) || (arg3 & 8)) {
                var_s1_2->unk0 = 0x2E;
                var_s1_2 += 1;
            }
        }
        if (var_fs0 != 0.0) {
            if (arg2 != 0) {
loop_57:
                var_fs0 = func_800028A0(var_fs0 * 10.0, &sp48);
                var_s1_2 += 1;
                var_s1_2->unk-1 = (s8) ((s32) sp48 + 0x30);
                temp_t7 = arg2 - 1;
                arg2 = temp_t7;
                if (temp_t7 != 0) {
                    if (var_fs0 != 0.0) {
                        goto loop_57;
                    }
                }
            }
            if (var_fs0 != 0.0) {
                arg6 = func_80004674(var_fs0, &sp60, arg6, var_s1_2 - 1, 0, arg4);
            }
        }
        arg2 -= 1;
        if (arg2 != 0) {
            do {
                *var_s1_2 = 0x30;
                var_s1_2 += 1;
                arg2 -= 1;
            } while (arg2 != 0);
        }
        if ((sp5C != 0) && !(arg3 & 8)) {
            if ((u32) arg6 < (u32) var_s1_2) {
                temp_t9 = var_s1_2->unk-1;
                var_s1_2 -= 1;
                if (temp_t9 == 0x30) {
loop_67:
                    if ((u32) arg6 < (u32) var_s1_2) {
                        temp_t1 = var_s1_2->unk-1;
                        var_s1_2 -= 1;
                        if (temp_t1 == 0x30) {
                            goto loop_67;
                        }
                    }
                }
            }
            if (*var_s1_2 == 0x2E) {
                var_s1_2 -= 1;
            }
            var_s1_2 += 1;
        }
        var_s1 = func_80004878(var_s1_2, sp60, arg5);
        break;
    case 0x47:
    case 0x67:
        if (arg2 == 0) {
            arg2 += 1;
        }
        if ((arg2 < sp60) || ((sp60 == 0) && (var_fs0 != 0.0) && (var_fs0 < D_8002D6C8))) {
            arg2 -= 1;
            arg5 -= 2;
            sp5C = 1;
            goto block_31;
        }
        if (sp60 != 0) {
            var_s0_4 = var_s0 + 1;
            if ((u32) var_s0_4 < arg7) {
                do {
                    temp_t5 = *var_s0_4;
                    var_s0_4 += 1;
                    var_s1 += 1;
                    var_s1->unk-1 = temp_t5;
                    arg2 -= 1;
                } while ((u32) var_s0_4 < arg7);
            }
        } else {
            arg6->unk1 = 0x30;
            var_s1 += 1;
        }
        if ((arg2 != 0) || (arg3 & 8)) {
            sp64 = 1;
            *var_s1 = 0x2E;
            var_s1 += 1;
        } else {
            sp64 = 0;
        }
        if (var_fs0 != 0.0) {
            if (arg2 != 0) {
                do {
                    var_fs0 = func_800028A0(var_fs0 * 10.0, &sp48);
                    var_s1 += 1;
                    var_s1->unk-1 = (s8) ((s32) sp48 + 0x30);
                } while (sp48 == 0.0);
                temp_t0_3 = arg2 - 1;
                arg2 = temp_t0_3;
                if ((temp_t0_3 != 0) && (var_fs0 != 0.0)) {
loop_94:
                    var_fs0 = func_800028A0(var_fs0 * 10.0, &sp48);
                    var_s1 += 1;
                    var_s1->unk-1 = (s8) ((s32) sp48 + 0x30);
                    temp_t5_2 = arg2 - 1;
                    arg2 = temp_t5_2;
                    if (temp_t5_2 != 0) {
                        if (var_fs0 != 0.0) {
                            goto loop_94;
                        }
                    }
                }
            }
            if (var_fs0 != 0.0) {
                arg6 = func_80004674(var_fs0, NULL, arg6, var_s1 - 1, 0, arg4);
            }
        }
        if (arg3 & 8) {
            arg2 -= 1;
            if (arg2 != 0) {
                do {
                    *var_s1 = 0x30;
                    var_s1 += 1;
                    arg2 -= 1;
                } while (arg2 != 0);
            }
        } else if (sp64 != 0) {
            if ((u32) arg6 < (u32) var_s1) {
                temp_t6 = var_s1->unk-1;
                var_s1 -= 1;
                if (temp_t6 == 0x30) {
loop_105:
                    if ((u32) arg6 < (u32) var_s1) {
                        temp_t9_2 = var_s1->unk-1;
                        var_s1 -= 1;
                        if (temp_t9_2 == 0x30) {
                            goto loop_105;
                        }
                    }
                }
            }
            if (*var_s1 != 0x2E) {
                var_s1 += 1;
            }
        }
        break;
    }
    return var_s1 - arg6;
}

u8 *func_80004674(f64 arg0, s32 *arg2, u8 *arg3, u8 *arg4, u8 arg5, u8 *arg6) {
    f64 sp18;

    if (arg0 != 0.0) {
        func_800028A0(arg0 * 10.0, &sp18);
    } else {
        sp18 = (f64) (arg5 - 0x30);
    }
    if (sp18 > 4.0) {
loop_4:
        if (arg4->unk0 == 0x2E) {
            arg4 -= 1;
        }
        arg4->unk0 += 1;
        if ((s32) arg4->unk0 < 0x3A) {

        } else {
            arg4->unk0 = 0x30;
            if (arg4 == arg3) {
                if (arg2 != NULL) {
                    arg4->unk0 = 0x31;
                    *arg2 += 1;
                } else {
                    arg4 -= 1;
                    arg4->unk-1 = 0x31;
                    arg3 -= 1;
                }
            } else {
                arg4 -= 1;
                goto loop_4;
            }
        }
    } else if (*arg6 == 0x2D) {
loop_16:
        if (arg4->unk0 == 0x2E) {
            arg4 -= 1;
        }
        if (arg4->unk0 == 0x30) {
            if (arg4 == arg3) {
                *arg6 = 0;
            }
            arg4 -= 1;
            goto loop_16;
        }
    }
    return arg3;
}

u8 *func_80004878(u8 *arg0, s32 arg1, s8 arg2) {
    void *sp134;
    s32 var_a1;
    u8 *var_a0;
    void *temp_a0;
    void *temp_t4;

    var_a1 = arg1;
    arg0->unk0 = arg2;
    temp_a0 = arg0 + 1;
    if (var_a1 < 0) {
        var_a1 = -var_a1;
        arg0->unk1 = 0x2D;
    } else {
        arg0->unk1 = 0x2B;
    }
    var_a0 = temp_a0 + 1;
    sp134 = sp + 0x134;
    if (var_a1 >= 0xA) {
        do {
            sp134 -= 1;
            sp134->unk-1 = (s8) ((var_a1 % 10) + 0x30);
            var_a1 = var_a1 / 10;
        } while (var_a1 >= 0xA);
        sp134 -= 1;
        sp134->unk-1 = (s8) (var_a1 + 0x30);
        if ((u32) sp134 < (u32) (sp + 0x134)) {
            do {
                var_a0 += 1;
                var_a0->unk-1 = (u8) sp134->unk0;
                temp_t4 = sp134 + 1;
                sp134 = temp_t4;
            } while ((u32) temp_t4 < (u32) (sp + 0x134));
        }
    } else {
        temp_a0->unk1 = 0x30;
        var_a0->unk1 = (s8) (var_a1 + 0x30);
        var_a0 = var_a0 + 1 + 1;
    }
    return var_a0;
}

s32 func_80004990(s32 arg0, s32 arg1, ? arg2, ? arg3) {
    s32 sp1C;
    void *sp18;
    s32 temp_v0;

    sp18 = &arg1 + 4;
    temp_v0 = func_80002CD0(arg0, arg1, sp18);
    sp1C = temp_v0;
    return temp_v0;
}
