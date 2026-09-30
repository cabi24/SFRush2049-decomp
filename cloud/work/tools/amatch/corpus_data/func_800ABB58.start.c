/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3119@@*/

/*@@HDR 3120 3697@@*/
#endif

f32 func_800ABB58(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    void *temp_a1;

    temp_v0 = arg0 >> 0xA;
    if (temp_v0 >= (s32) D_80140BDC) {
        return 0.0f;
    }
    temp_a1 = (s32 *) ((temp_v0 * 8) + (u8 *) &D_801161F4);
    temp_v1 = arg0 & 0x3FF;
    if (temp_v1 >= M2C_FIELD(temp_a1, s32 *, 4)) {
        return 0.0f;
    }
    return M2C_FIELD((M2C_FIELD(temp_a1, s32 *, 0) + (temp_v1 * 0x58)), f32 *, 0x10);
}
