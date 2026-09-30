void physics_velocity_integrate_d(ModelObj *arg0, s16 arg1) {
    f32 two = 2.0f;
    Rec808 *rec;
    GameCar *car;
    s32 flag;
    s16 lvl;
    f32 f24;

    car = &player_array[arg0->player];
    rec = &((Rec808 *) &D_8014A250)[arg0->player];
    lvl = (s32) M2C_FIELD(rec, f32 *, 0x3F0);
    flag = lvl >= 21 && (M2C_FIELD(car, s32 *, 0xE8) & 0x80) != 0;
    f24 = M2C_FIELD(car, f32 *, 0xB8);
    physics_velocity_integrate_a(arg0, flag, 194,
                                 M2C_FIELD(car, f32 *, 0xB0) + D_80123894,
                                 M2C_FIELD(car, f32 *, 0xB4) + M2C_FIELD(rec, f32 *, 0x430) * two,
                                 f24,
                                 arg1);
}