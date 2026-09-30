/*@@HDR 1 3697@@*/
#endif

u8 *func_800A4770(u8 *arg0, u8 *arg1) {
    u8 *temp_v1;
    u8 *var_a0;
    u8 *var_a1;
    u8 temp_t7;
    u8 temp_v0;
    u8 temp_v0_2;
    void *var_a0_2;

    var_a0 = arg0;
    temp_v1 = var_a0;
    if (*var_a0 != 0) {
        do {
            temp_t7 = M2C_FIELD(var_a0, u8 *, 1);
            var_a0 += 1;
        } while (temp_t7 != 0);
    }
    temp_v0 = *arg1;
    var_a0_2 = var_a0 + 1;
    var_a1 = arg1 + 1;
    M2C_FIELD(var_a0_2, u8 *, -1) = temp_v0;
    if (temp_v0 != 0) {
        do {
            temp_v0_2 = *var_a1;
            var_a0_2 = (u8 *) var_a0_2 + 1;
            var_a1 += 1;
            M2C_FIELD(var_a0_2, u8 *, -1) = temp_v0_2;
        } while (temp_v0_2 != 0);
    }
    return temp_v1;
}
