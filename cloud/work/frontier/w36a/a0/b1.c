void func_803A0FD8(void)
{
    Player *pl;
    s32 p;
    s32 i;

    func_803A0498();
    audio_frame_sync(62, 0, 0, 0, 0);
    if (D_80156994) {
        audio_frame_sync(80, 0, 0, 0, 0);
        audio_frame_sync(61, 0, 0, 0, 0);
    } else if (D_8014978C >= 6) {
        audio_frame_sync(80, 0, 0, 0, 0);
    } else {
        audio_frame_sync(81, 0, 0, 0, 0);
    }
    D_803BA02C = 0;
    for (p = 0; p < D_8014A108; p++) {
        D_803B28B8[p] = 0;
        D_803BA048[p] = 1;
        D_803BA038[p] = 1;
        D_803BA028[p] = 0;
        D_803BA1D0[p] = -1;
        func_8039D300(p);
    }
    for (p = 0; p < D_8014A108; p++) {
        for (D_803BA020[p] = 0, i = 0; i < 13; i++) {
            if (i < 13) D_803B9FD8[p][D_803BA020[p]++] = D_803B3010[i];
        }
    }
    for (i = 0; i < 48; i++) D_803B9AA0[i] = func_800A7D6C();
    init_state_begin();
    D_80146180[0] = 0;
    D_80146180[1] = 0;
    D_80146180[2] = 0;
    D_80146180[3] = 0;
    D_80154188 = 90.0f;
    func_800A5A40();
    D_80151AD0 = D_8014A108;
    if (D_80151AD0 == 3) D_80151AD0 = 4;
    if (D_8014A110 == 2) {
        D_803BA00C = 3;
        D_80151AD0 = 4;
        if (!D_80156994) {
            D_803BA00C = 1;
            D_80151AD0 = 2;
        }
    }
    car_render_full(D_80151AD0);
    if (D_80151AD0 == 1) {
        D_803B25A8 = 1.0f;
        D_803B25AC = 0.045f;
        D_803B25B0 = 40.0f;
        D_803B25B4 = 0.0f;
        D_803B25B8 = -5.4f;
        D_803B25BC = -8.4f;
        D_803B25C0 = 15.0f;
        D_803B25C4 = 11.25f;
    } else {
        D_803B25A8 = 1.0f;
        D_803B25AC = 0.055f;
        D_803B25B0 = 40.0f;
        D_803B25B4 = 0.0f;
        D_803B25B8 = -3.45f;
        D_803B25BC = -7.7f;
        D_803B25C0 = 15.0f;
        D_803B25C4 = 11.25f;
    }
    audio_interrupt_handler(1);
    sfx_position_3d(1);
    particle_lifetime_set();
    for (p = 0; p < 4; p++) D_803B9FC0[p] = ambient_sound_set(-20, 0, -10, 10, 192, 0, 0, 0);
    for (p = 0; p < D_8014A108; p++) {
        pl = &D_8014A118[p];
        math_utility(D_8011418C, D_803B9E40[p][0]);
        math_utility(D_8011418C, D_803B9E40[p][1]);
        D_803B25CC[p][0] = sign_extend_call(string_copy_format(D_803B85B0, 0, D_80140BDC - 1, 1), D_803B9E40[p][0], -1, 0x40000);
        D_803B25CC[p][1] = sign_extend_call(string_copy_format(D_803B85BC, 0, D_80140BDC - 1, 1), D_803B9E40[p][1], -1, 0);
        model_data_load(D_803B25CC[p][0], 1, 15);
        model_data_load(D_803B25CC[p][1], 1, 15);
        if (D_8014A110 == 2) {
            model_transform_setup(D_803B25CC[p][0], 0, 15);
            model_transform_setup(D_803B25CC[p][1], 0, 15);
        } else {
            model_transform_setup(D_803B25CC[p][0], 0, 1 << p);
            model_transform_setup(D_803B25CC[p][1], 0, 1 << p);
        }
        D_803B9E40[p][0][9] = 0.0;
        D_803B9E40[p][0][10] = 2.0f;
        D_803B9E40[p][0][11] = 15.0f;
        D_803B9E40[p][1][9] = 0.0;
        D_803B9E40[p][1][10] = 2.0f;
        D_803B9E40[p][1][11] = 15.0f;
        func_8039FF04(p);
        for (i = 0; i < 13; i++) {
            D_8011103C[p + 1][i] = func_800CDA60(pl->owner, i, 0);
            D_80111080[p + 1][i] = func_800CDA60(pl->owner, i, 1);
            while (!func_800F7604(p, D_80111080[p + 1][i])) {
                D_80111080[p + 1][i]--;
                if (D_80111080[p + 1][i] < 0) { D_80111080[p + 1][i] = 0; break; }
            }
            func_800C7578(pl->owner, i, 1, D_80111080[p + 1][i]);
            D_8011119C[p + 1][i] = func_800CDA60(pl->owner, i, 2);
            while (!func_800F75D0(p, D_8011119C[p + 1][i])) {
                D_8011119C[p + 1][i]--;
                if (D_8011119C[p + 1][i] < 0) { D_8011119C[p + 1][i] = 0; break; }
            }
            func_800C7578(pl->owner, i, 2, D_8011119C[p + 1][i]);
            D_80111230[p + 1][i] = func_800CDA60(pl->owner, i, 3);
            while (!func_800F75B0(p, D_80111230[p + 1][i])) {
                D_80111230[p + 1][i]--;
                if (D_80111230[p + 1][i] < 0) { D_80111230[p + 1][i] = 0; break; }
            }
            func_800C7578(pl->owner, i, 3, D_80111230[p + 1][i]);
            D_8011128C[p + 1][i] = func_800CDA60(pl->owner, i, 4);
            if (D_8014A110 != 4 && D_8011128C[p + 1][i] > 1) D_8011128C[p + 1][i] = 1;
            D_80111604[p + 1][i] = func_800CDA60(pl->owner, i, 5);
            D_80111648[p + 1][i] = func_800CDA60(pl->owner, i, 6);
            D_8011168C[p + 1][i] = func_800CDA60(pl->owner, i, 7);
            D_8011157C[p + 1][i] = func_800CDA60(pl->owner, i, 8);
            D_801115C0[p + 1][i] = func_800CDA60(pl->owner, i, 9);
            D_801112DC[p + 1][i] = func_800CDA60(pl->owner, i, 10) / 100.0f + 0.75f;
            D_801113E0[p + 1][i] = func_800CDA60(pl->owner, i, 11) / 100.0f + 0.75f;
            if (!D_801407D0) {
                D_801112DC[p + 1][i] = D_801112DC[0][i];
                D_801113E0[p + 1][i] = D_801113E0[0][i];
            }
        }
        D_803B9FD0[p] = func_800CDE88(pl->owner);
        while (!func_800F7620(p, D_803B9FD0[p])) {
            if (!D_803B9FD0[p]) D_803B9FD0[p] = 12;
            else D_803B9FD0[p]--;
        }
        for (D_803B9FD4[p] = 0; D_803B9FD4[p] < D_803BA020[p]; D_803B9FD4[p]++) {
            if (D_803B9FD0[p] == D_803B9FD8[p][D_803B9FD4[p]]) break;
        }
        if (D_803B9FD4[p] >= D_803BA020[p]) {
            D_803B9FD4[p] = p + 1;
            D_803B9FD0[p] = D_803B9FD8[p][D_803B9FD4[p]];
            func_800CCA04(pl->owner, D_803B9FD0[p]);
        }
        D_803BA018[p] = -1;
        func_8039E3BC(p);
        for (i = 0; i < D_803BA020[p]; i++) {
            D_803B9BC8[p][i].position = i - D_803B9FD4[p];
            if (D_803B9BC8[p][i].position < 0) D_803B9BC8[p][i].position += D_803BA020[p];
            D_803B9BC8[p][i].car = D_803B9FD8[p][i] + p * 13;
            D_803B9BC8[p][i].angle = 3.1415927f;
        }
        if (D_8014A110 == 6) D_8014A118[p].paint = 6;
        else {
            D_8014A118[p].paint = func_800CDDE8((void **)pl->owner);
            while (!func_800F75EC(p, D_8014A118[p].paint)) {
                D_8014A118[p].paint -= 2;
                if ((s32)D_8014A118[p].paint < 0) { D_8014A118[p].paint += 2; break; }
            }
        }
        for (i = 0; i < 13; i++)
            D_80110E78[p + 1][i] = D_8014A118[p].paint;
    }
    D_803B28AC = sound_control(0, 0, D_803B2AB8, 38);
    particle_velocity_set();
    effects_update_emitters();
    set_race_state();
    D_803B28A8 = 1;
}
