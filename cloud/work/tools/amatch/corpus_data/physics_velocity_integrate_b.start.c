void physics_velocity_integrate_b(ModelObj *arg0, s16 arg1) {
    Rec808 *rec;
    GameCar *car;
    s32 flag;
    s32 lvl;
    s8 t;

    car = &player_array[arg0->player];
    rec = &((Rec808 *) &D_8014A250)[arg0->player];
    lvl = (s32) M2C_FIELD(rec, f32 *, 0x3F0);
    flag = (s16) lvl >= 13;
    t = M2C_FIELD(rec, s8 *, 0x641);
    if (flag && t == 0) {
        flag = (M2C_FIELD(car, s32 *, 0xE8) & 0x800) != 0;
    }
    physics_velocity_integrate_a(flag, arg0, 194,
                                 0.0f,
                                 (t != 0 ? 2.0f : M2C_FIELD(rec, f32 *, 0x544)) + M2C_FIELD(car, f32 *, 0xD8),
                                 M2C_FIELD(car, f32 *, 0xDC) - 2.0f,
                                 arg1);
}