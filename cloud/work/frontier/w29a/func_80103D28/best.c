/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80103D28 = GC DrawUnlocked (highscorescr.c): per player with new unlocks (D_80142694[p] =
 * gStuffUnlocked), count the items unlocked this race (new flag set, old flag clear) over seven categories,
 * resize the player's name pane (D_80149418[p], ChangePaneSize = crowd_cheer_play) around the list and draw
 * the header lines (fixed[55], fixed[56]) and one line per new item, centred on the player rect.
 * Internal (no callee saves); only caller func_80105480 (unmatched) -> provisional at best.
 * w29a draft, complete. Measured KEPT (blob_unit without --internal, so +9 callee saves):
 *   FAIL func_80103D28: 619 of 629 words differ; compiled body is 631 words | words 573 ops 103 | frame 264/232
 * The ops rows are the save/restore block, the x/y narrowing copies and the spill homes; the control flow,
 * category order/sizes, globals and every call's arguments agree with retail. Internal measurement needs a
 * real caller: umerge inlines it into any C stand-in (1, 2 or 6 sites, with blob_unit's dead block, with a
 * 600-byte caller frame, stand-in defined first: all inlined), so it waits for func_80105480 (GC DrawScores?).
 * GC names: gStuffUnlocked D_80142694, namePanes D_80149418, gPlayerRects D_801148E8 (Rect.cx/cy at +8/+10),
 * gNumWindows D_80151AD0, gTrackTitles D_8011A8D8, CarTitles D_80110D70, ChangePaneSize crowd_cheer_play,
 * TB_GetFontLineHeight object_bytes_sum_global, HAL_GetTextWidth object_manager_update (u32: retail `srl`),
 * TB_Sprintf fcvt_wrapper ("%s %s" style formats D_8012062C..D_8012064C), header lines fixed[55]/fixed[56].
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct Rect { s16 x, y, w, h; s16 cx, cy; } Rect;
typedef struct UIdx { u16 h[48]; } UIdx;
typedef struct TextState { void *word0; char **fixed; void *word8; UIdx *indices; char **strings; } TextState;

extern OSMesgQueue D_801461D0;
extern TextState countdown_state;
extern s16 active_player_count, gNumWindows;      /* gNumPlayers, gNumWindows */
extern Rect D_801148E8[][4];                     /* gPlayerRects */
extern s8 *namePanes[];                         /* namePanes */
extern s8 D_80142694[];                          /* gStuffUnlocked[p] */
extern s8 D_80150DD8[][19], D_801426A0[][19];    /* tracks: unlocked now / before */
extern s8 D_80150E30[][13], D_801426F0[][13];    /* cars */
extern s8 D_80150E88[][4], D_80142750[][4];
extern s8 D_80150EB8[][8], D_80142780[][8];
extern s8 D_80150ED8[][9], D_80142AD8[][9];
extern s8 D_80150F00[][5], D_80142D78[][5];
extern s8 D_80150F40[][6], D_80142DC0[][6];
extern char *D_8011A8D8[];                       /* gTrackTitles */
extern char *D_80110D70[];                       /* CarTitles */
extern char D_8012062C[], D_80120634[], D_8012063C[], D_80120644[], D_8012064C[];
extern s32 osRecvMesg(OSMesgQueue *, void **, s32), osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32);
extern u32 object_manager_update(char *, s32);
extern s16 object_bytes_sum_global(void);
extern void crowd_cheer_play(void *, s32, s32, s32, s32);
extern void dispatch_handler(s32);
extern void state_utility(s16, s16, char *);
extern s32 fcvt_wrapper(char *, char *, ...);

void du_gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
void du_gfx_unlock(void) { osSendMesg(&D_801461D0, 0, 0); }
s32 du_font_set(s32 font)
{
    s32 old;
    du_gfx_lock();
    old = slot_state_setup(font);
    du_gfx_unlock();
    return old;
}

void DrawUnlocked(void)
{
    s16 x;
    s32 y;
    s32 i;
    s32 p;
    char str[64];
    s32 stuffCount;

    du_font_set(11);
    for (p = 0; p < active_player_count; p++) {
        if (D_80142694[p] == 0) continue;
        stuffCount = 3;
        for (i = 0; i < 19; i++) if (D_80150DD8[p][i] && !D_801426A0[p][i]) stuffCount++;
        for (i = 0; i < 13; i++) if (D_80150E30[p][i] && !D_801426F0[p][i]) stuffCount++;
        for (i = 0; i < 4; i++) if (D_80150E88[p][i] && !D_80142750[p][i]) stuffCount++;
        for (i = 0; i < 8; i++) if (D_80150EB8[p][i] && !D_80142780[p][i]) stuffCount++;
        for (i = 0; i < 9; i++) if (D_80150ED8[p][i] && !D_80142AD8[p][i]) stuffCount++;
        for (i = 0; i < 5; i++) if (D_80150F00[p][i] && !D_80142D78[p][i]) stuffCount++;
        for (i = 0; i < 6; i++) if (D_80150F40[p][i] && !D_80142DC0[p][i]) stuffCount++;
        x = D_801148E8[gNumWindows - 1][p].cx;
        y = D_801148E8[gNumWindows - 1][p].cy - stuffCount * object_bytes_sum_global() / 2;
        if (namePanes[p] != 0) {
            crowd_cheer_play(namePanes[p], D_801148E8[gNumWindows - 1][p].x, y - 4,
                D_801148E8[gNumWindows - 1][p].x + D_801148E8[gNumWindows - 1][p].w,
                y + stuffCount * object_bytes_sum_global() + 4);
        }
        dispatch_handler(1);
        state_utility(x - (object_manager_update(countdown_state.fixed[55], -1) >> 1), y, countdown_state.fixed[55]);
        y += object_bytes_sum_global();
        state_utility(x - (object_manager_update(countdown_state.fixed[56], -1) >> 1), y, countdown_state.fixed[56]);
        y += object_bytes_sum_global() * 2;
        dispatch_handler(22);
        for (i = 0; i < 19; i++) {
            if (D_80150DD8[p][i] && !D_801426A0[p][i]) {
                state_utility(x - (object_manager_update(D_8011A8D8[i], -1) >> 1), y, D_8011A8D8[i]);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 13; i++) {
            if (D_80150E30[p][i] && !D_801426F0[p][i]) {
                state_utility(x - (object_manager_update(D_80110D70[i], -1) >> 1), y, D_80110D70[i]);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 4; i++) {
            if (D_80150E88[p][i] && !D_80142750[p][i]) {
                fcvt_wrapper(str, D_8012062C, countdown_state.strings[countdown_state.indices->h[44] + i], countdown_state.fixed[42]);
                state_utility(x - (object_manager_update(str, -1) >> 1), y, str);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 8; i++) {
            if (D_80150EB8[p][i] && !D_80142780[p][i]) {
                fcvt_wrapper(str, D_80120634, countdown_state.strings[countdown_state.indices->h[4] + i], countdown_state.strings[countdown_state.indices->h[2] + 1]);
                state_utility(x - (object_manager_update(str, -1) >> 1), y, str);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 9; i++) {
            if (D_80150ED8[p][i] && !D_80142AD8[p][i]) {
                fcvt_wrapper(str, D_8012063C, countdown_state.strings[countdown_state.indices->h[6] + i], countdown_state.strings[countdown_state.indices->h[2] + 3]);
                state_utility(x - (object_manager_update(str, -1) >> 1), y, str);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 5; i++) {
            if (D_80150F00[p][i] && !D_80142D78[p][i]) {
                fcvt_wrapper(str, D_80120644, countdown_state.strings[countdown_state.indices->h[7] + i], countdown_state.strings[countdown_state.indices->h[2] + 4]);
                state_utility(x - (object_manager_update(str, -1) >> 1), y, str);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 6; i++) {
            if (D_80150F40[p][i] && !D_80142DC0[p][i]) {
                fcvt_wrapper(str, D_8012064C, countdown_state.strings[countdown_state.indices->h[8] + i], countdown_state.strings[countdown_state.indices->h[2] + 5]);
                state_utility(x - (object_manager_update(str, -1) >> 1), y, str);
                y += object_bytes_sum_global();
            }
        }
    }
}

