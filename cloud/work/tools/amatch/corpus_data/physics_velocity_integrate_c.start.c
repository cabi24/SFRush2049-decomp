void physics_velocity_integrate_c(ModelObj *arg0, s16 arg1) {
    f32 two = 2.0f;
    Rec808 *rec;
    GameCar *car;
    s32 flag;
    s32 lvl;

    car = &player_array[arg0->player];
    rec = &((Rec808 *) &D_8014A250)[arg0->player];
    lvl = (s32) M2C_FIELD(rec, f32 *, 0x3F0);
    flag = (s16) lvl >= 21 && (M2C_FIELD(car, s32 *, 0xE8) & 0x4000) != 0;
    physics_velocity_integrate_a(flag, arg0, 194,
                                 M2C_FIELD(car, f32 *, 0xC8) + D_80123890,
                                 M2C_FIELD(car, f32 *, 0xCC) + M2C_FIELD(rec, f32 *, 0x4E8) * two,
                                 M2C_FIELD(car, f32 *, 0xD0),
                                 arg1);
}