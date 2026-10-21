void func_803AA3D8(void)
{
    s32 playerIdx;
    s32 i, x, y, cx;
    u8 *str;
    u8 str1[64];
    s16 screenPos[2];
    func_800B669C(0, 1);
    brake_light_update(0, D_803B5F28[17].position, &D_80150B70[0], 0, screenPos);
    cx = screenPos[0];
    y = screenPos[1];
    func_800F6928(D_803B5F28[17].position[2] - 10.0f);
    dispatch_handler(1);
    object_create(13);
    dispatch_handler(1);
    str = countdown_state.text[47];
    state_utility(sound_pitch_diff_halved(str, cx), y, str);
    y += object_bytes_sum_global() * 2;
    object_create(10);
    if (D_803BA84D == 1) {
        render_helper(0.0f);
        y += object_bytes_sum_global();
        if (D_803BA800) {
            fcvt_wrapper(str1, countdown_state.text[143], D_803BA840);
            str = str1;
        } else str = countdown_state.text[144];
        crowd_cheer_play(D_803B6530, cx - D_803B8ACC.width / 2 - 4, y - 4,
                         cx + D_803B8ACC.width / 2 + 4, 4 + y + ((camera_zoom_fov(D_803B8ACC.width, str) + 2) * object_bytes_sum_global()));
        dispatch_handler(1);
        func_800B669C(1, 3);
        camera_auto_follow(cx, y, D_803B8ACC.width, D_803B8ACC.height, -1, 0, str);
        func_800B669C(0, 3);
        y += object_bytes_sum_global() * (camera_zoom_fov(D_803B8ACC.width, str) + 1);
        str = NAME(24, 0);
        dispatch_handler(D_803BA850 == 1 ? 22 : 1);
        state_utility(sound_pitch_diff_calc(str, cx - 5), y, str);
        str = NAME(24, 1);
        dispatch_handler(D_803BA850 == 0 ? 22 : 1);
        state_utility(cx + 5, y, str);
    } else if (D_803BA854 == 1) {
        render_helper(0.0f);
        y += object_bytes_sum_global();
        if (D_803BA800) {
            fcvt_wrapper(str1, countdown_state.text[146], D_803BA840);
            str = str1;
        } else str = countdown_state.text[147];
        crowd_cheer_play(D_803B6530, cx - D_803B8ACC.width / 2 - 4, y - 4,
                         cx + D_803B8ACC.width / 2 + 4, 4 + y + (camera_zoom_fov(D_803B8ACC.width, str) * object_bytes_sum_global()));
        dispatch_handler(1);
        func_800B669C(1, 3);
        camera_auto_follow(cx, y, D_803B8ACC.width, D_803B8ACC.height, -1, 0, str);
        func_800B669C(0, 3);
    } else {
        for (i = 0; i < 11; i++) {
            crowd_cheer_play(D_803B6530, -20, 0, -10, 10);
            func_800F6928(D_803B5F28[i].position[2] - 10.0f);
            brake_light_update(0, D_803B5F28[i].position, &D_80150B70[0], 0, screenPos);
            if (D_803B5F28[i].kind == 11) x = screenPos[0];
            else if (D_803B5F28[i].kind == 9) x = screenPos[0] - 65.0f * D_803B64E8 * 300.0f / D_803B64FC;
            else x = screenPos[0] - 45.0f * D_803B64E8 * 300.0f / D_803B64FC;
            y = screenPos[1];
            if (i == 1) {
                brake_light_update(0, D_803B5F28[14].position, &D_80150B70[0], 0, screenPos);
                cx = screenPos[0] - 25.0f * D_803B64E8 * 300.0f / D_803B64FC;
            } else {
                brake_light_update(0, D_803B5F28[13].position, &D_80150B70[0], 0, screenPos);
                cx = screenPos[0] - 45.0f * D_803B64E8 * 300.0f / D_803B64FC;
            }
            if (i >= 3 && i < 11 && i == D_803BA808 &&
                (D_803B5F28[13].angle < 0.6981317f || D_803B5F28[13].angle > 2.443461f) && D_803B5F28[13].alpha) {
                func_800ED66C(D_803B5F28[13].alpha);
                if (D_803B5F28[13].angle < 1.5707964f) {
                    func_800BEA3C(D_801146BC, D_801146C0);
                    dispatch_handler(1);
                } else {
                    func_800BEA3C(D_801146BC, D_801146C0);
                    dispatch_handler(22);
                }
                str = NAME(50, 2);
                object_create(11);
                state_utility(cx, y, str);
                object_create(10);
            }
            if (D_803B5F28[i].angle > 0.6981317f && D_803B5F28[i].angle < 2.443461f) continue;
            if (!D_803B5F28[i].alpha) continue;
            func_800ED66C(D_803B5F28[i].alpha);
            if (D_803B5F28[i].angle < 1.5707964f) {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(1);
            } else {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(22);
            }
            if (i == 0) {
                str = NAME(50, 0);
                state_utility(x, y, str);
            } else if (i == 1) {
                str = NAME(50, 1);
                state_utility(x, y, str);
                str = D_8011EAD0[D_803B65A4];
                state_utility(cx, y, str);
            } else if (i == 2) {
                switch (D_803BA830[D_803B65A4]) {
                case -4: str = countdown_state.text[165]; break;
                case -3: str = countdown_state.text[169]; break;
                case 0: str = countdown_state.text[148]; break;
                case -2: str = countdown_state.text[166]; break;
                case -1: str = countdown_state.text[168]; break;
                }
                dispatch_handler(0);
                state_utility(sound_pitch_diff_halved(str, x), y, str);
            } else if (i >= 3 && i < 11) {
                Profile **player;
                if (!D_803BA800) {
                    for (playerIdx = 0, player = D_8012E6E0; player; player = (*player)->next) {
                        if ((*(*player)->file)->controller == D_803B65A4) {
                            if (playerIdx == i - 3) str = (*player)->name;
                            playerIdx++;
                        }
                    }
                } else {
                    for (playerIdx = 0, player = D_803BA800; player;
                         player = i < D_803BA808 ? (*player)->previous : (*player)->next) {
                        if ((*(*player)->file)->controller == D_803B65A4) {
                            if (playerIdx == (i - D_803BA808 >= 0 ? i - D_803BA808 : -(i - D_803BA808))) str = (*player)->name;
                            playerIdx++;
                        }
                    }
                }
                if ((u32)object_manager_update(str, -1) > 140) mode_byte_set(0);
                music_fade(x, y, 98, str);
                mode_byte_set(-1);
            }
            object_bytes_sum_global();
        }
    }
}
