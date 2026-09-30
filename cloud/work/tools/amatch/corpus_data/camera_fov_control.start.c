void camera_fov_control(void *ipa_s0) {
    s32 temp_v1;
    s32 var_v0;
    s8 temp_v0;
    void *temp_v0_2;

    temp_v0 = M2C_FIELD(ipa_s0, s8 *, 0x64);
    if ((temp_v0 != 2) && ((temp_v0 != 0) || (leaderboard_update(M2C_FIELD(ipa_s0, s32 *, 0x60)) == 0))) {
        results_screen_update(M2C_FIELD(ipa_s0, s32 *, 0x60));
        temp_v0_2 = (s32 *) ((M2C_FIELD(ipa_s0, s16 *, 0x10) * 0x30) + (u8 *) &D_80117530);
        temp_v1 = M2C_FIELD(temp_v0_2, s32 *, 0x24);
        if (temp_v1 != -1) {
            if (D_8010FFC0 == 0) {
                var_v0 = -1;
            } else if (temp_v1 == -1) {
                var_v0 = -1;
            } else {
                var_v0 = camera_target_track((u8 *) ipa_s0 + 0x38, (s32) &D_801141B0, M2C_FIELD(temp_v0_2, f32 *, 0x2C), 0, 1.0f, 0.0f, temp_v1, 0, M2C_FIELD(temp_v0_2, s32 *, 0x28), 0x80U);
            }
            M2C_FIELD(ipa_s0, s32 *, 0x60) = var_v0;
        } else {
            M2C_FIELD(ipa_s0, s32 *, 0x60) = -1;
        }
        M2C_FIELD(ipa_s0, s8 *, 0x64) = 2;
    }
}