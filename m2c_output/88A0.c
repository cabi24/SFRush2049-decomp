void func_80007CA0(u32 arg0, s32 arg1) {
    u32 temp_t1;
    u32 var_t0;
    u32 var_t0_2;

    if (arg1 > 0) {
        if ((u32) arg1 < 0x2000U) {
            temp_t1 = arg0 + arg1;
            if (arg0 < temp_t1) {
                var_t0 = arg0 - (arg0 & 0xF);
                do {
                    M2C_ERROR(/* unknown instruction: cache 0x19, ($t0) */);
                    var_t0 += 0x10;
                } while (var_t0 < (u32) (temp_t1 - 0x10));
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
