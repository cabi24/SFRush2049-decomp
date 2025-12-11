s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
extern void *D_8002C464;

s32 func_800077D0(void) {
    s32 sp18;
    s32 temp_a0;

    temp_a0 = func_8000C4B0();
    sp18 = D_8002C464->unk4;
    func_8000C520(temp_a0);
    return sp18;
}
