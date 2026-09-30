s32 func_800B98D8(s32 arg0, s32 arg1) {
    s32 *var_v1;
    s32 *var_v1_2;
    s32 temp_a3;
    s32 var_v0;
    s8 temp_a1;
    s8 temp_a1_2;
    void *temp_v1;

    if (arg0 >= 0) {
        temp_v1 = D_801407FC + (arg0 * 0x10);
        if ((M2C_FIELD(temp_v1, u8 *, 0) != 0) && (((temp_a1 = M2C_FIELD(temp_v1, s8 *, 1), (temp_a1 >= 0)) && (*(D_801407FC + (temp_a1 * 0x10)) == 0)) || ((temp_a1_2 = M2C_FIELD(temp_v1, s8 *, 4), (temp_a1_2 >= 0)) && (*(D_801407FC + (temp_a1_2 * 0x10)) == 0)))) {
            return 0;
        }
    }
    (&D_80143A88)[arg1] = arg0;
    var_v0 = 0;
    if (arg1 > 0) {
        temp_a3 = arg1 & 3;
        if (temp_a3 != 0) {
            var_v1 = &(&D_80143A88)[0];
loop_10:
            var_v0 += 1;
            if (arg0 == *var_v1) {
                return 0;
            }
            var_v1 = var_v1 + 1;
            if (temp_a3 == var_v0) {
                if (var_v0 != arg1) {
                    goto block_14;
                }
                /* Duplicate return node #24. Try simplifying control flow for better match */
                return 1;
            }
            goto loop_10;
        }
block_14:
        var_v1_2 = &(&D_80143A88)[var_v0];
loop_15:
        var_v0 += 4;
        if (arg0 == M2C_FIELD(var_v1_2, s32 *, 0)) {
            return 0;
        }
        if (arg0 == M2C_FIELD(var_v1_2, s32 *, 4)) {
            return 0;
        }
        if (arg0 == M2C_FIELD(var_v1_2, s32 *, 8)) {
            return 0;
        }
        if (arg0 == M2C_FIELD(var_v1_2, s32 *, 0xC)) {
            return 0;
        }
        var_v1_2 = var_v1_2 + 4;
        if (var_v0 == arg1) {
            /* Duplicate return node #24. Try simplifying control flow for better match */
            return 1;
        }
        goto loop_15;
    }
    return 1;
}