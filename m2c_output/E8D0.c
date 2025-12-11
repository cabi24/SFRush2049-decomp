s32 func_8000D5C0(?);                               /* extern */
extern s32 PI_CART_ADDR_REG;
extern s32 PI_DRAM_ADDR_REG;
extern s32 PI_RD_LEN_REG;
extern s32 PI_STATUS_REG;
extern s32 PI_WR_LEN_REG;
extern s32 osRomBase;

s32 func_8000DCD0(s32 arg0, s32 arg1, ? arg2, s32 arg3) {
    if (PI_STATUS_REG & 3) {
        do {

        } while (PI_STATUS_REG & 3);
    }
    PI_DRAM_ADDR_REG = func_8000D5C0(arg2);
    PI_CART_ADDR_REG = (osRomBase | arg1) & 0x1FFFFFFF;
    switch (arg0) {                                 /* irregular */
    case 0:
        PI_WR_LEN_REG = arg3 - 1;
block_8:
        return 0;
    case 1:
        PI_RD_LEN_REG = arg3 - 1;
        goto block_8;
    default:
        return -1;
    }
}
