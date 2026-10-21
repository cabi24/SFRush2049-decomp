/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * time_result_display (0x800DA174, 332 B) = _TitlePane@optionscr.c (MAT3 GameCube optionscr.c _TitlePane,
 * GC 0x80074F48): HAL_SetFont(13) (inlined: font semaphore osRecvMesg/osSendMesg around _HAL_SetFont =
 * slot_state_setup); title width from TB_Sprintf("CONTROLLER %d %s") (fcvt_wrapper, D_80121004) when the
 * controller is removed (gControllerRemoved = D_80116DA8), else gText.single[50]; then ChangePaneSize
 * (crowd_cheer_play) on gTitlePane (D_80116DA4) or InitPane (ambient_sound_set) to create it.
 * Internal (private IPA: retail uses s0/s1/s2/s4 unsaved), called by updateOptionScreen (func_800DB1E0,
 * this group) and by the unmatched replay_save_prompt.
 * Body: the shape of the locked _TitlePane@audioscr.c (credits_scroll, src/blob/groups/credits_scroll_grp/
 * gr3_c.c) with optionscr's globals; retail is instruction-for-instruction the same as credits_scroll.
 * `buf[76]` as in credits_scroll (MAT3 str1[64]; the larger buffer is that function's disclosed frame size).
 */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
extern char **countdown_object;
extern s32 D_8015698C;
extern s8 D_80116DA8;
extern s32 D_80116DA4;
extern char D_80121004[];
void fcvt_wrapper(char *, char *, ...);
s32 object_manager_update(char *, s32);
s16 object_bytes_sum_global(void);
void crowd_cheer_play(s32, s32, s32, s32, s32);
s32 ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);

void gfx_lock_g3c(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_g3c(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_g3c(s32 font)
{
    s32 old;

    gfx_lock_g3c();
    old = slot_state_setup(font);
    gfx_unlock_g3c();
    return old;
}

void time_result_display(void)
{
    char buf[76];
    s32 w;

    font_set_g3c(13);
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

/* stand-in caller (context, kept): keeps time_result_display out of line as in retail, where the unmatched
 * replay_save_prompt (lane w36q) is its second caller. */
void __standin_time_result_display(void)
{
    time_result_display();
}
