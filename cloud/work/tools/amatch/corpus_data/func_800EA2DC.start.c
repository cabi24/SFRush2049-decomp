void func_800EA2DC(void *arg0, void *arg1, void *arg2, s32 arg3) {
    f32 sp70;
    s32 sp6C;
    s8 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s8 *, 0x35C);
    sp6C = (s32) temp_v1;
    func_800E8CB8(arg0, arg1, arg2);
    if (M2C_FIELD(arg0, s8 *, 0x35D) == 0xA) {
        func_800E92C8(arg0, &sp70, -(&D_801526F8)[temp_v1], (&D_801526E0)[temp_v1]);
    } else {
        func_800E92C8(arg0, &sp70, (&D_801526F8)[temp_v1], (&D_801526E0)[temp_v1]);
    }
    func_800E8D50(arg0, arg3, 0, &sp70);
}