/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3591@@*/

/*@@HDR 3592 3697@@*/
#endif

s32 struct_init_and_call(s32 arg0, s16 arg1, s32 arg2) {
    if (arg1 != 0) {
        M2C_FIELD(&D_80153F10, s32 *, 8) = arg0;
        M2C_FIELD(&D_80153F10, s16 *, 2) = arg1;
        M2C_FIELD(&D_80153F10, s32 *, 0xC) = arg2;
        M2C_FIELD(&D_80153F10, s16 *, 4) = 0;
        M2C_FIELD(&D_80153F10, s8 *, 0) = 1;
        func_8008ABE4();
    }
    return 1;
}
