void func_800CCE5C(void **arg0, u8 *arg1) {
    void *sp24;

    if (M2C_FIELD(*arg0, void ***, 0x2C) != NULL) {
        sp24 = *M2C_FIELD(*arg0, void ***, 0x2C);
        if (func_800A1910(((u8 *) sp24 + 0x3C), arg1, 0xB) != 0) {
            memcpy((u8 *) *arg0 + 0x21, arg1, 0xB);
            memcpy(((u8 *) sp24 + 0x3C), arg1, 0xB);
            M2C_FIELD(sp24, s32 *, 0x38) = format_string_parse(((u8 *) sp24 + 0x3C), 0xB);
            slot_state_lookup(M2C_FIELD(*arg0, void ***, 8), (u32) ((u8 *) sp24 + 0x38), 0xF);
        }
    }
}
