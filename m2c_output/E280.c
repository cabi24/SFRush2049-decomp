s32 func_8000D5C0(s32);                             /* extern */
s32 func_8000D710();                                /* extern */
extern s32 SP_DRAM_ADDR_REG;
extern s32 SP_MEM_ADDR_REG;
extern s32 SP_RD_LEN_REG;
extern s32 SP_WR_LEN_REG;

s32 func_8000D680(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (func_8000D710() != 0) {
        return -1;
    }
    SP_MEM_ADDR_REG = arg1;
    SP_DRAM_ADDR_REG = func_8000D5C0(arg2);
    if (arg0 == 0) {
        SP_WR_LEN_REG = arg3 - 1;
        return 0;
    }
    SP_RD_LEN_REG = arg3 - 1;
    return 0;
}
