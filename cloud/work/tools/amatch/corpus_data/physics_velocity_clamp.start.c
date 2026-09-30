s32 physics_velocity_clamp(s32 arg0, s32 arg1, s32 *arg2, s32 arg3) {
    s32 sp34;
    s16 temp_v0;
    s16 var_a1;
    s32 *var_v0;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_s0;
    s32 var_s2;
    s32 var_s3;
    void *temp_v0_2;
    void *var_v1;

    var_s0 = arg1;
    var_s2 = arg0;
    var_s3 = arg3;
loop_1:
    if (func_800B98D8(var_s2, var_s3) == 0) {
        return 0;
    }
    if (var_s2 < 0) {
        var_v0 = &D_80151D38;
        var_a0 = 1;
        if (M2C_FIELD(&D_80151CE8, s16 *, 8) >= 2) {
loop_5:
            if (var_s0 >= M2C_FIELD(var_v0, s16 *, 0x2E)) {
                var_a0 += 1;
                var_v0 = var_v0 + 0x14;
                if (var_a0 < M2C_FIELD(&D_80151CE8, s16 *, 8)) {
                    goto loop_5;
                }
            }
        }
        *arg2 = var_a0 - 1;
        return 1;
    }
    var_a1 = -1;
    var_a0_2 = 0;
    if (M2C_FIELD(&D_80151CE8, s16 *, 8) > 0) {
        var_v1 = (s32 *) ((var_s2 * 2) + (u8 *) &D_80151CE8);
        do {
            temp_v0 = M2C_FIELD(var_v1, s16 *, 0x38);
            if ((temp_v0 >= 0) && ((var_a1 < 0) || (var_a1 < temp_v0)) && (var_s0 >= temp_v0)) {
                *arg2 = var_a0_2;
                var_a1 = M2C_FIELD(var_v1, s16 *, 0x38);
            }
            var_a0_2 += 1;
            var_v1 = (u8 *) var_v1 + 0x50;
        } while (var_a0_2 < M2C_FIELD(&D_80151CE8, s16 *, 8));
    }
    if (var_a1 < 0) {
        var_s3 += 1;
        temp_v0_2 = M2C_ERROR(/* Read from unset register $t0 */) + (var_s2 * 0x10);
        var_s2 = (s32) M2C_FIELD(temp_v0_2, s8 *, 1);
        sp34 = var_s2;
        var_s0 = (s32) M2C_FIELD(temp_v0_2, u16 *, 2);
        goto loop_1;
    }
    return 1;
}