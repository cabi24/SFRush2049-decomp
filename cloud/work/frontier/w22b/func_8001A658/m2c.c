s32 func_8001467C(s32);                             /* extern */
? func_800149BC(s32);                               /* extern */
? func_800149DC(s32);                               /* extern */
? func_80014A0C(s32, s32);                          /* extern */
? func_80014A74(s32, s32, s32, s32, s32);           /* extern */
? func_80014D14();                                  /* extern */
? func_80018C2C(s32);                               /* extern */
? func_80018D40(s32);                               /* extern */
? func_800190AC(s32, ?, ?);                         /* extern */
s32 func_80019AD4(? *, s32);                        /* extern */
? func_80019BE4(? *);                               /* extern */
s32 func_8001A5D8(? *, s32);                        /* extern */
u8 func_8001E7BC(s32);                              /* extern */
? func_80020FDC(u8, u8, ?);                         /* extern */
s32 func_80021428(? *, u8);                         /* extern */
s32 func_80021448(? *, u8);                         /* extern */
s32 func_80021468(? *, s32);                        /* extern */
u8 func_80021488(? *);                              /* extern */
s32 func_800214A8(? *);                             /* extern */
s32 func_800214C8(? *, s32);                        /* extern */
s32 func_800214E8(? *);                             /* extern */
s32 func_80021508(? *);                             /* extern */
s32 func_80021528(? *, u8);                         /* extern */
? func_80023E9C(? *);                               /* extern */
extern s32 D_8004BE90;
extern ? D_8004BEB8;
extern ? D_8004F2B8;
extern u8 D_8004F2F8;
extern ? D_8004F300;
extern ? D_8004F800;
extern u8 D_8004FA18;

void func_8001A658(void) {
    s32 sp68;
    s32 sp58;
    s32 sp48;
    ? *var_s0;
    ? *var_s0_5;
    ? *var_s2;
    ? *var_v0;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_lo;
    s32 temp_s1;
    s32 temp_t0;
    s32 temp_t3;
    s32 temp_t6;
    s32 temp_v0_11;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_7;
    s32 temp_v0_9;
    s32 temp_v1_5;
    s32 temp_v1_6;
    s32 var_a0;
    s32 var_a3;
    s32 var_s0_2;
    s32 var_s0_4;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_s6;
    s32 var_v0_4;
    s32 var_v0_5;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    u32 temp_s0;
    u32 temp_v0;
    u32 temp_v0_3;
    u32 temp_v1_2;
    u32 temp_v1_3;
    u32 var_s0_3;
    u32 var_v0_2;
    u32 var_v0_3;
    u8 temp_a0_2;
    u8 temp_t7;
    u8 temp_v0_10;
    u8 temp_v0_2;
    u8 temp_v0_6;
    u8 temp_v1_4;
    u8 var_a0_2;
    u8 var_a1;
    void *temp_v0_8;
    void *temp_v1;

    var_s2 = &D_8004BEB8;
    var_s6 = 0;
    if ((s32) D_8004FA18 > 0) {
        do {
            if ((unaligned s32) var_s2->unk0 != 0) {
                func_80023E9C(var_s2);
                goto block_5;
            }
            if (func_8001467C(var_s6) != 0) {
block_5:
                var_s1 = 0;
                var_s0 = var_s2;
                if (var_s2->unkBD == 0) {
                    do {
                        temp_v0 = (unaligned s32) var_s0->unk16C;
                        if (temp_v0 != 0) {
                            var_s0->unk168 = (unaligned s32) ((unaligned s32) var_s0->unk168 + D_8004BE90);
                            temp_v0_2 = func_8001E7BC(((u32) (((u32) (unaligned s32) var_s0->unk168 % temp_v0) * 0x10) / (u32) (temp_v0 >> 8)) & 0xFFFF);
                            var_s0->unk170 = (s8) (temp_v0_2 >> 8);
                            var_s0->unk171 = temp_v0_2;
                        }
                        var_s1 += 0xC;
                        var_s0 += 0xC;
                    } while (var_s1 < 0x18);
                    if ((unaligned s32) var_s2->unk24 & 0x4000) {
                        temp_v0_3 = (unaligned s32) var_s2->unk70;
                        var_s2->unk6C = (unaligned s32) ((unaligned s32) var_s2->unk6C + D_8004BE90);
                        var_s2->unk74 = (unaligned s32) func_8001E7BC(((u32) (((u32) (unaligned s32) var_s2->unk6C % temp_v0_3) * 0x10) / (u32) (temp_v0_3 >> 8)) & 0xFFFF);
                    }
                    var_a0 = 0;
                    var_v0 = var_s2;
                    do {
                        if (var_v0->unk48 != 0) {
                            temp_v1 = var_s2 + (var_a0 * 4);
                            temp_t7 = var_v0->unkB0 - 1;
                            var_v0->unkB0 = temp_t7;
                            if (!(temp_t7 & 0xFF)) {
                                var_v0->unkB0 = (u8) var_v0->unk48;
                                temp_v1->unk40 = 0;
                            } else {
                                temp_v1->unk40 = (s32) (temp_v1->unk40 + temp_v1->unk7C);
                            }
                        }
                        var_a0 += 1;
                        var_v0 += 1;
                    } while (var_a0 != 2);
                    if (var_s2->unk4A != 0xFF) {
                        if (func_800214E8(var_s2) < 0x1F81) {
                            var_s2->unk24 = (unaligned s32) ((unaligned s32) var_s2->unk24 & 0xBFFFFFFF);
                        } else {
                            var_s2->unk24 = (unaligned s32) ((unaligned s32) var_s2->unk24 | 0x40000000);
                        }
                    } else {
                        var_s2->unk24 = (unaligned s32) ((unaligned s32) var_s2->unk24 & 0xBFFFFFFF);
                    }
                    var_s0_2 = (unaligned s32) var_s2->unk24;
                    if (!(var_s0_2 & 0x1000)) {
                        if (var_s2->unk4A != 0xFF) {
                            if (func_80021508(var_s2) < 0x1F81) {
                                var_s2->unk24 = (unaligned s32) ((unaligned s32) var_s2->unk24 & ~0x800);
                                func_80020FDC(var_s2->unk4A, var_s2->unk4B, 0);
                            } else {
                                if (!((unaligned s32) var_s2->unk24 & 0x800)) {
                                    func_80019BE4(var_s2);
                                }
                                var_s2->unk24 = (unaligned s32) ((unaligned s32) var_s2->unk24 | 0x800);
                            }
                            var_s0_2 = M2C_ERROR(/* Unable to handle lwr; missing a corresponding lwl */);
                        }
                        var_s2->unk24 = (unaligned s32) (var_s0_2 | 0x2000);
                    }
                    if ((unaligned s32) var_s2->unk24 & 0x20) {
                        var_s2->unk24 = (unaligned s32) ((unaligned s32) var_s2->unk24 & ~0x20);
                        var_s2->unk24 = (unaligned s32) ((unaligned s32) var_s2->unk24 | 0x10);
                        func_800149BC(var_s6);
                    }
                    if (((unaligned s32) var_s2->unk24 & 0x80) && !((unaligned s32) var_s2->unk24 & 0x40000000)) {
                        var_s2->unk24 = (unaligned s32) ((unaligned s32) var_s2->unk24 & ~0x90);
                        func_800149DC(var_s6);
                    }
                    temp_v0_4 = (unaligned s32) var_s2->unk28;
                    if (temp_v0_4 != 0) {
                        var_s2->unk28 = (unaligned s32) (temp_v0_4 - ((var_s2->unk2D | (var_s2->unk2C << 8)) * D_8004BE90));
                        if ((unaligned s32) var_s2->unk28 < 0) {
                            var_s2->unk28 = (unaligned s32) 0;
                        }
                    }
                    if ((unaligned s32) var_s2->unk24 & 0x10000) {
                        temp_v0_5 = (unaligned s32) var_s2->unkAC - (((u32) (unaligned s32) var_s2->unkA8 >> 8) * (unaligned s32) var_s2->unk3C);
                        var_s2->unk38 = (unaligned s32) temp_v0_5;
                        if (temp_v0_5 < 0) {
                            var_s2->unk38 = (unaligned s32) 0;
                        } else {
                            temp_v1_2 = (unaligned s32) var_s2->unk38;
                            var_v0_2 = temp_v1_2;
                            if (temp_v1_2 >= 0x7F0001U) {
                                var_v0_2 = 0x7F0000;
                            }
                            var_s2->unk38 = (unaligned s32) var_v0_2;
                        }
                        var_s2->unkA8 = (unaligned s32) ((unaligned s32) var_s2->unkA8 - D_8004BE90);
                        if ((unaligned s32) var_s2->unkA8 == 0) {
                            var_s2->unk24 = (unaligned s32) ((unaligned s32) var_s2->unk24 & 0xFFFEFFFF);
                            var_s2->unk38 = (unaligned s32) var_s2->unkAC;
                        }
                    }
                    var_s1_2 = ((var_s2->unk51 | (var_s2->unk50 << 8)) << 0x10) + ((s32) (var_s2->unkC0 << 0x10) / 100);
                    if ((unaligned s32) var_s2->unk24 & 0x40010) {
                        if (var_s2->unk4A != 0xFF) {
                            temp_v0_6 = func_80021488(var_s2);
                            var_a0_2 = temp_v0_6;
                            var_s2->unkB8 = (u8) (temp_v0_6 >> 8);
                            var_s2->unkB9 = temp_v0_6;
                            goto block_51;
                        }
                    } else {
                        var_a0_2 = var_s2->unkB9 | (var_s2->unkB8 << 8);
block_51:
                        temp_a0 = var_a0_2 - 0x2000;
                        if (var_a0_2 != 0x2000) {
                            if (temp_a0 < 0) {
                                var_s1_2 += var_s2->unk6A * temp_a0 * 8;
                            } else {
                                var_s1_2 += var_s2->unk6B * temp_a0 * 8;
                            }
                        }
                    }
                    if ((unaligned s32) var_s2->unk24 & 0x4000) {
                        temp_a1 = (var_s2->unk78 << 8) + ((s32) (var_s2->unk79 << 8) / 100);
                        if (((unaligned s32) var_s2->unk24 & 0x8000) && (var_s2->unk4A != 0xFF)) {
                            sp58 = temp_a1;
                            var_v1 = (s32) ((func_800214C8(var_s2, temp_a1) >> 7) * (unaligned s32) var_s2->unk74) >> 7;
                        } else {
                            var_v1 = (unaligned s32) var_s2->unk74;
                        }
                        var_s1_2 += (s32) (temp_a1 * var_v1) >> 4;
                    }
                    temp_s0 = func_8001A5D8(var_s2, func_80019AD4(var_s2, var_s1_2)) + var_s2->unk40 + var_s2->unk44;
                    if (var_s2->unk4A != 0xFF) {
                        var_s0_3 = (u32) (func_800214A8(var_s2) * (temp_s0 >> 0x10)) >> 0xD;
                    } else {
                        var_s0_3 = temp_s0 >> 0x10;
                    }
                    func_80014A0C(var_s6, var_s0_3 & 0xFFFF);
                    var_a3 = 0;
                    if ((unaligned s32) var_s2->unk24 & 0x20000) {
                        temp_v0_7 = (unaligned s32) var_s2->unk88 - (((u32) (unaligned s32) var_s2->unkB4 >> 8) * (unaligned s32) var_s2->unk84);
                        var_s2->unk30 = (unaligned s32) temp_v0_7;
                        if (temp_v0_7 < 0) {
                            var_s2->unk30 = (unaligned s32) 0;
                        } else {
                            temp_v1_3 = (unaligned s32) var_s2->unk30;
                            var_v0_3 = temp_v1_3;
                            if (temp_v1_3 >= 0x7F0001U) {
                                var_v0_3 = 0x7F0000;
                            }
                            var_s2->unk30 = (unaligned s32) var_v0_3;
                        }
                        var_s2->unkB4 = (unaligned s32) ((unaligned s32) var_s2->unkB4 - D_8004BE90);
                        if ((unaligned s32) var_s2->unkB4 <= 0) {
                            var_s2->unk24 = (unaligned s32) ((unaligned s32) var_s2->unk24 & 0xFFFDFFFF);
                            var_s2->unk30 = (unaligned s32) var_s2->unk88;
                        }
                    }
                    temp_v0_8 = (var_s2->unkBE * 0x28) + &D_8004F300;
                    temp_a0_2 = var_s2->unk4A;
                    var_v0_4 = 0x15;
                    var_s1_3 = (unaligned s32) var_s2->unk38;
                    if (var_s2->unk4C != 0) {
                        var_v0_4 = 0x16;
                    }
                    temp_v1_4 = var_s2->unk2F;
                    var_s0_4 = ((s32) *(&D_8004F300 + (var_v0_4 * 0x28)) >> 0x10) * ((s32) (((s32) temp_v0_8->unk0 >> 0x10) * ((s32) (((s32) temp_v0_8->unk18 >> 0x10) * ((s32) (unaligned s32) var_s2->unk30 >> 7)) >> 7)) >> 7);
                    if (temp_v1_4 != 0xFF) {
                        var_s0_4 = *(&D_8004F2B8 + temp_v1_4) * (var_s0_4 >> 7);
                    }
                    if (D_8004F2F8 == 0) {
                        var_a1 = temp_a0_2;
                        if (temp_a0_2 != 0xFF) {
                            sp48 = (s32) var_a1;
                            var_s0_4 = (func_80021428(var_s2, var_a1) >> 7) * (var_s0_4 >> 7);
                            if (var_s1_3 != 0x800000) {
                                sp48 = (s32) var_a1;
                                temp_s1 = var_s1_3 + (((func_80021448(var_s2, var_a1) >> 7) - 0x40) << 0x10);
                                if (temp_s1 < 0) {
                                    var_s1_3 = 0;
                                } else {
                                    var_v0_5 = temp_s1;
                                    if (temp_s1 >= 0x7F0001) {
                                        var_v0_5 = 0x7F0000;
                                    }
                                    var_s1_3 = var_v0_5;
                                }
                            }
                            sp48 = (s32) var_a1;
                            var_a3 = func_80021468(var_s2, (s32) var_a1) << 9;
                            if (var_a3 >= 0x7F0001) {
                                var_a3 = 0x7F0000;
                            }
                        } else {
                            var_a3 = 0;
                        }
                    } else {
                        var_a1 = temp_a0_2;
                        if (temp_a0_2 != 0xFF) {
                            sp48 = (s32) var_a1;
                            sp68 = 0;
                            temp_lo = (func_80021428(var_s2, var_a1) >> 7) * (var_s0_4 >> 7);
                            var_a3 = 0;
                            var_s0_4 = temp_lo;
                        }
                        var_s1_3 = 0x400000;
                    }
                    var_v1_2 = 0;
                    if (var_a1 != 0xFF) {
                        sp68 = var_a3;
                        var_v1_3 = (s32) ((func_80021528(var_s2, var_a1) >> 7) * var_s0_4) >> 7;
                        if (var_v1_3 >= 0x7F0001) {
                            var_v1_3 = 0x7F0000;
                        }
                        var_v1_4 = (var_v1_3 >> 7) * var_s2->unk99;
                        if (var_v1_4 >= 0x7F0001) {
                            var_v1_4 = 0x7F0000;
                        }
                        var_v1_2 = (var_s2->unk9A << 0x10) + var_v1_4;
                        if (var_v1_2 >= 0x7F0001) {
                            var_v1_2 = 0x7F0000;
                        }
                    }
                    temp_t6 = var_s0_4 >> 8;
                    var_s2->unkC2 = (s8) ((u32) temp_t6 >> 8);
                    var_s2->unkC3 = (s8) temp_t6;
                    func_80014A74(var_s6, var_s0_4, var_s1_3, var_a3, var_v1_2);
                }
            }
            var_s6 += 1;
            var_s2 += 0x1A0;
        } while (var_s6 < (s32) D_8004FA18);
    }
    var_s0_5 = &D_8004F300;
    do {
        temp_v1_5 = var_s0_5->unkC;
        if (temp_v1_5 != 0) {
            temp_v0_9 = var_s0_5->unk4 - (var_s0_5->unk8 * (temp_v1_5 >> 8));
            var_s0_5->unk0 = temp_v0_9;
            if (temp_v0_9 < 0) {
                var_s0_5->unk0 = 0;
            }
            temp_t3 = var_s0_5->unkC - D_8004BE90;
            var_s0_5->unkC = temp_t3;
            if (temp_t3 <= 0) {
                temp_v0_10 = var_s0_5->unk15;
                var_s0_5->unkC = 0;
                var_s0_5->unk0 = (s32) var_s0_5->unk4;
                switch (temp_v0_10) {               /* irregular */
                case 1:
                    func_80018D40(var_s0_5->unk10);
                    break;
                case 2:
                    func_80018C2C(var_s0_5->unk10);
                    break;
                case 3:
                    func_800190AC(var_s0_5->unk10, 0, 0);
                    break;
                }
            }
        }
        temp_v1_6 = var_s0_5->unk24;
        if (temp_v1_6 != 0) {
            temp_v0_11 = var_s0_5->unk1C - (var_s0_5->unk20 * (temp_v1_6 >> 8));
            var_s0_5->unk18 = temp_v0_11;
            if (temp_v0_11 < 0) {
                var_s0_5->unk18 = 0;
            }
            temp_t0 = var_s0_5->unk24 - D_8004BE90;
            var_s0_5->unk24 = temp_t0;
            if (temp_t0 <= 0) {
                var_s0_5->unk24 = 0;
                var_s0_5->unk18 = (s32) var_s0_5->unk1C;
            }
        }
        var_s0_5 += 0x28;
    } while (var_s0_5 != &D_8004F800);
    func_80014D14();
}
