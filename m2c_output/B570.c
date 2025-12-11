s32 func_8000A350(void *, u16, s16 (*)[], s16 (*)[], s32 *, s32 *); /* extern */
? func_8000B240(void *, s32 *);                     /* extern */
? func_8000D2B0(s16 (*)[], s16 (*)[], ?);           /* extern */
s32 func_8000F3A4(void *, s16 (*)[], ?, s32);       /* extern */
s32 func_8000F680(s32, s32, s32, s32 *, s32);       /* extern */
s32 func_8000ACA4(void *arg0, s16 (*arg1)[], s32 arg2, s32 *arg3, s32 arg4, s32 *arg5, s32 *arg6); /* static */

s32 func_8000A970(void *arg0, u16 arg1, s32 arg2, s16 (*arg3)[], s16 (*arg4)[], s32 arg5, s32 *arg6) {
    s32 sp294;
    s32 sp290;
    s32 sp28C;
    s16 sp6A;
    u16 sp66;
    u16 sp64;
    s32 sp60;
    s32 sp54;
    s8 sp51;
    s8 sp50;
    s16 *temp_v0_7;
    s32 temp_t2;
    s32 temp_t7;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v0_6;
    s32 var_s0;
    s32 var_s4;
    s32 var_s5;
    s32 var_s6;
    s32 var_s7;
    u16 temp_t6;

    temp_t6 = arg1 & 0xFFFF;
    var_s6 = 0;
    var_s7 = 0;
    var_s5 = 0;
    if ((temp_t6 == 0) || (arg2 == 0)) {
        return 5;
    }
    temp_t7 = (s32) (arg5 + 0xFF) / 256;
    var_s4 = temp_t7;
    arg1 = temp_t6;
    temp_v0_6 = func_8000A350(arg0, temp_t6, arg3, arg4, arg6);
    if ((temp_v0_6 != 0) && (temp_v0_6 != 5)) {
        return temp_v0_6;
    }
    if (*arg6 != -1) {
        return 9;
    }
    func_8000B240(arg0, &sp54);
    if (sp54 < arg5) {
        return 7;
    }
    if (temp_t7 == 0) {
        return 5;
    }
    temp_v0_5 = func_8000A350(arg0, 0U, NULL, NULL, NULL, arg6);
    if ((temp_v0_5 != 0) && (temp_v0_5 != 5)) {
        return temp_v0_5;
    }
    if (*arg6 == -1) {
        return 8;
    }
    var_s0 = 0;
    if ((s32) arg0->unk64 > 0) {
loop_18:
        temp_v0_4 = func_8000F3A4(arg0, (s16 (*)[]) &sp180[0], 0, var_s0 & 0xFF);
        if (temp_v0_4 != 0) {
            return temp_v0_4;
        }
        temp_v0_3 = func_8000ACA4(arg0, (s16 (*)[]) &sp180[0], var_s4, &sp294, var_s0, &sp290, &sp28C);
        if (temp_v0_3 != 0) {
            return temp_v0_3;
        }
        if (sp294 != -1) {
            if (var_s5 == 0) {
                sp51 = (s8) sp294;
                sp50 = (s8) var_s0;
                goto block_27;
            }
            temp_v0_7 = &(&sp80[0])[var_s6];
            temp_v0_7->unk0 = (s8) var_s0;
            temp_v0_7->unk1 = (s8) sp294;
            temp_v0_2 = func_8000F3A4(arg0, (s16 (*)[]) &sp80[0], 1, var_s7 & 0xFF);
            if (temp_v0_2 != 0) {
                return temp_v0_2;
            }
block_27:
            if (sp290 < var_s4) {
                func_8000D2B0((s16 (*)[]) &sp180[0], (s16 (*)[]) &sp80[0], 0x100);
                var_s6 = sp28C;
                var_s7 = var_s0 & 0xFF;
                var_s5 += 1;
                var_s4 -= sp290;
                goto block_31;
            }
            var_s4 = 0;
            temp_v0 = func_8000F3A4(arg0, (s16 (*)[]) &sp180[0], 1, var_s0 & 0xFF);
            if (temp_v0 != 0) {
                return temp_v0;
            }
            goto block_32;
        }
block_31:
        temp_t2 = (var_s0 + 1) & 0xFF;
        var_s0 = temp_t2;
        if (temp_t2 >= (s32) arg0->unk64) {
            goto block_32;
        }
        goto loop_18;
    }
block_32:
    if ((var_s4 > 0) || (sp294 == -1)) {
        return 3;
    }
    sp66 = (u16) sp50;
    sp6A = 0;
    sp64 = arg1;
    sp60 = arg2;
    func_8000D2B0(arg3, (s16 (*)[]) &sp70[0], 0x10);
    func_8000D2B0(arg4, (s16 (*)[]) &sp6C[0], 4);
    return func_8000F680(arg0->unk4, arg0->unk8, (*arg6 + arg0->unk5C) & 0xFFFF, &sp60, 0);
}

s32 func_8000ACA4(void *arg0, s16 (*arg1)[], s32 arg2, s32 *arg3, u8 arg4, s32 *arg5, s32 *arg6) {
    s32 var_t0;
    s32 var_v0;
    s32 var_v1;
    s8 var_v1_2;
    u16 *var_v0_2;
    u16 *var_v0_3;
    void *temp_t1;

    if ((s32) arg4 > 0) {
        var_v0 = 1;
    } else {
        var_v0 = arg0->unk60;
    }
    var_v1 = var_v0;
    if (var_v0 < 0x80) {
        var_v0_2 = arg1 + (var_v0 * 2);
loop_5:
        if (*var_v0_2 != 3) {
            var_v1 += 1;
            var_v0_2 += 2;
            if (var_v1 != 0x80) {
                goto loop_5;
            }
        }
    }
    if (var_v1 == 0x80) {
        *arg3 = -1;
    } else {
        var_t0 = var_v1;
        var_v1_2 = var_v1 + 1;
        *arg5 = 1;
        if ((arg2 >= 2) && (var_v1_2 < 0x80)) {
            var_v0_3 = arg1 + (var_v1_2 * 2);
loop_12:
            temp_t1 = arg1 + (var_t0 * 2);
            if (*var_v0_3 == 3) {
                temp_t1->unk0 = arg4;
                temp_t1->unk1 = var_v1_2;
                var_t0 = (s32) var_v1_2;
                *arg5 += 1;
            }
            var_v1_2 += 1;
            var_v0_3 += 2;
            if ((*arg5 < arg2) && (var_v1_2 < 0x80)) {
                goto loop_12;
            }
        }
        *arg3 = var_v1;
        if ((var_v1_2 == 0x80) && (*arg5 < arg2)) {
            *arg6 = var_t0;
        } else {
            *(arg1 + (var_t0 * 2)) = 1;
            *arg6 = 0;
        }
    }
    return 0;
}
