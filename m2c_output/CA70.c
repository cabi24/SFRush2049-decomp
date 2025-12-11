s32 func_8000D5C0(s32, s32);                        /* extern */
s32 func_8000FB60();                                /* extern */
extern s32 AI_BITRATE_REG;
extern s32 AI_DACRATE_REG;
extern s32 AI_DRAM_ADDR_REG;
extern s32 AI_LEN_REG;
extern s32 D_8002C368;
extern u8 D_8002C3C0;

s32 func_8000BE70(s32 arg0, s32 arg1) {
    s32 var_a1;

    if (func_8000FB60() != 0) {
        return -1;
    }
    var_a1 = arg0;
    if (D_8002C3C0 != 0) {
        var_a1 = arg0 - 0x2000;
    }
    if (!((arg0 + arg1) & 0x1FFF)) {
        D_8002C3C0 = 1;
    } else {
        D_8002C3C0 = 0;
    }
    AI_DRAM_ADDR_REG = func_8000D5C0(var_a1, var_a1);
    AI_LEN_REG = arg1;
    return 0;
}

s32 func_8000BF00(s32 arg0) {
    f32 var_ft3;
    s32 var_a0;
    u32 temp_lo;
    u32 temp_v1;

    var_ft3 = (f32) arg0;
    if (arg0 < 0) {
        var_ft3 += 4294967296.0f;
    }
    temp_v1 = (u32) (((f32) D_8002C368 / var_ft3) + 0.5f);
    if (temp_v1 < 0x84U) {
        return -1;
    }
    temp_lo = temp_v1 / 66U;
    var_a0 = temp_lo & 0xFF;
    if ((temp_lo & 0xFF) >= 0x11) {
        var_a0 = 0x10;
    }
    AI_DACRATE_REG = temp_v1 - 1;
    AI_BITRATE_REG = var_a0 - 1;
    return (s32) D_8002C368 / (s32) temp_v1;
}
