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
            ambient_sound_set(D_80110650[0] - 8, h - 4, D_80110650[0] + D_80110650[1] + 8,
                              h + D_80110650[2] + 4, 176, 0, 0, 0);
        }
        D_80110634 = 1;
    }
    return 1;
}

static void text_on(f32 z)
{
    render_helper(z);
    D_80118E20 = 1;
    D_80118E24 = 1;
}

static void text_off(f32 z)
{
    D_80118E24 = 3;
    D_80118E20 = 0;
    render_helper(z);
}

static void print_centered(s32 x, s32 y, char *s)
{
    camera_auto_follow(x, y, 320, 110, -1, 0, s);
}

s32 func_80100B8C(s32 arg0)
{

    text_on(0.0f);
    if (D_801613E8 == 1) {
        font_set(13);
        dispatch_handler(22);
        print_centered(160, 10, D_8012029C);
        print_centered(160, 190, countdown_object[33]);
    } else {
        font_set(11);
        dispatch_handler(22);
        print_centered(160, 185, countdown_object[32]);
    }
    dispatch_handler(1);
    text_off(-1.0f);
    return 1;
}
