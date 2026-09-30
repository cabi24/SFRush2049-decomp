s32 minimap_render(s32 arg0, u16 arg1, s32 *arg2, s32 *arg3, s32 *arg4, s32 arg5, s32 arg6) {
    s32 sp3C;
    s32 temp_v0;
    s32 var_s1;
    s32 var_s2;
    u16 *var_s3;
    u16 var_s7;
    void *temp_v1;

    var_s1 = arg0;
    var_s2 = arg6;
    var_s7 = arg1;
    var_s3 = &D_801407F0;
loop_1:
    if (func_800B98D8(var_s1, var_s2) == 0) {
        return 0;
    }
    if (var_s2 == 0) {
        *arg4 = 0;
    }
    if (var_s1 >= 0) {
        if (arg5 == 0) {
            var_s3 = (u16 *) &D_801407FC;
            goto block_12;
        }
        if (*(*var_s3 + (var_s1 * 0x10)) != 0) {
            goto block_10;
        }
block_12:
        temp_v0 = var_s1 * 0x10;
        var_s2 += 1;
        *arg4 = (*arg4 + M2C_FIELD((M2C_ERROR(/* Read from unset register $t1 */) + temp_v0), u16 *, 0xA)) - var_s7;
        temp_v1 = *var_s3 + temp_v0;
        var_s1 = (s32) M2C_FIELD(temp_v1, s8 *, 4);
        sp3C = var_s1;
        var_s7 = M2C_FIELD(temp_v1, u16 *, 6);
        goto loop_1;
    }
block_10:
    *arg2 = var_s1;
    *arg3 = (s32) var_s7;
    return 1;
}