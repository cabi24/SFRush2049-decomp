/*@@HDR 0 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3227@@*/

/*@@HDR 3228 3697@@*/
#endif

u8 func_800CDE38(void **arg0) {
    u8 *p;

    p = *M2C_FIELD(*arg0, u8 ***, 0x2C);
    switch (p[0x47]) {
    case 6:
        return p[0x63];
    case 4:
        return p[0x73];
    case 5:
        return p[0x87];
    }
    return p[0x4F];
}
