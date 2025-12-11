extern s32 DPC_STATUS_REG;

s32 func_8000D740(void) {
    if (DPC_STATUS_REG & 0x100) {
        return 1;
    }
    return 0;
}
