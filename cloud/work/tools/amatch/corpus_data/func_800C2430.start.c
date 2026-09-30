void func_800C2430(s32 ipa_t1) {
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
    if (temp_v1 & 0x40) {
        if ((D_80156BC8 != 0) || (D_80157238 >= 3)) {
            M2C_FIELD(temp_s1, s32 *, 0) = (s32) (temp_v1 & ~0x40);
        } else {
            temp_v0 = &player_array[ipa_t1];
            M2C_FIELD(temp_s1, f32 *, 0x10) = (f32) (M2C_FIELD(temp_s1, f32 *, 0x10) + M2C_FIELD(temp_v0, f32 *, 0x368));
            M2C_FIELD(temp_s1, f32 *, 0x14) = (f32) (M2C_FIELD(temp_s1, f32 *, 0x14) + M2C_FIELD(temp_v0, f32 *, 0x36C));
            M2C_FIELD(temp_s1, f32 *, 0x18) = (f32) (M2C_FIELD(temp_s1, f32 *, 0x18) + M2C_FIELD(temp_v0, f32 *, 0x370));
        }
        sp1C = fabsf(M2C_FIELD(temp_s1, f32 *, 0x10));
        sp20 = fabsf(M2C_FIELD(temp_s1, f32 *, 0x14));
        temp_f0 = fabsf(M2C_FIELD(temp_s1, f32 *, 0x18));
        sp24 = temp_f0;
        if ((D_80123EEC < sp20) || (D_80123EEC < temp_f0)) {
            M2C_FIELD(temp_s1, s32 *, 0) = (s32) (M2C_FIELD(temp_s1, s32 *, 0) & ~0x40);
            return;
        }
        if (((*(f32 *)0x80123EF0 < sp1C) && (sp1C < D_80123EF4)) || (((D_80156BC8 != 0) || (D_80157238 > 0) || (*((s32 *) ((u8 *) &D_80152900 + (ipa_t1 * 0x3B8))) & 0x100000)) && (D_80123EF8 < sp1C))) {
            M2C_FIELD(temp_s1, s32 *, 0) = (s32) (M2C_FIELD(temp_s1, s32 *, 0) & ~0x80);
            if (M2C_FIELD(temp_s1, f32 *, 0x10) > 0.0f) {
                func_800C1B60(0, ipa_t1);
                M2C_FIELD(temp_s1, f32 *, 0x10) = (f32) (M2C_FIELD(temp_s1, f32 *, 0x10) + D_80123EFC);
            } else {
                func_800C1B60(1, ipa_t1);
                M2C_FIELD(temp_s1, f32 *, 0x10) = (f32) (M2C_FIELD(temp_s1, f32 *, 0x10) + D_80123F00);
            }
            M2C_FIELD(temp_s1, f32 *, 0x18) = 0.0f;
            M2C_FIELD(temp_s1, f32 *, 0x14) = 0.0f;
        }
    } else if ((*(s32 *)0x80161360 != 0) && (M2C_FIELD(&D_80161388, f32 *, 4) < M2C_BITWISE(f32, M2C_FIELD(&D_80161388, s32 *, 0))) && (M2C_FIELD(&D_80161388, f32 *, 8) < M2C_BITWISE(f32, M2C_FIELD(&D_80161388, s32 *, 0)))) {
        M2C_FIELD(temp_s1, s32 *, 0) = (s32) (temp_v1 | 0x40);
        temp_v0_2 = &player_array[ipa_t1];
        M2C_FIELD(temp_s1, f32 *, 0x10) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x368);
        M2C_FIELD(temp_s1, f32 *, 0x14) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x36C);
        M2C_FIELD(temp_s1, f32 *, 0x18) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x370);
    }
}