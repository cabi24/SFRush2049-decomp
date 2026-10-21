/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Context (not a member): byte-identical copy of the locked credits_scroll (_TitlePane@audioscr.c) from
 * src/blob/groups/credits_scroll_grp/gr3_c.c. It is font_set_g3c's second call site in the whole-program unit;
 * without it umerge inlines font_set_g3c into time_result_display differently (the dead `move s0,v0` of the
 * inlined HAL_SetFont return value disappears). */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
extern char **countdown_object;
extern s32 D_8015698C;
extern s8 D_8011064C;
extern s32 D_80110648;
extern char D_8011F6B0[];
void fcvt_wrapper(char *, char *, ...);
s32 object_manager_update(char *, s32);
s16 object_bytes_sum_global(void);
void crowd_cheer_play(s32, s32, s32, s32, s32);
s32 ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);
s32 font_set_g3c(s32 font);
void credits_scroll(void)
{
    char buf[76];
    s32 w;

    font_set_g3c(13);
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
