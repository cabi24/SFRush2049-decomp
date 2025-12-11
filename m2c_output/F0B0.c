? func_80007CA0(s32, ?, s32);                       /* extern */
? func_800084E0(s32, ?, s32);                       /* extern */
s32 func_8000D5C0(s32);                             /* extern */
extern s32 SI_DRAM_ADDR_REG;
extern s32 SI_PIF_ADDR_RD64B_REG;
extern s32 SI_PIF_ADDR_WR64B_REG;
extern s32 SI_STATUS_REG;

s32 func_8000E4B0(s32 arg0, s32 arg1) {
    if (SI_STATUS_REG & 3) {
        return -1;
    }
    if (arg0 == 1) {
        func_80007CA0(arg1, 0x40, arg0);
    }
    SI_DRAM_ADDR_REG = func_8000D5C0(arg1);
    if (arg0 == 0) {
        SI_PIF_ADDR_RD64B_REG = 0x1FC007C0;
    } else {
        SI_PIF_ADDR_WR64B_REG = 0x1FC007C0;
    }
    if (arg0 == 0) {
        func_800084E0(arg1, 0x40, arg0);
    }
    return 0;
}
