void func_800EA2DC(Cr *car, void *arg1, void *arg2, s32 arg3) {
    volatile s32 padv[5];
    f32 v[3];
    s32 pl;

    pl = car->player;
    func_800E8CB8(car, arg1, arg2);
    if (car->mode == 0xA) {
        func_800E92C8(car, v, -D_801526F8[pl], D_801526E0[pl]);
    } else {
        func_800E92C8(car, v, D_801526F8[pl], D_801526E0[pl]);
    }
    func_800E8D50(car, arg3, 0, v);
}