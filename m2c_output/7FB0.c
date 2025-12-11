s32 func_8000C4B0();                                /* extern */
? func_8000C520(s32);                               /* extern */
s32 func_8000C970();                                /* extern */
extern s32 D_80037C50;
extern u32 D_80037C54;
extern s32 D_80037C58;

s32 func_800073B0(void) {
    s32 sp34;
    s32 sp30;
    u32 sp2C;
    s32 sp28;
    s32 temp_s0;

    temp_s0 = func_8000C4B0();
    sp34 = func_8000C970();
    sp30 = sp34 - D_80037C58;
    sp2C = D_80037C54;
    sp28 = D_80037C50;
    func_8000C520(temp_s0);
    return ((u32) (sp30 + sp2C) < sp2C) + sp28;
}
