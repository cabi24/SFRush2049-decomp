? func_80007270(s32, ?, ?);                         /* extern */
? func_8000D2B0(s8 *, s32, ?);                      /* extern */
? func_8000E4B0(?, s8 *);                           /* extern */
? func_8000E5B0();                                  /* extern */
? func_8000E5F4();                                  /* extern */
s32 func_8000E620(s32, s32);                        /* extern */
s32 func_8000F9F0(s32, u16);                        /* extern */
s32 func_8000FAC0(s8 *);                            /* extern */
extern s32 D_8002C4C0;
extern u8 D_80037AE0;
extern s8 D_80037F60;

s32 func_8000E8D0(s32 arg0, s32 arg1, u16 arg2, s32 arg3) {
    s32 sp44;
    s32 sp40;
    s32 temp_a0;
    s32 temp_v0;
    s32 var_s3;
    s32 var_s5;
    s32 var_v0;
    s8 *temp_s2;
    s8 *var_s0;

    var_s5 = 2;
    func_8000E5B0();
    sp40 = arg2 << 5;
    sp44 = (s32) arg2 >> 3;
loop_1:
    var_s0 = &D_80037F60;
    if ((D_80037AE0 != 2) || (arg1 != D_8002C4C0)) {
        D_80037AE0 = 2;
        D_8002C4C0 = arg1;
        var_v0 = 0;
        if (arg1 > 0) {
            temp_a0 = arg1 & 3;
            if (temp_a0 != 0) {
                do {
                    var_v0 += 1;
                    *var_s0 = 0;
                    var_s0 += 1;
                } while (temp_a0 != var_v0);
                if (var_v0 != arg1) {
                    goto loop_9;
                }
            } else {
                do {
loop_9:
                    var_v0 += 4;
                    var_s0->unk0 = 0;
                    var_s0->unk1 = 0;
                    var_s0->unk2 = 0;
                    var_s0->unk3 = 0;
                    var_s0 += 4;
                } while (var_v0 != arg1);
            }
        }
        D_80037F60.unk3C = 1;
        var_s0->unk0 = 0xFF;
        var_s0->unk1 = 3;
        var_s0->unk2 = 0x21;
        var_s0->unk3 = 2;
        var_s0->unk26 = 0xFF;
        var_s0->unk27 = 0xFE;
    } else {
        var_s0 = &(&D_80037F60)[arg1];
    }
    var_s0->unk4 = (s8) sp44;
    var_s0->unk5 = (s8) (func_8000F9F0(arg2 & 0xFFFF, arg2) | sp40);
    func_8000E4B0(1, &D_80037F60);
    func_80007270(arg0, 0, 1);
    func_8000E4B0(0, &D_80037F60);
    func_80007270(arg0, 0, 1);
    temp_s2 = var_s0 + 6;
    var_s3 = (s32) (var_s0->unk2 & 0xC0) >> 4;
    if (var_s3 == 0) {
        if (var_s0->unk26 != func_8000FAC0(temp_s2)) {
            temp_v0 = func_8000E620(arg0, arg1);
            var_s3 = temp_v0;
            if (temp_v0 == 0) {
                var_s3 = 4;
                goto block_18;
            }
        } else {
            func_8000D2B0(temp_s2, arg3, 0x20);
            goto block_18;
        }
    } else {
        var_s3 = 1;
block_18:
        if (var_s3 == 4) {
            var_s5 -= 1;
            if (var_s5 >= 0) {
                goto loop_1;
            }
        }
    }
    func_8000E5F4();
    return var_s3;
}
