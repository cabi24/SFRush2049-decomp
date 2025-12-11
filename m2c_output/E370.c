void func_8000D770(void) {
    M2C_ERROR(/* mtc0 $a0, $12 */);
}

s32 func_8000D780(void) {
    return M2C_ERROR(/* mfc0 $12 */);
}
