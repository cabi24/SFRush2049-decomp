? func_8000D2B0(? *, void *, ?);                    /* extern */
s32 func_8000E620(s32, s32);                        /* extern */
s32 func_8000E850(void *, ?);                       /* extern */
s32 func_8000E8D0(s32, s32, s32, s32 *);            /* extern */
s32 func_8000F2D0(void *);                          /* extern */
s32 func_8000F3A4(void *, u16 (*)[], ?, s32);       /* extern */

s32 func_8000A520(void *arg0, s32 arg1, void *arg2) {
    ? sp48;
    ? sp44;
    u16 sp3E;
    s32 sp38;
    u16 sp34;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_s2;
    u16 var_at;
    u8 var_s0;

    if ((arg1 >= arg0->unk50) || (arg1 < 0)) {
        return 5;
    }
    if (!(arg0->unk0 & 1)) {
        return 5;
    }
    temp_v0_4 = func_8000F2D0(arg0);
    if (temp_v0_4 != 0) {
        return temp_v0_4;
    }
    if (arg0->unk65 != 0) {
        temp_v0_3 = func_8000E850(arg0, 0);
        if (temp_v0_3 != 0) {
            return temp_v0_3;
        }
    }
    temp_v0_2 = func_8000E8D0(arg0->unk4, arg0->unk8, (arg0->unk5C + arg1) & 0xFFFF, &sp38);
    if (temp_v0_2 != 0) {
        return temp_v0_2;
    }
    if ((sp3C == 0) || (sp38 == 0)) {
        return 5;
    }
    var_at = sp3E;
    var_s2 = 0;
    var_s0 = 0xFF;
loop_16:
    sp34 = var_at;
    if ((s32) sp34 >= arg0->unk60) {
        if (var_s0 != (u8) sp34) {
            var_s0 = (u8) sp34;
            temp_v0 = func_8000F3A4(arg0, (u16 (*)[]) &sp58[0], 0, (u8) sp34 & 0xFF);
            if (temp_v0 != 0) {
                return temp_v0;
            }
        }
        var_s2 += 1;
        var_at = (&sp58[0])[unksp35];
        goto loop_16;
    }
    if (sp34 != 1) {
        return 3;
    }
    arg2->unk0 = (s32) (var_s2 << 8);
    arg2->unk8 = sp3C;
    arg2->unk4 = sp38;
    func_8000D2B0(&sp48, arg2 + 0xE, 0x10);
    func_8000D2B0(&sp44, arg2 + 0xA, 4);
    return func_8000E620(arg0->unk4, arg0->unk8);
}
