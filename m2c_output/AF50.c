s32 func_8000E620(s32, s32);                        /* extern */
s32 func_8000E8D0(s32, s32, s32, s32 *);            /* extern */
s32 func_8000F2D0(void *);                          /* extern */

s32 func_8000A350(void *arg0, s32 arg1, s32 arg2, u8 *arg3, u8 *arg4, s32 *arg5) {
    ? sp60;
    u16 sp54;
    s32 sp50;
    s32 *var_v0;
    s32 *var_v0_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 var_a0;
    s32 var_s3;
    u8 *var_v1;
    u8 *var_v1_2;
    u8 temp_t3;
    u8 temp_t5;

    if (!(arg0->unk0 & 1)) {
        return 5;
    }
    temp_v0_3 = func_8000F2D0(arg0);
    if (temp_v0_3 != 0) {
        return temp_v0_3;
    }
    var_s3 = 0;
    if (arg0->unk50 > 0) {
loop_6:
        temp_v0_2 = func_8000E8D0(arg0->unk4, arg0->unk8, (arg0->unk5C + var_s3) & 0xFFFF, &sp50);
        if (temp_v0_2 != 0) {
            return temp_v0_2;
        }
        temp_v0 = func_8000E620(arg0->unk4, arg0->unk8);
        if (temp_v0 != 0) {
            return temp_v0;
        }
        if (((arg1 & 0xFFFF) == sp54) && (sp50 == arg2)) {
            var_a0 = 0;
            if (arg3 != NULL) {
                var_v0 = &sp50;
                var_v1 = arg3;
loop_14:
                temp_t3 = var_v0->unk10;
                var_v0 += 1;
                if (temp_t3 != *var_v1) {
                    var_a0 = 1;
                } else {
                    var_v1 += 1;
                    if (var_v0 != &sp60) {
                        goto loop_14;
                    }
                }
            }
            if (arg4 != NULL) {
                var_v0_2 = &sp50;
                if (var_a0 == 0) {
                    var_v1_2 = arg4;
loop_20:
                    temp_t5 = var_v0_2->unkC;
                    var_v0_2 += 1;
                    if (temp_t5 != *var_v1_2) {
                        var_a0 = 1;
                    } else {
                        var_v1_2 += 1;
                        if (var_v0_2 != &sp54) {
                            goto loop_20;
                        }
                    }
                }
            }
            if (var_a0 == 0) {
                *arg5 = var_s3;
                return temp_v0;
            }
            goto block_25;
        }
block_25:
        var_s3 += 1;
        if (var_s3 >= arg0->unk50) {
            goto block_26;
        }
        goto loop_6;
    }
block_26:
    *arg5 = -1;
    return 5;
}
