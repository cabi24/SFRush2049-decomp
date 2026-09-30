void physics_velocity_integrate_b(ModelObj *arg0, s16 arg1) {
    GameCar *car;
    s16 lvl;
    s32 t;

    car = &player_array[arg0->player];
    lvl = (s32) M2C_FIELD((&((Rec808 *) &D_8014A250)[arg0->player]), f32 *, 0x3F0);
    t = M2C_FIELD((&((Rec808 *) &D_8014A250)[arg0->player]), s8 *, 0x641);
    physics_velocity_integrate_a(arg0, lvl >= 13 && (t != 0 || (M2C_FIELD(car, s32 *, 0xE8) & 0x800) != 0), 194,
                                 0.0f,
                                 M2C_FIELD(car, f32 *, 0xD8) + (t != 0 ? 2.0f : M2C_FIELD((&((Rec808 *) &D_8014A250)[arg0->player]), f32 *, 0x544)),
                                 M2C_FIELD(car, f32 *, 0xDC) - 2.0f,
                                 arg1);
}