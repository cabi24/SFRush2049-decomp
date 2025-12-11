s32 func_8000D5C0(s32);                             /* extern */
? func_8000D640(?);                                 /* extern */
s32 func_8000D740();                                /* extern */
extern s32 DPC_END_REG;
extern s32 DPC_START_REG;
extern s32 DPC_STATUS_REG;

s32 func_80007B00(s32 arg0, ? arg2, s32 arg3) {
    if (func_8000D740() != 0) {
        return -1;
    }
    DPC_STATUS_REG = 1;
    do {

    } while (DPC_STATUS_REG & 1);
    DPC_START_REG = func_8000D5C0(arg0);
    DPC_END_REG = func_8000D5C0(arg0) + arg3;
    return 0;
}

void func_80007B80(void) {
    func_8000D640(0x400);
}
