s32 func_8000FD70();                                /* extern */
extern ? D_A0000000;

s32 func_8000D790(void) {
    return M2C_ERROR(/* cfc1 */);
}

s32 func_8000D7A0(s32 arg0, s32 *arg1) {
    if (func_8000FD70() != 0) {
        return -1;
    }
    *arg1 = *(arg0 | (s32) &D_A0000000);
    return 0;
}
