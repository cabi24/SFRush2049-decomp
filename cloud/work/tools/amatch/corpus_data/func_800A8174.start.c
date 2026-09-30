void func_800A8174(void *arg0) {
    s32 temp_t9;
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x16A4);
    if (temp_v0 >= 9) {
        *(M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC)) = (s8) M2C_FIELD(arg0, u16 *, 0x16A0);
        temp_t9 = M2C_FIELD(arg0, s32 *, 0xC) + 1;
        M2C_FIELD(arg0, s32 *, 0xC) = temp_t9;
        *(M2C_FIELD(arg0, s32 *, 4) + temp_t9) = (s8) ((s32) M2C_FIELD(arg0, u16 *, 0x16A0) >> 8);
        M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + 1);
    } else if (temp_v0 > 0) {
        *(M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC)) = (s8) M2C_FIELD(arg0, u16 *, 0x16A0);
        M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + 1);
    }
    M2C_FIELD(arg0, u16 *, 0x16A0) = 0U;
    M2C_FIELD(arg0, s32 *, 0x16A4) = 0;
}