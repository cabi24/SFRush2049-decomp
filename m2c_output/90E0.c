void func_800084E0(u32 arg0, s32 arg1) {
    s32 temp_t2;
    s32 temp_t2_2;
    u32 temp_t0;
    u32 temp_t1;
    u32 var_t0;
    u32 var_t0_2;
    u32 var_t1;

    if (arg1 > 0) {
        if ((u32) arg1 < 0x2000U) {
            var_t0 = arg0;
            temp_t1 = arg0 + arg1;
            if (var_t0 < temp_t1) {
                temp_t2 = var_t0 & 0xF;
                var_t1 = temp_t1 - 0x10;
                if (temp_t2 != 0) {
                    temp_t0 = var_t0 - temp_t2;
                    M2C_ERROR(/* unknown instruction: cache 0x15, ($t0) */);
                    if (temp_t0 < var_t1) {
                        var_t0 = temp_t0 + 0x10;
                        goto block_6;
                    }
                } else {
block_6:
                    temp_t2_2 = var_t1 & 0xF;
                    if ((temp_t2_2 == 0) || (var_t1 -= temp_t2_2, M2C_ERROR(/* unknown instruction: cache 0x15, 0x10($t1) */), ((var_t1 < var_t0) == 0))) {
                        do {
                            M2C_ERROR(/* unknown instruction: cache 0x11, ($t0) */);
                            var_t0 += 0x10;
                        } while (var_t0 < var_t1);
                    }
                }
            }
        } else {
            var_t0_2 = 0x80000000;
            do {
                M2C_ERROR(/* unknown instruction: cache 0x1, ($t0) */);
                var_t0_2 += 0x10;
            } while (var_t0_2 < 0x80001FF0U);
        }
    }
}
