s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
extern u16 *D_8002C464;

void func_80006DA0(u8 arg0) {
    s32 temp_a0;

    temp_a0 = func_8000C4B0();
    if (arg0 != 0) {
        *D_8002C464 |= 0x20;
    } else {
        *D_8002C464 &= 0xFFDF;
    }
    func_8000C520(temp_a0);
}
