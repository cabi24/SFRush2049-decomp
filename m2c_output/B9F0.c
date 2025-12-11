s32 func_8000E620(s32, s32);                        /* extern */
s32 func_8000E850(void *, u8);                      /* extern */
s32 func_8000E8D0(s32, s32, s32, s32 *);            /* extern */
s32 func_8000F2D0(void *);                          /* extern */
s32 func_8000F3A4(? *, ?, u8);                      /* extern */
s32 func_8000F680(s32, s32, s32, s32 *, s32);       /* extern */

s32 func_8000ADF0(void *arg0, u8 *arg1, ? *arg2, u16 *arg3) {
    s32 temp_v0;
    s32 temp_v1;
    s32 var_v0;
    u16 temp_at;
    u8 temp_v0_2;
    u8 temp_v0_3;

    temp_v0_2 = arg3->unk0;
    if (temp_v0_2 != *arg1) {
        *arg1 = temp_v0_2;
        temp_v0 = func_8000F3A4(arg2, 0, *arg1);
        if (temp_v0 != 0) {
            return temp_v0;
        }
    }
    temp_at = *(arg2 + (arg3->unk1 * 2));
    arg3->unk0 = temp_at;
    temp_v1 = temp_at & 0xFFFF;
    if ((temp_v1 < arg0->unk60) || ((s32) arg3->unk0 >= (s32) arg0->unk64) || (temp_v0_3 = arg3->unk1, ((s32) temp_v0_3 <= 0)) || (var_v0 = 0, (((s32) temp_v0_3 < 0x80) == 0))) {
        if (temp_v1 == 1) {
            return 5;
        }
        var_v0 = 3;
        /* Duplicate return node #10. Try simplifying control flow for better match */
        return var_v0;
    }
    return var_v0;
}

s32 func_8000AEC4(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 *arg5) {
    u8 sp154;
    s32 sp14C;
    ? sp4C;
    u16 sp48;
    u8 sp3B;
    s32 sp34;
    s32 *var_s1;
    s32 temp_s0;
    s32 temp_t3;
    s32 temp_t8;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 temp_v0_7;
    s32 temp_v0_8;
    s32 var_s0;
    s32 var_s3;
    s32 var_v0;

    temp_s0 = arg2 & 0xFF;
    if ((arg1 >= arg0->unk50) || (arg1 < 0)) {
        return 5;
    }
    if ((arg4 <= 0) || (arg4 & 0x1F)) {
        return 5;
    }
    if ((arg3 < 0) || (arg3 & 0x1F)) {
        return 5;
    }
    if (!(arg0->unk0 & 1)) {
        return 5;
    }
    if (func_8000F2D0(arg0) == 2) {
        return 2;
    }
    if (arg0->unk65 != 0) {
        temp_v0_7 = func_8000E850(arg0, 0U);
        if (temp_v0_7 != 0) {
            return temp_v0_7;
        }
    }
    temp_v0_6 = func_8000E8D0(arg0->unk4, arg0->unk8, (arg0->unk5C + arg1) & 0xFFFF, &sp14C);
    if (temp_v0_6 != 0) {
        return temp_v0_6;
    }
    if ((sp150 == 0) || (sp14C == 0)) {
        return 5;
    }
    if (((s32) sp152 < arg0->unk60) || ((s32) (u8) sp152 >= (s32) arg0->unk64) || ((s32) unksp153 <= 0) || ((s32) unksp153 >= 0x80)) {
        if (sp152 == 1) {
            return 5;
        }
        return 3;
    }
    sp34 = temp_s0;
    if ((temp_s0 == 0) && !(sp154 & 2)) {
        return 6;
    }
    sp3B = 0xFF;
    temp_v0_8 = arg3 / 32;
    var_s0 = temp_v0_8;
    sp48 = sp152;
    if (temp_v0_8 >= 8) {
loop_32:
        temp_v0_5 = func_8000ADF0(arg0, &sp3B, &sp4C, &sp48);
        var_s0 -= 8;
        if (temp_v0_5 != 0) {
            return temp_v0_5;
        }
        if (var_s0 < 8) {
            goto block_35;
        }
        goto loop_32;
    }
block_35:
    var_s1 = arg5;
    temp_t8 = arg4 / 32;
    var_s3 = temp_t8;
    if (temp_t8 > 0) {
loop_36:
        if (var_s0 == 8) {
            temp_v0_4 = func_8000ADF0(arg0, &sp3B, &sp4C, &sp48);
            var_s0 = 0;
            if (temp_v0_4 != 0) {
                return temp_v0_4;
            }
        }
        if ((u8) sp48 != arg0->unk65) {
            temp_v0_3 = func_8000E850(arg0, (u8) sp48);
            if (temp_v0_3 != 0) {
                return temp_v0_3;
            }
        }
        temp_t3 = ((unksp49 * 8) + var_s0) & 0xFFFF;
        if (sp34 == 0) {
            var_v0 = func_8000E8D0(arg0->unk4, arg0->unk8, temp_t3, var_s1);
        } else {
            var_v0 = func_8000F680(arg0->unk4, arg0->unk8, temp_t3, var_s1, 0);
        }
        var_s1 += 0x20;
        if (var_v0 != 0) {
            return var_v0;
        }
        var_s3 -= 1;
        var_s0 += 1;
        if (var_s3 == 0) {
            goto block_48;
        }
        goto loop_36;
    }
block_48:
    if ((sp34 == 1) && !(sp154 & 2)) {
        sp154 |= 2;
        if (arg0->unk65 != 0) {
            temp_v0_2 = func_8000E850(arg0, 0U);
            if (temp_v0_2 != 0) {
                return temp_v0_2;
            }
        }
        temp_v0 = func_8000F680(arg0->unk4, arg0->unk8, (arg0->unk5C + arg1) & 0xFFFF, &sp14C, 0);
        if (temp_v0 != 0) {
            return temp_v0;
        }
        goto block_55;
    }
block_55:
    return func_8000E620(arg0->unk4, arg0->unk8);
}
