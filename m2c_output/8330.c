s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
s32 func_8000D2A0();                                /* extern */
extern void *D_8002C460;

s32 func_80007730(void *arg0) {
    s32 temp_t9;
    s32 temp_v0;
    s32 var_v1;

    temp_v0 = func_8000D2A0();
    if (temp_v0 & 0x100) {
        var_v1 = 1;
    } else {
        var_v1 = 0;
    }
    if (temp_v0 & 0x80) {
        temp_t9 = arg0->unk4 | var_v1;
        arg0->unk4 = temp_t9;
        arg0->unk4 = (s32) (temp_t9 & ~2);
    }
    return var_v1;
}

s32 func_80007790(void) {
    s32 sp18;
    s32 temp_a0;

    temp_a0 = func_8000C4B0();
    sp18 = D_8002C460->unk4;
    func_8000C520(temp_a0);
    return sp18;
}
