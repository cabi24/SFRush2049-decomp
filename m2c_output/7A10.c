? func_800075E0(s32, s32, ?);                       /* extern */
s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
extern s32 D_8002C350;
extern s32 D_8002C36C;
extern ? D_80036710;

void func_80006E10(s32 arg0, s32 arg1, s32 arg2) {
    s32 *sp20;
    s32 *temp_t9;
    s32 temp_v0;

    temp_v0 = func_8000C4B0();
    temp_t9 = (arg0 * 8) + &D_80036710;
    sp20 = temp_t9;
    *temp_t9 = arg1;
    sp20->unk4 = arg2;
    if (arg0 == 0xE) {
        if ((D_8002C36C != 0) && (D_8002C350 == 0)) {
            func_800075E0(arg1, arg2, 0);
        }
        D_8002C350 = 1;
    }
    func_8000C520(temp_v0);
}
