void car_collision_init(void *arg0, M2C_UNK arg1, s8 arg2, s32 arg3) {
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_t8_2;
    s32 temp_t8_3;
    s32 temp_t8_4;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_v0;
    s8 temp_v1_2;
    u16 temp_t8;

    temp_v1 = M2C_FIELD(arg0, s32 *, 0x16A4);
    if (temp_v1 >= 0xE) {
        temp_t8 = M2C_FIELD(arg0, u16 *, 0x16A0) | (arg3 << temp_v1);
        M2C_FIELD(arg0, u16 *, 0x16A0) = temp_t8;
        *(M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC)) = (s8) temp_t8;
        temp_t8_2 = M2C_FIELD(arg0, s32 *, 0xC) + 1;
        M2C_FIELD(arg0, s32 *, 0xC) = temp_t8_2;
        *(M2C_FIELD(arg0, s32 *, 4) + temp_t8_2) = (s8) ((s32) M2C_FIELD(arg0, u16 *, 0x16A0) >> 8);
        temp_v0 = M2C_FIELD(arg0, s32 *, 0x16A4);
        M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + 1);
        M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_v0 - 0xD);
        M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) ((s32) (arg3 & 0xFFFF) >> (0x10 - temp_v0));
    } else {
        M2C_FIELD(arg0, u16 *, 0x16A0) = (u16) (M2C_FIELD(arg0, u16 *, 0x16A0) | (arg3 << temp_v1));
        M2C_FIELD(arg0, s32 *, 0x16A4) = (s32) (temp_v1 + 3);
    }
    temp_t8_3 = (M2C_FIELD(arg0, s32 *, 0x1694) + 0xA) & ~7;
    M2C_FIELD(arg0, s32 *, 0x1694) = temp_t8_3;
    M2C_FIELD(arg0, s32 *, 0x1694) = (s32) (temp_t8_3 + ((arg2 + 4) * 8));
    func_800A8174(arg0);
    M2C_FIELD(arg0, s32 *, 0x169C) = 8;
    *(M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC)) = arg2;
    temp_t8_4 = M2C_FIELD(arg0, s32 *, 0xC) + 1;
    M2C_FIELD(arg0, s32 *, 0xC) = temp_t8_4;
    *(M2C_FIELD(arg0, s32 *, 4) + temp_t8_4) = (s8) (arg2 >> 8);
    temp_v1_2 = ~arg2;
    temp_t7 = M2C_FIELD(arg0, s32 *, 0xC) + 1;
    M2C_FIELD(arg0, s32 *, 0xC) = temp_t7;
    var_v0 = arg2 - 1;
    *(M2C_FIELD(arg0, s32 *, 4) + temp_t7) = temp_v1_2;
    temp_t7_2 = M2C_FIELD(arg0, s32 *, 0xC) + 1;
    M2C_FIELD(arg0, s32 *, 0xC) = temp_t7_2;
    *(M2C_FIELD(arg0, s32 *, 4) + temp_t7_2) = (s8) (temp_v1_2 >> 8);
    M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) + 1);
    if (arg2 != 0) {
        do {
            *(M2C_FIELD(M2C_ERROR(/* Read from unset register $t0 */), s32 *, 4) + M2C_FIELD(M2C_ERROR(/* Read from unset register $t0 */), s32 *, 0xC)) = *M2C_ERROR(/* Read from unset register $a1 */);
            M2C_FIELD(M2C_ERROR(/* Read from unset register $t0 */), s32 *, 0xC) = (s32) (M2C_FIELD(M2C_ERROR(/* Read from unset register $t0 */), s32 *, 0xC) + 1);
            var_v0 -= 1;
        } while (var_v0 != 0);
    }
}