s32 func_8000F680(s32, s32, ?, ? *, s32);           /* extern */

s32 func_8000E850(void *arg0, s32 arg1) {
    ? sp38;
    ? *var_v0;
    s32 temp_v0;
    s8 temp_s0;

    temp_s0 = arg1 & 0xFF;
    var_v0 = &sp38;
    do {
        var_v0 += 4;
        var_v0->unk-3 = temp_s0;
        var_v0->unk-2 = temp_s0;
        var_v0->unk-1 = temp_s0;
        var_v0->unk-4 = temp_s0;
    } while (var_v0 != &arg0);
    temp_v0 = func_8000F680(arg0->unk4, arg0->unk8, 0x400, &sp38, 0);
    if (temp_v0 == 0) {
        arg0->unk65 = temp_s0;
    }
    return temp_v0;
}
