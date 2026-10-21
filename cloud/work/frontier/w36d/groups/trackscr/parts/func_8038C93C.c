void func_8038C93C(void)
{
    Color color = D_803B8394;
    s32 i;
    s16 texture_index;
    f32 yaw;
    for (i = 0; i < 48; i++) {
        if (D_803B6B14[i].handle == -1) {
            memcpy(D_803B6B14[i].matrix, D_8011418C, 36);
            D_803B6B14[i].handle = sign_extend_call(
                string_copy_format(D_803B6AF0[D_803B6B14[i].other], 0, D_80140BDC - 1, 1),
                (u32)D_803B6B14[i].matrix, -1,
                (D_803B6B14[i].other == 0 || D_803B6B14[i].other == 1 || D_803B6B14[i].other == 2) ? 0x42000 : 0);
            func_8008E06C((s16)D_803B6B14[i].handle, (s32 *)&color);
        }
    }
    func_8008D870((s16)D_803B6B14[42].handle,
        (s32)func_800B24EC(D_803B9264, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B6B14[42].matrix, D_8011418C, 36);
    yaw = -1.5707963705062866f;
    func_800B5898(yaw, D_803B6B14[42].matrix);
    D_803B6B14[42].position[0] = 0.0f;
    D_803B6B14[42].position[1] = 115.0f;
    D_803B6B14[42].position[2] = 200.0f;
    func_8008D870((s16)D_803B6B14[43].handle,
        (s32)func_800B24EC(D_803B9274, &texture_index, 0, D_80140BDC - 1, 1), -1);
    func_8008D870((s16)D_803B6B14[44].handle,
        (s32)func_800B24EC(D_803B927C, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B6B14[44].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[44].matrix);
    D_803B6B14[44].position[0] = 75.1f;
    D_803B6B14[44].position[1] = 55.2f;
    D_803B6B14[44].position[2] = 100.0f;
    func_8008B32C(D_803B6B14[44].matrix, D_803B6B14[44].matrix, 0.5f);
    memcpy(D_803B6B14[45].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[45].matrix);
    func_800B5940(3.1415927410125732f, D_803B6B14[45].matrix);
    D_803B6B14[45].position[0] = 87.1f;
    D_803B6B14[45].position[1] = 55.2f;
    D_803B6B14[45].position[2] = 100.0f;
    func_8008D870((s16)D_803B6B14[46].handle,
        (s32)func_800B24EC(D_803B9280, &texture_index, 0, D_80140BDC - 1, 1), -1);
    memcpy(D_803B6B14[46].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[46].matrix);
    D_803B6B14[46].position[0] = -75.1f;
    D_803B6B14[46].position[1] = 55.20f;
    D_803B6B14[46].position[2] = 100.0f;
    func_8008B32C(D_803B6B14[46].matrix, D_803B6B14[46].matrix, 0.5f);
    memcpy(D_803B6B14[47].matrix, D_8011418C, 36);
    func_800B5898(yaw, D_803B6B14[47].matrix);
    D_803B6B14[47].position[0] = -87.1f;
    D_803B6B14[47].position[1] = 55.20f;
    D_803B6B14[47].position[2] = 100.0f;
    D_803B7714 = 1;
    D_803B7718 = 1;
    particle_velocity_set();
}
