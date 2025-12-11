void func_80007810(void) {
    u32 var_t0;

    var_t0 = 0x80000000;
    do {
        M2C_ERROR(/* unknown instruction: cache 0x1, ($t0) */);
        var_t0 += 0x10;
    } while (var_t0 < 0x80001FF0U);
}
