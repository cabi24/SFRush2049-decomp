void func_800C26C4(s32 ipa_t1) {
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    GameCar *temp_v0;
    GameCar *temp_v0_2;
    f32 temp_f0;
    f32 temp_f2;
    f32 var_f0;
    s32 temp_v1;
    void *temp_s1;

    temp_s1 = (s32 *) ((ipa_t1 * 0x7C) + (u8 *) &D_801569B8);
    temp_v1 = M2C_FIELD(temp_s1, s32 *, 0);
    if (temp_v1 & 0x10) {
        if ((D_80156BD8 != 0) || (D_80157238 >= 3)) {
            M2C_FIELD(temp_s1, s32 *, 0) = (s32) (temp_v1 & ~0x10);
        } else {
            temp_v0 = &player_array[ipa_t1];
            M2C_FIELD(temp_s1, f32 *, 0x1C) = (f32) (M2C_FIELD(temp_s1, f32 *, 0x1C) + M2C_FIELD(temp_v0, f32 *, 0x368));
            M2C_FIELD(temp_s1, f32 *, 0x20) = (f32) (M2C_FIELD(temp_s1, f32 *, 0x20) + M2C_FIELD(temp_v0, f32 *, 0x36C));
            M2C_FIELD(temp_s1, f32 *, 0x24) = (f32) (M2C_FIELD(temp_s1, f32 *, 0x24) + M2C_FIELD(temp_v0, f32 *, 0x370));
        }
        sp1C = fabsf(M2C_FIELD(temp_s1, f32 *, 0x1C));
        sp20 = fabsf(M2C_FIELD(temp_s1, f32 *, 0x20));
        temp_f0 = fabsf(M2C_FIELD(temp_s1, f32 *, 0x24));
        sp24 = temp_f0;
        if ((D_80123F04 < sp1C) || (D_80123F04 < temp_f0)) {
            M2C_FIELD(temp_s1, s32 *, 0) = (s32) (M2C_FIELD(temp_s1, s32 *, 0) & ~0x10);
            return;
        }
        if (((*(f32 *)0x80123F08 < sp20) && (sp20 < D_80123F0C)) || (((D_80156BD8 != 0) || (D_80157238 > 0) || (*((s32 *) ((u8 *) &D_80152900 + (ipa_t1 * 0x3B8))) & 0x100000)) && (D_80123F10 < sp20))) {
            M2C_FIELD(temp_s1, s32 *, 0) = (s32) (M2C_FIELD(temp_s1, s32 *, 0) & ~0x80);
            func_800C1B60(4, ipa_t1);
            temp_f2 = M2C_FIELD(temp_s1, f32 *, 0x20);
            if (temp_f2 > 0.0f) {
                var_f0 = D_80123F14;
            } else {
                var_f0 = D_80123F18;
            }
            M2C_FIELD(temp_s1, f32 *, 0x24) = 0.0f;
            M2C_FIELD(temp_s1, f32 *, 0x1C) = 0.0f;
            M2C_FIELD(temp_s1, f32 *, 0x20) = (f32) (temp_f2 + var_f0);
        }
    } else if ((*(s32 *)0x80161360 != 0) && (M2C_BITWISE(f32, M2C_FIELD(&D_80161388, s32 *, 0)) < M2C_FIELD(&D_80161388, f32 *, 4)) && (M2C_FIELD(&D_80161388, f32 *, 8) < M2C_FIELD(&D_80161388, f32 *, 4))) {
        M2C_FIELD(temp_s1, s32 *, 0) = (s32) (temp_v1 | 0x10);
        temp_v0_2 = &player_array[ipa_t1];
        M2C_FIELD(temp_s1, f32 *, 0x1C) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x368);
        M2C_FIELD(temp_s1, f32 *, 0x20) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x36C);
        M2C_FIELD(temp_s1, f32 *, 0x24) = (f32) M2C_FIELD(temp_v0_2, f32 *, 0x370);
    }
}