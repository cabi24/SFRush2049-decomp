extern s32 SP_PC;
extern s32 SP_STATUS_REG;

s32 func_8000D650(s32 arg0) {
    if (!(SP_STATUS_REG & 1)) {
        return -1;
    }
    SP_PC = arg0;
    return 0;
}
