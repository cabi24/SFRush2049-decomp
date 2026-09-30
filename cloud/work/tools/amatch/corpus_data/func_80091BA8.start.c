s32 func_80091BA8(s32 arg0) {
    s32 temp_a1;

    if (arg0 == -1) {
        return 0;
    }
    temp_a1 = (arg0 & D_80146104) * 0x44;
    if (arg0 != M2C_FIELD((D_80110244 + temp_a1), s32 *, 0xC)) {
        return 0;
    }
    return temp_a1 + D_80110244;
}