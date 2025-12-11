s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
extern void *D_8002C464;

void func_80006ED0(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_a0;

    temp_a0 = func_8000C4B0();
    D_8002C464->unk10 = arg0;
    D_8002C464->unk14 = arg1;
    D_8002C464->unk2 = (s16) arg2;
    func_8000C520(temp_a0);
}
