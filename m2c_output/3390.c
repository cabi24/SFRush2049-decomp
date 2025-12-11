s32 func_80002790(s32 arg0, s8 arg1, u32 arg2) {
    s32 spC;
    s32 sp8;
    s32 sp4;
    s32 var_a2_2;
    u32 var_a2;

    var_a2 = arg2;
    spC = arg0;
    if (arg1 == 0) {
        sp8 = spC & 3;
        if ((var_a2 != 0) && (sp8 > 0) && (sp8 != 4)) {
loop_4:
            *spC = 0;
            spC += 1;
            sp8 += 1;
            var_a2 -= 1;
            if ((var_a2 != 0) && (sp8 > 0)) {
                if (sp8 != 4) {
                    goto loop_4;
                }
            }
        }
        sp4 = spC;
        if (var_a2 >= 4U) {
            do {
                *sp4 = 0;
                sp4 += 4;
                var_a2 -= 4;
            } while (var_a2 >= 4U);
        }
        spC = sp4;
    }
    var_a2_2 = var_a2 - 1;
    if (var_a2 != 0) {
        do {
            *spC = arg1;
            spC += 1;
            var_a2_2 -= 1;
        } while (var_a2_2 != 0);
    }
    return arg0;
}
