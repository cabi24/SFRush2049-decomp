/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3226@@*/

/*@@HDR 3227 3697@@*/
#endif

u8 func_800CDDE8(void **arg0) {
    u8 temp_v0;
    void *temp_v1;

    temp_v1 = *M2C_FIELD(*arg0, void ***, 0x2C);
    temp_v0 = M2C_FIELD(temp_v1, u8 *, 0x47);
    if (temp_v0 != 4) {
        if (temp_v0 != 5) {
            if (temp_v0 == 6) {
                return M2C_FIELD(temp_v1, u8 *, 0x62);
            }
            return M2C_FIELD(temp_v1, u8 *, 0x4E);
        }
        return M2C_FIELD(temp_v1, u8 *, 0x86);
    }
    return M2C_FIELD(temp_v1, u8 *, 0x72);
}
