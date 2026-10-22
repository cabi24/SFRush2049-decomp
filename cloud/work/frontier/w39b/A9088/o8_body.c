/* w39b: func_803A9088 = drawDeletePlayer@playerscr.c, straight MAT3 port (MAT3 local names/order, MAT3 statement
 * shapes; N64 deltas from the retail words: 14 menu items, buttons 14/15/16, text indices -10, HAL_SetTextDepth
 * calls, no /2 screen scaling, pak name table, N64 case -4 text, flash colours by value). */
#define abs(x) ((x) >= 0 ? (x) : -(x))
void func_803A9088(s32 p)
{
    s32 x;
    s32 y;
    s32 cx;
    u8 *str;
    u8 str1[128];
    s16 paneT;
    s32 menuItem;
    s32 i;
    s16 screenPos[2];
    f32 gTextPosScale = D_803B5D10;
    Profile **player;
    s16 w;
    f32 delta;
    if (D_8014A108 > 1) gTextPosScale *= 0.5f;
    if (D_8014A108 > 1) object_create(12); else object_create(13);
    y = 0;
    str = countdown_state.text[140];
    dispatch_handler(D_801146AC[p]);
    func_800B669C(y, 1);
    brake_light_update(p, D_803B470C[p][16].position, &D_80150B70[p], 0, screenPos);
    cx = screenPos[0];
    y = screenPos[1];
    func_800F6928(D_803B470C[p][16].position[2] - 10.0f);
    dispatch_handler(1);
    state_utility(sound_pitch_diff_halved(str, cx), y, str);
    y += object_bytes_sum_global();
    if (D_8014A108 > 1) object_create(12); else object_create(10);
    if (D_803BADCC[p] == 1) {
        func_800B669C(0, 3);
        func_800F6928(0.0f);
        y += object_bytes_sum_global();
        paneT = y - 4;
        fcvt_wrapper(str1, D_803B89A4, countdown_state.text[141], D_803BADD8[p], countdown_state.text[142]);
        dispatch_handler(1);
        func_800B669C(1, 3);
        camera_auto_follow(cx, y, D_803B5D50[D_8014A108 - 1][p].width,
            D_803B5D50[D_8014A108 - 1][p].y + D_803B5D50[D_8014A108 - 1][p].height - y, -1, 0, str1);
        func_800B669C(0, 3);
        y += object_bytes_sum_global() * (camera_zoom_fov(D_803B5D50[D_8014A108 - 1][p].width, str1) + 1);
        str = countdown_state.indexed[countdown_state.indices[24]];
        dispatch_handler(D_803BADD0[p] == 1 ? 22 : 1);
        state_utility(sound_pitch_diff_calc(str, cx - 5), y, str);
        str = countdown_state.indexed[countdown_state.indices[24] + 1];
        dispatch_handler(D_803BADD0[p] == 0 ? 22 : 1);
        state_utility(cx + 5, y, str);
        y += object_bytes_sum_global();
        crowd_cheer_play(D_803BAC08[p], cx - D_803B5D50[D_8014A108 - 1][p].width / 2 - 4, paneT,
            cx + D_803B5D50[D_8014A108 - 1][p].width / 2 + 4, y + 4);
    } else if (D_803BADD4[p] == 1) {
        func_800B669C(0, 3);
        func_800F6928(0.0f);
        y += object_bytes_sum_global();
        paneT = y - 4;
        fcvt_wrapper(str1, countdown_state.text[145], D_803BADD8[p]);
        dispatch_handler(1);
        func_800B669C(1, 3);
        camera_auto_follow(cx, y, D_803B5D50[D_8014A108 - 1][p].width,
            D_803B5D50[D_8014A108 - 1][p].y + D_803B5D50[D_8014A108 - 1][p].height - y, -1, 0, str1);
        func_800B669C(0, 3);
        y += object_bytes_sum_global() * camera_zoom_fov(D_803B5D50[D_8014A108 - 1][p].width, str1);
        crowd_cheer_play(D_803BAC08[p], cx - D_803B5D50[D_8014A108 - 1][p].width / 2 - 4, paneT,
            cx + D_803B5D50[D_8014A108 - 1][p].width / 2 + 4, y + 4);
    } else {
        crowd_cheer_play(D_803BAC08[p], -20, 0, -10, 10);
        for (menuItem = 0; menuItem < 14; menuItem++) {
            if (D_803B470C[p][menuItem].angle > 0.6981317f && D_803B470C[p][menuItem].angle < 2.443461f) continue;
            if (!D_803B470C[p][menuItem].alpha) continue;
            func_800ED66C(D_803B470C[p][menuItem].alpha);
            if (D_803B470C[p][menuItem].angle < 1.5707964f) {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(1);
            } else {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(22);
            }
            func_800F6928(D_803B470C[p][menuItem].position[2] - 10.0f);
            brake_light_update(p, D_803B470C[p][menuItem].position, &D_80150B70[p], 0, screenPos);
            if (D_803B470C[p][menuItem].kind == 9) x = screenPos[0] - 105.0f * gTextPosScale * 300.0f / D_803B5D24;
            else if (D_803B470C[p][menuItem].kind == 7) x = screenPos[0] - 65.0f * gTextPosScale * 300.0f / D_803B5D24;
            else x = screenPos[0] - 45.0f * gTextPosScale * 300.0f / D_803B5D24;
            y = screenPos[1];
            if (menuItem == 0 || (menuItem >= 3 && menuItem < 11)) {
                brake_light_update(p, D_803B470C[p][14].position, &D_80150B70[p], 0, screenPos);
                cx = screenPos[0] - 45.0f * gTextPosScale * 300.0f / D_803B5D24;
            } else if (menuItem == 1) {
                brake_light_update(p, D_803B470C[p][15].position, &D_80150B70[p], 0, screenPos);
                cx = screenPos[0] - 25.0f * gTextPosScale * 300.0f / D_803B5D24;
            }
            if (menuItem == 1) {
                dispatch_handler(D_803BADC8[p] == 1 ? 22 : 1);
                str = countdown_state.indexed[countdown_state.indices[21] + menuItem];
                state_utility(x, y, str);
                str = D_8011EAD0[D_803BADBC[p]];
                state_utility(cx, y, str);
            } else if (menuItem == 2) {
                switch (D_803BADC0[p]) {
                case -4: str = countdown_state.text[165]; break;
                case -3: str = countdown_state.text[169]; break;
                case 0: str = countdown_state.text[148]; break;
                case -2: str = countdown_state.text[166]; break;
                case -5: str = countdown_state.text[161]; break;
                case -1: str = countdown_state.text[168]; break;
                }
                delta = 65.0f * gTextPosScale * 300.0f / D_803B5D24;
                x += delta;
                dispatch_handler(0);
                state_utility(sound_pitch_diff_halved(str, x), y, str);
            } else if (menuItem >= 3 && menuItem < 11) {
            i = 0;
            for (player = D_803BAC80[p]; player; ) {
                if ((*player)->file && (*(*player)->file)->controller == D_803BADBC[p]) {
                    if (i == abs(menuItem - D_803BADC8[p])) break;
                    i++;
                }
                if (menuItem < D_803BADC8[p]) player = (*player)->previous;
                else player = (*player)->next;
            }
            if (!player) {
                player = (menuItem > D_803BADC8[p] || D_803BADC0[p] < 8) ? D_8012E6E0 : D_8012E6E4;
                for (; player;) {
                    if ((*player)->file && (*(*player)->file)->controller == D_803BADBC[p]) {
                        if (i == abs(menuItem - ((menuItem > D_803BADC8[p] || D_803BADC0[p] < 8) ? 3 : 10))) break;
                        i++;
                    }
                    player = (menuItem > D_803BADC8[p] || D_803BADC0[p] < 8) ? (*player)->next : (*player)->previous;
                }
            }
            if (player) {
                str = (*player)->name;
                w = D_8014A108 == 1 ? 98 : (D_8014A108 == 2 ? 70 : 55);
                if ((u32)object_manager_update(str, -1) > w) mode_byte_set(0);
                if (D_803BADC8[p] == menuItem && D_803BAC80[p] == player)
                    dispatch_handler(22);
                else {
                    dispatch_handler(1);
                    if (func_80395E24(p, player)) dispatch_handler(0);
                    if (!D_803BAD00[p][menuItem]) dispatch_handler(4);
                }
                music_fade(x, y, w, str);
                mode_byte_set(-1);
            }

            }
            object_bytes_sum_global();
        }
    }
    func_800B669C(0, 3);
    func_800ED66C(-1.0f);
    render_helper(0.0f);
}
