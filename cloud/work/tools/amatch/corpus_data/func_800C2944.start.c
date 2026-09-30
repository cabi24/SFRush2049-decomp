void func_800C2944(s32 ipa_t1) {
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    GameCar *temp_v0;
    GameCar *temp_v0_2;
    f32 temp_f0;
    s32 temp_v1;
    void *temp_s1;

    temp_s1 = (s32 *) ((ipa_t1 * 0x7C) + (u8 *) &D_801569B8);
    temp_v1 = M2C_FIELD(temp_s1, s32 *, 0);
    if (temp_v1 & 0x20) {
        if ((D_80156CE4 != 0) || (D_80157238 >= 3)) {
            M2C_FIELD(temp_s1, s32 *, 0) = (s32) (temp_v1 & ~0x20);
        } else {
            temp_v0 = &player_array[ipa_t1];
            M2C_FIELD(temp_s1, f32 *, 4) = (f32) (M2C_FIELD(temp_s1, f32 *, 4) + M2C_FIELD(temp_v0, f32 *, 0x368));
            M2C_FIELD(temp_s1, f32 *, 8) = (f32) (M2C_FIELD(temp_s1, f32 *, 8) + M2C_FIELD(temp_v0, f32 *, 0x36C));
            M2C_FIELD(temp_s1, f32 *, 0xC) = (f32) (M2C_FIELD(temp_s1, f32 *, 0xC) + M2C_FIELD(temp_v0, f32 *, 0x370));
        }
        sp1C = fabsf(M2C_FIELD(temp_s1, f32 *, 4));
        sp20 = fabsf(M2C_FIELD(temp_s1, f32 *, 8));
        temp_f0 = fabsf(M2C_FIELD(temp_s1, f32 *, 0xC));
        sp24 = temp_f0;
        if ((D_80123F1C < sp1C) || (D_80123F20 < sp20)) {
            M2C_FIELD(temp_s1, s32 *, 0) = (s32) (M2C_FIELD(temp_s1, s32 *, 0) & ~0x20);
            return;
        }
        if (((*(f32 *)0x80123F24 < temp_f0) && (temp_f0 < D_80123F28)) || (((D_80156CE4 != 0) || (D_80157238 > 0) || (*((s32 *) ((u8 *) &D_80152900 + (ipa_t1 * 0x3B8))) & 0x100000)) && (D_80123F2C < temp_f0))) {
            M2C_FIELD(temp_s1, s32 *, 0) = (s32) (M2C_FIELD(temp_s1, s32 *, 0) & ~0x80);
            if (M2C_FIELD(temp_s1, f32 *, 0xC) > 0.0f) {
                func_800C1B60(3, ipa_t1);
                M2C_FIELD(temp_s1, f32 *, 0xC) = (f32) (M2C_FIELD(temp_s1, f32 *, 0xC) + D_80123F30);
            } else {
                func_800C1B60(2, ipa_t1);
                M2C_FIELD(temp_s1, f32 *, 0xC) = (f32) (M2C_FIELD(temp_s1, f32 *, 0xC) + D_80123F34);
            }
            M2C_FIELD(temp_s1, f32 *, 8) = 0.0f;
            M2C_FIELD(temp_s1, f32 *, 4) = 0.0f;
        }
    } else if ((*(s32 *)0x80161360 != 0) && (M2C_FIELD(&D_80161388, f32 *, 4) < M2C_FIELD(&D_80161388, f32 *, 8)) && (M2C_BITWISE(f32, M2C_FIELD(&D_80161388, s32 *, 0)) < M2C_FIELD(&D_80161388, f32 *, 8))) {
        M2C_FIELD(temp_s1, s32 *, 0) = (s32) (temp_v1 | 0x20);
        temp_v0_2 = &player_array[ipa_t1];
        M2C_FIELD(temp_s1, f32 *, 4) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x368);
        M2C_FIELD(temp_s1, f32 *, 8) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x36C);
        M2C_FIELD(temp_s1, f32 *, 0xC) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x370);
    }
}