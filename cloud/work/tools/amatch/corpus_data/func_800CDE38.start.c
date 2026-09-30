/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3227@@*/

/*@@HDR 3228 3697@@*/
#endif

u8 func_800CDE38(void **arg0) {
    u8 temp_v0;
    void *temp_v1;

    temp_v1 = *M2C_FIELD(*arg0, void ***, 0x2C);
    temp_v0 = M2C_FIELD(temp_v1, u8 *, 0x47);
    if (temp_v0 != 4) {
        if (temp_v0 != 5) {
            if (temp_v0 == 6) {
                return M2C_FIELD(temp_v1, u8 *, 0x63);
            }
            return M2C_FIELD(temp_v1, u8 *, 0x4F);
        }
        return M2C_FIELD(temp_v1, u8 *, 0x87);
    }
    return M2C_FIELD(temp_v1, u8 *, 0x73);
}
