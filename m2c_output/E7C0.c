? func_80006A00(? *, ? *, ?);                       /* extern */
? func_80007270(? *, ? *, ?);                       /* extern */
? func_800075E0(? *, ?, ?);                         /* extern */
extern s32 D_8002C4A0;
extern ? D_80037C70;
extern ? D_80037C78;
extern s32 PI_STATUS_REG;
extern s32 osRomBase;

void func_8000DBC0(void) {
    D_8002C4A0 = 1;
    func_80006A00(&D_80037C78, &D_80037C70, 1);
    func_800075E0(&D_80037C78, 0, 0);
}

void func_8000DC10(void) {
    ? sp1C;

    if (D_8002C4A0 == 0) {
        func_8000DBC0();
    }
    func_80007270(&D_80037C78, &sp1C, 1);
}

void func_8000DC54(void) {
    func_800075E0(&D_80037C78, 0, 0);
}

s32 func_8000DC80(s32 arg0, s32 *arg1) {
    if (PI_STATUS_REG & 3) {
        do {

        } while (PI_STATUS_REG & 3);
    }
    *arg1 = *(osRomBase | arg0 | 0xA0000000);
    return 0;
}
