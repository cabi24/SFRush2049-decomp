? func_80008590(? *, ?);                            /* extern */
? func_8000C670(? **, ?, ?);                        /* extern */
extern ? D_8002C400;
extern ? D_8002C430;
extern ? *D_8002C460;
extern ? *D_8002C464;
extern ? D_8002C4E0;
extern ? D_8002C530;
extern ? D_8002C580;
extern u32 VI_CURRENT_REG;
extern s32 VI_STATUS_REG;
extern s32 osTvType;

void func_8000C540(void) {
    func_80008590(&D_8002C400, 0x60);
    D_8002C460 = &D_8002C400;
    D_8002C464 = &D_8002C430;
    D_8002C430.unk2 = 1;
    D_8002C460->unk2 = 1;
    D_8002C464->unk4 = 0x80000000;
    D_8002C460->unk4 = 0x80000000;
    if (osTvType == 0) {
        D_8002C464->unk8 = &D_8002C4E0;
    } else if (osTvType == 2) {
        D_8002C464->unk8 = &D_8002C530;
    } else {
        D_8002C464->unk8 = &D_8002C580;
    }
    D_8002C464->unk0 = 0x20;
    D_8002C464->unkC = (s32) D_8002C464->unk8->unk4;
    if ((u32) VI_CURRENT_REG >= 0xBU) {
        do {

        } while ((u32) VI_CURRENT_REG >= 0xBU);
    }
    VI_STATUS_REG = 0;
    func_8000C670(&D_8002C464, 1, 0x80000000);
}
