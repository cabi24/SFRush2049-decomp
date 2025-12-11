u8 *func_80002730(u8 *arg0, s32 arg1, s32 arg2) {
    u8 *sp4;
    s32 var_a2;

    sp4 = arg0;
    var_a2 = arg2 - 1;
    if (arg2 != 0) {
loop_1:
        if (*sp4 == arg1) {
            return sp4;
        }
        sp4 += 1;
        var_a2 -= 1;
        if (var_a2 == 0) {
            goto block_4;
        }
        goto loop_1;
    }
block_4:
    return NULL;
}
