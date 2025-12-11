extern s32 D_8002C370;
extern ? D_8002C3E0;

s32 func_8000C4B0(void) {
    M2C_ERROR(/* mtc0 $t1, $12 */);
    if ((D_8002C370 & 0xFF00) != (D_8002C370 & 0xFF00)) {
        M2C_ERROR(/* mtc0 $t1, $12 */);
    }
    return M2C_ERROR(/* mfc0 $12 */) & 1;
}

void func_8000C520(s32 arg0) {
    M2C_ERROR(/* mtc0 $t0, $12 */);
}
