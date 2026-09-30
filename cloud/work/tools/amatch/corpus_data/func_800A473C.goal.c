/*@@HDR 0 3085@@*/

u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3697@@*/
#endif

u8 *func_800A473C(u8 *arg0, u8 *arg1) {
    u8 *r = arg0;
    while ((*arg0++ = *arg1++) != 0) {
    }
    return r;
}
