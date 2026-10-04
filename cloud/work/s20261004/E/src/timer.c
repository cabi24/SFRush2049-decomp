s32 game_timer_display(s32 arg0)
{
    s16 h;

    if (!D_80110634) {
        if (!(state_word_a & 0x7C03FFFE)) {
            font_set(13);
            D_8011064C = !D_80156CF0[D_8015698C].flag;
            credits_scroll();
            h = object_bytes_sum_global() * 2 + 10;
            font_set(10);
            ambient_sound_set(D_80110650[0] - 8, h - 4, D_80110650[1] + D_80110650[0] + 8,
                              h + D_80110650[2] + 4, 176, 0, 0, 0);
        }
        D_80110634 = 1;
    }
    return 1;
}

s32 func_80100B8C(s32 arg0)
{

    render_helper(0.0f);
    func_800B669C(1, 1);
    if (D_801613E8 == 1) {
        font_set(13);
        dispatch_handler(22);
        camera_auto_follow(160, 10, 320, 110, -1, 0, D_8012029C);
        camera_auto_follow(160, 190, 320, 110, -1, 0, countdown_object[33]);
    } else {
        font_set(11);
        dispatch_handler(22);
        camera_auto_follow(160, 185, 320, 110, -1, 0, countdown_object[32]);
    }
    dispatch_handler(1);
    D_80118E20.second = 3;
    D_80118E20.first = 0;
    render_helper(-1.0f);
    return 1;
}
