? func_80008590(? *, ?);                            /* extern */
s32 func_8000A350(void *, s32, s32, s32 *);         /* extern */
s32 func_8000E850(void *, ?);                       /* extern */
s32 func_8000E8D0(s32, s32, s32, ? *);              /* extern */
s32 func_8000F3A4(void *, ? *, ?, s32);             /* extern */
s32 func_8000F680(s32, s32, s32, ? *, s32);         /* extern */
s32 func_8000A8D8(void *arg0, ? *arg1, s32 arg2, s32 arg3, u16 *arg4); /* static */

s32 func_8000A700(void *arg0, s32 arg1, s32 arg2, s32 arg4) {
    s32 sp16C;
    ? sp68;
    ? sp48;
    u16 sp44;
    s32 temp_t6;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    u8 var_s0;
    u8 var_s3;

    temp_t6 = arg1 & 0xFFFF;
    if ((temp_t6 == 0) || (arg2 == 0)) {
        return 5;
    }
    temp_v0_6 = func_8000A350(arg0, temp_t6, arg4, &sp16C);
    if (temp_v0_6 != 0) {
        return temp_v0_6;
    }
    if (arg0->unk65 != 0) {
        temp_v0_5 = func_8000E850(arg0, 0);
        if (temp_v0_5 != 0) {
            return temp_v0_5;
        }
    }
    temp_v0_4 = func_8000E8D0(arg0->unk4, arg0->unk8, (arg0->unk5C + sp16C) & 0xFFFF, &sp48);
    var_s0 = sp4E;
    if (temp_v0_4 != 0) {
        return temp_v0_4;
    }
    var_s3 = sp4F;
    if ((s32) var_s0 < (s32) arg0->unk64) {
loop_12:
        temp_v0_3 = func_8000F3A4(arg0, &sp68, 0, var_s0 & 0xFF);
        if (temp_v0_3 != 0) {
            return temp_v0_3;
        }
        temp_v0_2 = func_8000A8D8(arg0, &sp68, var_s3 & 0xFF, var_s0 & 0xFF, &sp44);
        if (temp_v0_2 != 0) {
            return temp_v0_2;
        }
        temp_v0 = func_8000F3A4(arg0, &sp68, 1, var_s0 & 0xFF);
        if (temp_v0 != 0) {
            return temp_v0;
        }
        if (sp44 == 1) {
            goto block_21;
        }
        var_s0 = (u8) sp44;
        var_s3 = unksp45;
        if ((s32) var_s0 >= (s32) arg0->unk64) {
            goto block_21;
        }
        goto loop_12;
    }
block_21:
    if ((s32) var_s0 >= (s32) arg0->unk64) {
        return 3;
    }
    func_80008590(&sp48, 0x20);
    return func_8000F680(arg0->unk4, arg0->unk8, (arg0->unk5C + sp16C) & 0xFFFF, &sp48, 0);
}

s32 func_8000A8D8(void *arg0, ? *arg1, s32 arg2, s32 arg3, u16 *arg4) {
    u16 spC;
    u16 sp8;
    s32 temp_v0;

    temp_v0 = arg3 & 0xFF;
    spC = (temp_v0 << 8) + (arg2 & 0xFF);
loop_1:
    sp8 = spC;
    spC = *(arg1 + (unkspD * 2));
    *(arg1 + (sp9 * 2)) = 3;
    if ((s32) spC >= arg0->unk60) {
        if (temp_v0 == (u8) spC) {
            goto loop_1;
        }
    }
    *arg4 = spC;
    return 0;
}
