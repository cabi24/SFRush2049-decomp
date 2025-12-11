s32 func_8000FCB0(u32);                             /* extern */

s32 func_8000D5C0(u32 arg0) {
    if ((arg0 >= 0x80000000U) && (arg0 < 0xA0000000U)) {
        return arg0 & 0x1FFFFFFF;
    }
    if ((arg0 >= 0xA0000000U) && (arg0 < 0xC0000000U)) {
        return arg0 & 0x1FFFFFFF;
    }
    return func_8000FCB0(arg0);
}
