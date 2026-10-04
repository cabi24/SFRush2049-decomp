void credits_scroll(void)
{
    char buf[76];
    s32 w;

    font_set(13);
    if (D_8011064C) {
        fcvt_wrapper(buf, D_8011F6B0, D_8015698C + 1, countdown_object[233]);
        w = object_manager_update(buf, -1);
    } else {
        w = object_manager_update(countdown_object[52], -1);
    }
    if (D_80110648) {
        crowd_cheer_play(D_80110648, 152 - w / 2, 6, w / 2 + 168, object_bytes_sum_global() + 14);
    } else {
        D_80110648 = ambient_sound_set(152 - w / 2, 6, w / 2 + 168, object_bytes_sum_global() + 14, 176, 0, 0, 0);
    }
}


void time_result_display(void)
{
    char buf[76];
    s32 w;

    font_set(13);
    if (D_80116DA8) {
        fcvt_wrapper(buf, D_80121004, D_8015698C + 1, countdown_object[233]);
        w = object_manager_update(buf, -1);
    } else {
        w = object_manager_update(countdown_object[50], -1);
    }
    if (D_80116DA4) {
        crowd_cheer_play(D_80116DA4, 152 - w / 2, 6, w / 2 + 168, object_bytes_sum_global() + 14);
    } else {
        D_80116DA4 = ambient_sound_set(152 - w / 2, 6, w / 2 + 168, object_bytes_sum_global() + 14, 176, 0, 0, 0);
    }
}

