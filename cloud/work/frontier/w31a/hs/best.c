/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * w31a high-score unit (GC highscorescr.c), complete drafts of all four N64 functions:
 *   func_80103D28 = DrawUnlocked (internal), func_80104704 = DrawName(p) (internal, p in s5 by IPA),
 *   func_80104B14 = DrawGhost (internal), func_80105480 = animText@highscorescr.c (kept; DrawCircuitCode and
 *   DrawScores are inlined into it).
 * Unit (Pi): blob_unit --tag w31a score func_80105480 func_80104704 func_80104B14 func_80103D28 --with THIS
 *   $(cat cloud/work/frontier/w31a/hs/uargs_hs.txt) --neighbours
 *   -> EQUAL func_80104704 (260 words, internal); func_80104B14 17/601 (6 unverified own-.rodata rows; colour);
 *      func_80105480 27/445 (frame 704 vs 584: helper inline areas); func_80103D28 588/629 (frame/length exact; colour).
 * NOT a match deliverable: func_80104704's only caller func_80105480 is unmatched (provisional).
 * Shaping devices (disclosed): non-static inlined helpers (w17j: not on keep) hs_SetFont (void HAL_SetFont wrapper,
 *   DrawName), hs_SetJust (TB_SetJust, DrawGhost), gh_font_set/gh_SetFont (DrawGhost's own font helper copy + void
 *   wrapper), hs_CenterText at the 9 DrawUnlocked sites; one unused local `s16 gFreePages` in DrawGhost (GC local;
 *   exact frame residual 568 -> 576, one slot).
 * The rest of this header is the w29a/w30a DrawUnlocked note (historical).
 */
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
typedef float f32;
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

s16 hs_CenterText(char *text, s16 centerX);

void func_80103D28(void)
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
        state_utility(hs_CenterText(countdown_state.fixed[55], x), y, countdown_state.fixed[55]);
        y += object_bytes_sum_global();
        state_utility(hs_CenterText(countdown_state.fixed[56], x), y, countdown_state.fixed[56]);
        y += object_bytes_sum_global() * 2;
        dispatch_handler(22);
        for (i = 0; i < 19; i++) {
            if (D_80150DD8[p][i] && !D_801426A0[p][i]) {
                state_utility(hs_CenterText(D_8011A8D8[i], x), y, D_8011A8D8[i]);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 13; i++) {
            if (D_80150E30[p][i] && !D_801426F0[p][i]) {
                state_utility(hs_CenterText(D_80110D70[i], x), y, D_80110D70[i]);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 4; i++) {
            if (D_80150E88[p][i] && !D_80142750[p][i]) {
                fcvt_wrapper(str, D_8012062C, countdown_state.strings[countdown_state.indices->h[44] + i], countdown_state.fixed[42]);
                state_utility(hs_CenterText(str, x), y, str);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 8; i++) {
            if (D_80150EB8[p][i] && !D_80142780[p][i]) {
                fcvt_wrapper(str, D_80120634, countdown_state.strings[countdown_state.indices->h[4] + i], countdown_state.strings[countdown_state.indices->h[2] + 1]);
                state_utility(hs_CenterText(str, x), y, str);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 9; i++) {
            if (D_80150ED8[p][i] && !D_80142AD8[p][i]) {
                fcvt_wrapper(str, D_8012063C, countdown_state.strings[countdown_state.indices->h[6] + i], countdown_state.strings[countdown_state.indices->h[2] + 3]);
                state_utility(hs_CenterText(str, x), y, str);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 5; i++) {
            if (D_80150F00[p][i] && !D_80142D78[p][i]) {
                fcvt_wrapper(str, D_80120644, countdown_state.strings[countdown_state.indices->h[7] + i], countdown_state.strings[countdown_state.indices->h[2] + 4]);
                state_utility(hs_CenterText(str, x), y, str);
                y += object_bytes_sum_global();
            }
        }
        for (i = 0; i < 6; i++) {
            if (D_80150F40[p][i] && !D_80142DC0[p][i]) {
                fcvt_wrapper(str, D_8012064C, countdown_state.strings[countdown_state.indices->h[8] + i], countdown_state.strings[countdown_state.indices->h[2] + 5]);
                state_utility(hs_CenterText(str, x), y, str);
                y += object_bytes_sum_global();
            }
        }
    }
}


/* ---- func_80105480 = GC animText@highscorescr.c (w30a draft) ---- */
typedef struct HsIdx { u8 pad54[54]; u16 tableNames; u8 pad56[54]; u16 scoreNames; } HsIdx;
extern s32 gHighScoreState;
extern char D_80143EF0[];                        /* gCircuitCode */
extern s8 D_801148D0;                            /* gTable */
extern s8 D_80151AC0[][5];                       /* gNewTotalScore[table][5] */
extern s32 D_801146AC[];                         /* playerTextColor */
extern char *D_80143FE0[];                       /* gScoreNames */
extern char D_80143F20[][10];                    /* gScoreTimes */
extern s8 D_80143F18[];                          /* gDrawName */
extern void render_helper(f32);
extern u32 func_800BE744(char *);                /* strlen */
extern s32 sprintf(char *, const char *, ...);
extern void race_position_update(char *, char *, s32);
extern char *D_80117420;                         /* gPlayerNameChars */
extern s16 D_80144010[];                         /* gNameSelect[p] */
extern char D_80143A50[][13];                    /* gScoreName[p] */
extern char D_801149A8[], D_801149AC[], D_801149B0[];   /* DrawName statics del$1225 "DEL", end$1226 "END", spc$1227 "SPC" */
typedef struct Ghost { u8 pad0[10]; char name[22]; f32 time; } Ghost;   /* GC GHOST_DATA: name +10 (GC +0xe), time +32 */
extern Ghost **D_801461A8;                       /* gSavedGhost */
extern Ghost **D_80146198[];                     /* gPakGhosts[pak] */
extern s8 D_801460C0;                            /* gGhostPak */
extern s8 D_801460F0;                            /* gGhostMenuConflict */
extern s8 D_801460FC;                            /* gGhostMenuConfirm */
extern s8 D_80146148;                            /* gGhostMenuNotify */
extern s8 D_80146131;                            /* confirm menu selection (two options) */
extern s32 D_80144DA0;                           /* gGhostMenuSelect */
extern s32 D_801461C0[];                         /* gGhostStatus[pak] */
extern char *D_8011EAD0[];                       /* gPS_PakName[pak] ("1".."4") */
extern s32 D_80118E20[2];                        /* TB_SetJust pair */
extern void camera_auto_follow(s16, s16, s16, s16, s32, s32, char *);  /* TB_DrawTextScrollBox */

s16 hs_CenterText(char *text, s16 centerX)
{
    return centerX - (object_manager_update(text, -1) >> 1);
}

s16 hs_RightText(char *text, s16 rightX)
{
    return rightX - object_manager_update(text, -1);
}

s32 hs_font_set(s32 font)
{
    s32 old;
    du_gfx_lock();
    old = slot_state_setup(font);
    osSendMesg(&D_801461D0, 0, 0);
    return old;
}

void hs_SetFont(s32 font)
{
    hs_font_set(font);
}

/* func_80104704 = GC DrawName(p)@highscorescr.c: name-entry grid for player p (internal; p arrives in s5). */
void func_80104704(s32 p)
{
    char ltr[2];
    s32 x;
    s32 y;
    s32 i;

    hs_SetFont(12);
    ltr[1] = 0;
    y = D_801148E8[gNumWindows - 1][p].y + 4;
    x = D_801148E8[gNumWindows - 1][p].cx;
    dispatch_handler(1);
    state_utility(hs_CenterText(countdown_state.fixed[150], x), y, countdown_state.fixed[150]);
    y = D_801148E8[gNumWindows - 1][p].y + 15;
    x = D_801148E8[gNumWindows - 1][p].cx;
    dispatch_handler(22);
    state_utility(hs_CenterText(D_80143A50[p], x), y, D_80143A50[p]);
    y = D_801148E8[gNumWindows - 1][p].y + 30;
    x = D_801148E8[gNumWindows - 1][p].cx - 48;
    for (i = 0; i < 54; i++) {
        ltr[0] = D_80117420[i];
        dispatch_handler(i == D_80144010[p] ? 22 : 1);
        state_utility(hs_CenterText(ltr, x), y, ltr);
        if ((i + 1) % 9 == 0) {
            y += 10;
            x = D_801148E8[gNumWindows - 1][p].cx - 48;
        } else {
            x += 12;
        }
    }
    x = D_801148E8[gNumWindows - 1][p].cx - 36;
    dispatch_handler(D_80144010[p] == -3 ? 22 : 16);
    state_utility(hs_CenterText(D_801149B0, x), y, D_801149B0);
    x += 36;
    dispatch_handler(D_80144010[p] == -2 ? 22 : 16);
    state_utility(hs_CenterText(D_801149A8, x), y, D_801149A8);
    x += 36;
    dispatch_handler(D_80144010[p] == -1 ? 22 : 16);
    state_utility(hs_CenterText(D_801149AC, x), y, D_801149AC);
}

void DrawCircuitCode(void)
{
    s32 x;
    s32 y;
    s32 i;
    s32 length;
    char str[64];

    hs_font_set(11);
    dispatch_handler(1);
    state_utility(hs_CenterText(countdown_state.fixed[64], 160), 80, countdown_state.fixed[64]);
    state_utility(hs_CenterText(countdown_state.fixed[65], 160), 120, countdown_state.fixed[65]);
    if (func_800BE744(D_80143EF0) > 22) length = 22;
    else length = func_800BE744(D_80143EF0);
    str[1] = 0;
    x = 160 - (length + 1) / 2 * 10 / 2;
    y = 95;
    for (i = 0; i < length; i++) {
        str[0] = D_80143EF0[i];
        state_utility(hs_CenterText(str, x + 5), y, str);
        if ((i + 1) % ((length + 1) / 2) == 0) {
            x = 160 - (length + 1) / 2 * 10 / 2;
            y += 10;
        } else {
            x += 10;
        }
    }
}

void DrawScores(void)
{
    s32 i;
    s32 numWid;
    char str[96];
    char label[256];

    hs_font_set(13);
    dispatch_handler(1);
    state_utility(hs_CenterText(countdown_state.strings[((HsIdx *)countdown_state.indices)->tableNames + D_801148D0], 160), 10, countdown_state.strings[((HsIdx *)countdown_state.indices)->tableNames + D_801148D0]);
    for (i = 0; i < 5; i++) {
        sprintf(str, "%d", i + 1);
        hs_font_set(5);
        dispatch_handler(D_80151AC0[D_801148D0][i] == -1 ? 1 : D_801146AC[D_80151AC0[D_801148D0][i]]);
        numWid = object_manager_update(str, -1);
        state_utility(16, 76 + i * 24, str);
        hs_font_set(11);
        state_utility(numWid + 16, 78 + i * 24, countdown_state.strings[((HsIdx *)countdown_state.indices)->scoreNames + i]);
    }
    for (i = 0; i < 5; i++) {
        hs_font_set(10);
        dispatch_handler(D_80151AC0[D_801148D0][i] == -1 ? 1 : D_801146AC[D_80151AC0[D_801148D0][i]]);
        race_position_update(label, D_80143FE0[i], 160);
        state_utility(69, 80 + i * 24, label);
    }
    for (i = 0; i < 5; i++) {
        hs_font_set(2);
        dispatch_handler(D_80151AC0[D_801148D0][i] == -1 ? 1 : D_801146AC[D_80151AC0[D_801148D0][i]]);
        state_utility(hs_RightText(D_80143F20[i], 304), 80 + i * 24, D_80143F20[i]);
    }
}

/* func_80104B14 = GC DrawGhost@highscorescr.c (internal; only caller func_80105480 state 4). */
void hs_SetJust(s32 h, s32 v)
{
    D_80118E20[0] = h;
    D_80118E20[1] = v;
}

s32 gh_font_set(s32 font)
{
    s32 old;
    du_gfx_lock();
    old = slot_state_setup(font);
    osSendMesg(&D_801461D0, 0, 0);
    return old;
}

void gh_SetFont(s32 font)
{
    gh_font_set(font);
}

void func_80104B14(void)
{
    s32 i;
    s32 y;
    char *str;
    s16 gFreePages;
    char str1[128];
    char time[16];

    gh_SetFont(13);
    dispatch_handler(1);
    str = countdown_state.fixed[219];
    state_utility(hs_CenterText(str, 160), 10, str);
    gh_SetFont(11);
    str = countdown_state.fixed[220];
    state_utility(hs_CenterText(str, 160), 55, str);
    gh_SetFont(10);
    y = object_bytes_sum_global() + 55;
    str = (*D_801461A8)->name;
    state_utility(hs_CenterText(str, 160), y, str);
    y += object_bytes_sum_global();
    gh_SetFont(2);
    fcvt_wrapper(time, "%01d:%02d.%03d", (s32)((*D_801461A8)->time / 60.0f), (s32)(*D_801461A8)->time % 60,
                 (s32)((*D_801461A8)->time * 1000.0f) % 1000);
    state_utility(hs_CenterText(time, 160), y, time);
    if (D_801460F0 == 1) {
        gh_SetFont(10);
        fcvt_wrapper(str1, countdown_state.fixed[223], (*D_80146198[D_801460C0])->name);
        hs_SetJust(1, 3);
        camera_auto_follow(160, 110, 130, object_bytes_sum_global() * 6, -1, 0, str1);
        hs_SetJust(0, 3);
    } else if (D_801460FC == 1) {
        gh_SetFont(10);
        y = object_bytes_sum_global();
        hs_SetJust(1, 3);
        camera_auto_follow(160, 110, 130, y * 4, -1, 0, countdown_state.fixed[221]);
        hs_SetJust(0, 3);
        y = object_bytes_sum_global() * 2 + 110;
        dispatch_handler(D_80146131 ? 22 : 1);
        str = countdown_state.strings[countdown_state.indices->h[24]];
        state_utility(hs_CenterText(str, 145), y, str);
        dispatch_handler(D_80146131 ? 1 : 22);
        str = countdown_state.strings[countdown_state.indices->h[24] + 1];
        state_utility(hs_CenterText(str, 175), y, str);
    } else if (D_80146148 == 1) {
        gh_SetFont(10);
        fcvt_wrapper(str1, countdown_state.fixed[222], D_8011EAD0[D_801460C0]);
        hs_SetJust(1, 3);
        camera_auto_follow(160, 110, 130, object_bytes_sum_global() * 6, -1, 0, str1);
        hs_SetJust(0, 3);
    } else {
        gh_SetFont(10);
        y = 110;
        for (i = 0; i < 4; i++) {
            dispatch_handler(i == D_80144DA0 ? 22 : 1);
            time[0] = 0;
            if (i == 1) {
                switch (D_801461C0[D_801460C0]) {
                case -4:
                    str = countdown_state.fixed[165];
                    break;
                case -3:
                    str = countdown_state.fixed[169];
                    break;
                case -2:
                    str = countdown_state.fixed[166];
                    break;
                case -1:
                    str = countdown_state.fixed[168];
                    break;
                case -5:
                    str = countdown_state.fixed[160];
                    break;
                case 0:
                    str = countdown_state.strings[countdown_state.indices->h[46] + i];
                    break;
                case 1:
                case 2:
                    str = (*D_80146198[D_801460C0])->name;
                    fcvt_wrapper(time, "%01d:%02d.%03d", (s32)((*D_80146198[D_801460C0])->time / 60.0f),
                                 (s32)(*D_80146198[D_801460C0])->time % 60,
                                 (s32)((*D_80146198[D_801460C0])->time * 1000.0f) % 1000);
                    break;
                }
                if (D_801461C0[D_801460C0] == 2) dispatch_handler(5);
            } else {
                str = countdown_state.strings[countdown_state.indices->h[46] + i];
            }
            if (i == 2 && D_801461C0[D_801460C0] != 0 && D_801461C0[D_801460C0] != 1) dispatch_handler(4);
            if (i == 1) {
                state_utility(hs_CenterText(str, 160), y, str);
                y += object_bytes_sum_global();
                if (time[0]) {
                    gh_SetFont(2);
                    state_utility(hs_CenterText(time, 160), y, time);
                    gh_SetFont(10);
                }
            } else {
                state_utility(95, y, str);
                if (i == 0) state_utility(hs_RightText(D_8011EAD0[D_801460C0], 225), y, D_8011EAD0[D_801460C0]);
            }
            dispatch_handler(1);
            y += object_bytes_sum_global();
        }
    }
}

s32 func_80105480(s32 arg0)
{
    s32 p;

    render_helper(0.0f);
    switch (gHighScoreState) {
    case 1:
        func_80103D28();
        break;
    case 2:
        DrawCircuitCode();
        break;
    case 3:
        for (p = 0; p < active_player_count; p++) {
            if (D_80143F18[p] == 1) func_80104704(p);
        }
        break;
    case 4:
        func_80104B14();
        break;
    case 5:
        DrawScores();
        break;
    }
    render_helper(-1.0f);
    return 1;
}
