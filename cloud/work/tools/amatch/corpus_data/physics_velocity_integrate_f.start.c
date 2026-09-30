void physics_velocity_integrate_f(ModelObj *arg0, s16 arg1) {
    f32 two = 2.0f;
    Rec808 *rec;
    GameCar *car;
    s32 flag;
    s32 lvl;

    car = &player_array[arg0->player];
    rec = &((Rec808 *) &D_8014A250)[arg0->player];
    lvl = (s32) M2C_FIELD(rec, f32 *, 0x3F0);
    flag = (s16) lvl >= 21 && (M2C_FIELD(car, s32 *, 0xE8) & 0x100) != 0;
    physics_velocity_integrate_a(flag, arg0, 194,
                                 M2C_FIELD(car, f32 *, 0xBC) - D_8012389C,
                                 M2C_FIELD(rec, f32 *, 0x48C) * two + M2C_FIELD(car, f32 *, 0xC0),
                                 M2C_FIELD(car, f32 *, 0xC4),
                                 arg1);
}