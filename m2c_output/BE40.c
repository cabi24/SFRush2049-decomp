s32 func_8000F2D0(void *);                          /* extern */
s32 func_8000F3A4(void *, u16 (*)[], ?, s32);       /* extern */

s32 func_8000B240(void *arg0, s32 *arg1) {
    ? sp148;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_a1;
    s32 var_s0;
    s32 var_s3;
    s32 var_v1;
    u16 *var_v0;
    u16 *var_v0_2;
    u16 temp_t8;

    var_s0 = 0;
    if (!(arg0->unk0 & 1)) {
        return 5;
    }
    temp_v0_2 = func_8000F2D0(arg0);
    if (temp_v0_2 != 0) {
        return temp_v0_2;
    }
    var_s3 = 0;
    if ((s32) arg0->unk64 > 0) {
loop_6:
        temp_v0 = func_8000F3A4(arg0, (u16 (*)[]) &sp48[0], 0, var_s3 & 0xFF);
        if (temp_v0 != 0) {
            return temp_v0;
        }
        if (var_s3 > 0) {
            var_a1 = 1;
        } else {
            var_a1 = arg0->unk60;
        }
        var_v1 = var_a1;
        if (var_a1 < 0x80) {
            temp_t9 = (0x80 - var_a1) & 3;
            if (temp_t9 != 0) {
                var_v0 = &(&sp48[0])[var_a1];
                do {
                    var_v1 += 1;
                    if (*var_v0 == 3) {
                        var_s0 += 1;
                    }
                    var_v0 += 2;
                } while ((temp_t9 + var_a1) != var_v1);
                if (var_v1 != 0x80) {
                    goto block_18;
                }
            } else {
block_18:
                var_v0_2 = &(&sp48[0])[var_v1];
                do {
                    if (var_v0_2->unk0 == 3) {
                        var_s0 += 1;
                    }
                    if (var_v0_2->unk2 == 3) {
                        var_s0 += 1;
                    }
                    if (var_v0_2->unk4 == 3) {
                        var_s0 += 1;
                    }
                    temp_t8 = var_v0_2->unk6;
                    var_v0_2 += 8;
                    if (temp_t8 == 3) {
                        var_s0 += 1;
                    }
                } while (var_v0_2 != &sp148);
            }
        }
        temp_t9_2 = (var_s3 + 1) & 0xFF;
        var_s3 = temp_t9_2;
        if (temp_t9_2 >= (s32) arg0->unk64) {
            goto block_29;
        }
        goto loop_6;
    }
block_29:
    *arg1 = var_s0 << 8;
    return 0;
}
