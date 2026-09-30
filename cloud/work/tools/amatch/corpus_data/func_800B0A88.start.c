void func_800B0A88(s16 ipa_s1, s32 ipa_s2) {
    u8 temp_v0;
    void *temp_s0;
    void *temp_t0;

    unksp48 = (s32) ipa_s1;
    unksp4C = ipa_s2;
    if ((gameplay_mode == 2) || (gameplay_mode == 6) || (temp_t0 = (D_8014A250_Record *) ((ipa_s1 * 0x808) + (u8 *) &D_8014A250), (M2C_FIELD(temp_t0, s16 *, 0x7CA) != 0))) {
        *((s32 *) ((u8 *) &D_80152ABC + ((ipa_s1 * 0x3B8) + ((s16) ipa_s2 * 0x18)))) = 0;
        return;
    }
    temp_s0 = (GameCar *) ((ipa_s1 * 0x3B8) + ((s16) ipa_s2 * 0x18) + 0x290 + (u8 *) player_array);
    M2C_FIELD(temp_s0, s32 *, 0x14) = 0x800924F4;
    M2C_FIELD(temp_s0, s16 *, 8) = ipa_s1;
    M2C_FIELD(temp_s0, s32 *, 0xC) = (s32) (s16) ipa_s2;
    temp_v0 = M2C_FIELD(temp_t0, u8 *, 8);
    unksp38 = 0.0f;
    unksp40 = 0.0f;
    unksp3C = *((f32 *) ((u8 *) &D_8011B4B8 + (temp_v0 * 0xC)));
    save_slot_valid((s32) (&D_801427C0)[((&D_80111299)[(ipa_s1 * 0xD) + temp_v0] * 4) + (s16) ipa_s2 + 0xE0], 0xF, (s16) *((s32 *) ((u8 *) &D_80139320 + (ipa_s1 << 6))), 0, (s16) (s32) ipa_s1, -1, 1);
    M2C_FIELD(temp_s0, s16 *, 6) = (s16) temp_v0;
    func_8008D6FC(M2C_FIELD(temp_s0, s16 *, 6), &unksp38, NULL);
    model_data_load(M2C_FIELD(temp_s0, s16 *, 6), 1, 0xF);
    func_80092484(ipa_s1, (s16) ipa_s2);
}