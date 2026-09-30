s32 MP_TargetSteerPos(void *ipa_s0, f32 ipa_f20, f32 ipa_f22) {
    if ((M2C_FIELD(ipa_s0, s32 *, 0x10) = func_80020174(M2C_FIELD(ipa_s0, u16 *, 0xE), 0xFF, 0xFF)) == -1) {
        return 0;
    }
    func_8001fea4(M2C_FIELD(ipa_s0, s32 *, 0x10), (u8) (u32) (ipa_f20 * 127.0f));
    func_8001fff4(M2C_FIELD(ipa_s0, s32 *, 0x10), (u8) (u32) ((ipa_f22 + 1.0f) * 0.5f * 127.0f));
    M2C_FIELD(ipa_s0, s8 *, 1) = 1;
    return 1;
}