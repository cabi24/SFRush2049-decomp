/*@@HDR 0 3697@@*/
#endif

u8 *func_800A4770(u8 *arg0, u8 *arg1) {
    u8 *r = arg0;
    while (*arg0 != 0) {
        arg0++;
    }
    while ((*arg0++ = *arg1++) != 0) {
    }
    return r;
}
