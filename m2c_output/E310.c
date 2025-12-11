extern s32 SP_STATUS_REG;

s32 func_8000D710(void) {
    if (SP_STATUS_REG & 0x1C) {
        return 1;
    }
    return 0;
}
