s32 func_8000B3E0(void *);                          /* extern */
? func_8000D2B0(? *, void *, ?);                    /* extern */
? func_8000E5B0();                                  /* extern */
? func_8000E5F4();                                  /* extern */
s32 func_8000E620(s32, s32);                        /* extern */
s32 func_8000E850(void *, ?);                       /* extern */
s32 func_8000E8D0(s32, s32, ?, ? *);                /* extern */
? func_8000EB74(? *, u16 *, u16 *);                 /* extern */
s32 func_8000EC70(void *, ? *, ? *);                /* extern */
s32 func_8000EFC0(void *, ? *);                     /* extern */
s32 func_8000F680(s32, s32, ?, ? *, s32);           /* extern */
s32 func_8000F8D0(? *, ? *, ?);                     /* extern */
s32 func_80009E18(void *arg0);                      /* static */

s32 func_80009C10(s32 arg0, void *arg1, s32 arg2) {
    s32 sp6C;
    u16 sp6A;
    u16 sp68;
    ? sp48;
    ? *sp44;
    ? sp20;
    s32 temp_t0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v1;
    s32 var_t4;
    s32 var_v0;
    u8 temp_t6;

    func_8000E5B0();
    sp6C = func_8000E620(arg0, arg2);
    func_8000E5F4();
    if (sp6C != 0) {
        return sp6C;
    }
    arg1->unk4 = arg0;
    arg1->unk0 = 0;
    arg1->unk8 = arg2;
    temp_v0_5 = func_80009E18(arg1);
    if (temp_v0_5 != 0) {
        return temp_v0_5;
    }
    temp_v0_4 = func_8000E850(arg1, 0);
    if (temp_v0_4 != 0) {
        return temp_v0_4;
    }
    temp_v0_3 = func_8000E8D0(arg1->unk4, arg1->unk8, 1, &sp48);
    if (temp_v0_3 != 0) {
        return temp_v0_3;
    }
    func_8000EB74(&sp48, &sp6A, &sp68);
    sp44 = &sp48;
    if ((sp6A != sp64) || (sp68 != sp66)) {
        var_v0 = func_8000EFC0(arg1, &sp48);
        if (var_v0 != 0) {
            var_t4 = arg1->unk0 | 4;
            goto block_22;
        }
    }
    if (!(sp60 & 1)) {
        temp_v0_2 = func_8000EC70(arg1, &sp48, &sp20);
        if (temp_v0_2 != 0) {
            if (temp_v0_2 == 0xA) {
                arg1->unk0 = (s32) (arg1->unk0 | 4);
            }
            return temp_v0_2;
        }
        sp44 = &sp20;
        if (!(sp38 & 1)) {
            return 0xB;
        }
        goto block_19;
    }
block_19:
    func_8000D2B0(sp44, arg1 + 0xC, 0x20);
    arg1->unk4C = (s32) sp44->unk1B;
    temp_t6 = sp44->unk1A;
    arg1->unk50 = 0x10;
    arg1->unk54 = 8;
    temp_v0_6 = temp_t6 & 0xFF;
    temp_v1 = temp_v0_6 * 8;
    temp_t0 = temp_v1 + 8;
    arg1->unk60 = (s32) ((temp_v0_6 * 2) + 3);
    arg1->unk58 = temp_t0;
    arg1->unk5C = (s32) (temp_t0 + temp_v1);
    arg1->unk64 = temp_t6;
    temp_v0 = func_8000E8D0(arg1->unk4, arg1->unk8, 7, arg1 + 0x2C);
    if (temp_v0 != 0) {
        return temp_v0;
    }
    var_v0 = func_8000B3E0(arg1);
    var_t4 = arg1->unk0 | 1;
block_22:
    arg1->unk0 = var_t4;
    return var_v0;
}

s32 func_80009E18(void *arg0) {
    ? sp70;
    ? sp50;
    ? sp30;
    ? *var_v1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s8 temp_t6;
    s8 temp_t7;
    s8 temp_t8;
    s8 var_v0;

    temp_v0_4 = func_8000E850(NULL);
    if (temp_v0_4 != 0) {
        return temp_v0_4;
    }
    temp_v0_3 = func_8000E8D0(arg0->unk4, arg0->unk8, 0, &sp30);
    var_v1 = &sp70;
    if (temp_v0_3 != 0) {
        return temp_v0_3;
    }
    var_v0 = 0;
    do {
        temp_t6 = var_v0 + 1;
        temp_t7 = var_v0 + 2;
        temp_t8 = var_v0 + 3;
        var_v1->unk0 = var_v0;
        var_v0 += 4;
        var_v1->unk3 = temp_t8;
        var_v1->unk2 = temp_t7;
        var_v1->unk1 = temp_t6;
        var_v1 += 4;
    } while (var_v0 != 0x20);
    temp_v0_2 = func_8000F680(arg0->unk4, arg0->unk8, 0, &sp70, 0);
    if (temp_v0_2 != 0) {
        return temp_v0_2;
    }
    temp_v0 = func_8000E8D0(arg0->unk4, arg0->unk8, 0, &sp50);
    if (temp_v0 != 0) {
        return temp_v0;
    }
    if (func_8000F8D0(&sp70, &sp50, 0x20) != 0) {
        return 0xB;
    }
    return func_8000F680(arg0->unk4, arg0->unk8, 0, &sp30, 0);
}
