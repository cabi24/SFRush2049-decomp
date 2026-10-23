/* High-score boundary research, N64 source with original MAT3 local/type evidence.
 * All known emitted native bodies are disclosed in group.json; claims are empty.
 * The authentic 128-byte ellipse and single HAL_SetFont graph replace old aliases.
 * DrawGhost retains the recorded GC local order; gFreePages is unused in the
 * N64 flow and has no code effect here. Its old pre-buffer placement is rejected
 * as a frame-shaping device. Root and Ghost remain partial; see verification.
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
extern s32 osRecvMesg(OSMesgQueue *, void **, s32), osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32);
extern u32 object_manager_update(void *, s16);
extern s16 object_bytes_sum_global(void);
extern void crowd_cheer_play(void *, s32, s32, s32, s32);
extern void dispatch_handler(s32);
extern void state_utility(s16, s16, void *);
extern void fcvt_wrapper(char *, char *, ...);

void du_gfx_lock(void);
void du_gfx_unlock(void);
s32 object_create(s32 font);

s16 sound_pitch_diff_halved(void *text, s16 centerX);

void DrawUnlocked(void);


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
extern void race_position_update(void *, void *, s16);
extern char *D_80117420;                         /* gPlayerNameChars */
extern s16 D_80144010[];                         /* gNameSelect[p] */
extern char D_80143A50[][13];                    /* gScoreName[p] */
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
typedef enum { J_LEFT=0,J_CENTER=1,J_RIGHT=2,J_BOTTOM=3 } JustEnum;
typedef struct { JustEnum h, v; } Just;
extern Just D_80118E20;                        /* TB_SetJust pair */
extern void camera_auto_follow(s16, s16, s16, s16, s32, s32, char *);  /* TB_DrawTextScrollBox */

s16 sound_pitch_diff_halved(void *text, s16 centerX);

s16 sound_pitch_diff_calc(void *text, s16 rightX);




/* func_80104704 = GC DrawName(p)@highscorescr.c: name-entry grid for player p (internal; p arrives in s5). */
void DrawName(s32 p);

void DrawCircuitCode(void);

void music_fade(s16 x, s16 y, s16 w, void *text);

void DrawScores(void);

/* func_80104B14 = GC DrawGhost@highscorescr.c (internal; only caller func_80105480 state 4). */
void func_800B669C(JustEnum h, JustEnum v)
{
    D_80118E20.h = h;
    D_80118E20.v = v;
}




void DrawGhost(void);

/* Original int x and nested guard recover native allocation without new storage. */
void DrawUnlocked(void)
{
    s32 x;
    s32 y;
    s32 i;
    s32 p;
    unsigned char str[64];
    s32 stuffCount;

    object_create(11);
    for (p = 0; p < active_player_count; p++) {
        if (D_80142694[p]) {
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
            state_utility(sound_pitch_diff_halved(countdown_state.fixed[55], x), y, countdown_state.fixed[55]);
            y += object_bytes_sum_global();
            state_utility(sound_pitch_diff_halved(countdown_state.fixed[56], x), y, countdown_state.fixed[56]);
            y += object_bytes_sum_global() * 2;
            dispatch_handler(22);
            for (i = 0; i < 19; i++) {
                if (D_80150DD8[p][i] && !D_801426A0[p][i]) {
                    state_utility(sound_pitch_diff_halved(D_8011A8D8[i], x), y, D_8011A8D8[i]);
                    y += object_bytes_sum_global();
                }
            }
            for (i = 0; i < 13; i++) {
                if (D_80150E30[p][i] && !D_801426F0[p][i]) {
                    state_utility(sound_pitch_diff_halved(D_80110D70[i], x), y, D_80110D70[i]);
                    y += object_bytes_sum_global();
                }
            }
            for (i = 0; i < 4; i++) {
                if (D_80150E88[p][i] && !D_80142750[p][i]) {
                    fcvt_wrapper(str, "%s %s", countdown_state.strings[countdown_state.indices->h[44] + i], countdown_state.fixed[42]);
                    state_utility(sound_pitch_diff_halved(str, x), y, str);
                    y += object_bytes_sum_global();
                }
            }
            for (i = 0; i < 8; i++) {
                if (D_80150EB8[p][i] && !D_80142780[p][i]) {
                    fcvt_wrapper(str, "%s %s", countdown_state.strings[countdown_state.indices->h[4] + i], countdown_state.strings[countdown_state.indices->h[2] + 1]);
                    state_utility(sound_pitch_diff_halved(str, x), y, str);
                    y += object_bytes_sum_global();
                }
            }
            for (i = 0; i < 9; i++) {
                if (D_80150ED8[p][i] && !D_80142AD8[p][i]) {
                    fcvt_wrapper(str, "%s %s", countdown_state.strings[countdown_state.indices->h[6] + i], countdown_state.strings[countdown_state.indices->h[2] + 3]);
                    state_utility(sound_pitch_diff_halved(str, x), y, str);
                    y += object_bytes_sum_global();
                }
            }
            for (i = 0; i < 5; i++) {
                if (D_80150F00[p][i] && !D_80142D78[p][i]) {
                    fcvt_wrapper(str, "%s %s", countdown_state.strings[countdown_state.indices->h[7] + i], countdown_state.strings[countdown_state.indices->h[2] + 4]);
                    state_utility(sound_pitch_diff_halved(str, x), y, str);
                    y += object_bytes_sum_global();
                }
            }
            for (i = 0; i < 6; i++) {
                if (D_80150F40[p][i] && !D_80142DC0[p][i]) {
                    fcvt_wrapper(str, "%s %s", countdown_state.strings[countdown_state.indices->h[8] + i], countdown_state.strings[countdown_state.indices->h[2] + 5]);
                    state_utility(sound_pitch_diff_halved(str, x), y, str);
                    y += object_bytes_sum_global();
                }
            }
        }
    }
}

void DrawCircuitCode(void)
{
    s32 x;
    s32 y;
    s32 i;
    s32 length;
    unsigned char str[64];

    object_create(11);
    dispatch_handler(1);
    state_utility(sound_pitch_diff_halved(countdown_state.fixed[64], 160), 80, countdown_state.fixed[64]);
    state_utility(sound_pitch_diff_halved(countdown_state.fixed[65], 160), 120, countdown_state.fixed[65]);
    if (func_800BE744(D_80143EF0) > 22) length = 22;
    else length = func_800BE744(D_80143EF0);
    str[1] = 0;
    x = 160 - (length + 1) / 2 * 10 / 2;
    y = 95;
    for (i = 0; i < length; i++) {
        str[0] = D_80143EF0[i];
        state_utility(sound_pitch_diff_halved(str, x + 5), y, str);
        if ((i + 1) % ((length + 1) / 2) == 0) {
            x = 160 - (length + 1) / 2 * 10 / 2;
            y += 10;
        } else {
            x += 10;
        }
    }
}

void DrawName(s32 p)
{
    static unsigned char del[4] = "DEL";
    static unsigned char end[4] = "END";
    static unsigned char spc[4] = "SPC";
    unsigned char ltr[2];
    s32 x;
    s32 y;
    s32 i;

    object_create(12);
    ltr[1] = 0;
    y = D_801148E8[gNumWindows - 1][p].y + 4;
    x = D_801148E8[gNumWindows - 1][p].cx;
    dispatch_handler(1);
    state_utility(sound_pitch_diff_halved(countdown_state.fixed[150], x), y, countdown_state.fixed[150]);
    y = D_801148E8[gNumWindows - 1][p].y + 15;
    x = D_801148E8[gNumWindows - 1][p].cx;
    dispatch_handler(22);
    state_utility(sound_pitch_diff_halved(D_80143A50[p], x), y, D_80143A50[p]);
    y = D_801148E8[gNumWindows - 1][p].y + 30;
    x = D_801148E8[gNumWindows - 1][p].cx - 48;
    for (i = 0; i < 54; i++) {
        ltr[0] = D_80117420[i];
        dispatch_handler(i == D_80144010[p] ? 22 : 1);
        state_utility(sound_pitch_diff_halved(ltr, x), y, ltr);
        if ((i + 1) % 9 == 0) {
            y += 10;
            x = D_801148E8[gNumWindows - 1][p].cx - 48;
        } else {
            x += 12;
        }
    }
    x = D_801148E8[gNumWindows - 1][p].cx - 36;
    dispatch_handler(D_80144010[p] == -3 ? 22 : 16);
    state_utility(sound_pitch_diff_halved(spc, x), y, spc);
    x += 36;
    dispatch_handler(D_80144010[p] == -2 ? 22 : 16);
    state_utility(sound_pitch_diff_halved(del, x), y, del);
    x += 36;
    dispatch_handler(D_80144010[p] == -1 ? 22 : 16);
    state_utility(sound_pitch_diff_halved(end, x), y, end);
}

void DrawGhost(void)
{
    s32 i;
    s32 y;
    unsigned char *str;
    unsigned char str1[128];
    unsigned char time[16];
    s16 gFreePages;

    object_create(13);
    dispatch_handler(1);
    str = countdown_state.fixed[219];
    state_utility(sound_pitch_diff_halved(str, 160), 10, str);
    object_create(11);
    str = countdown_state.fixed[220];
    state_utility(sound_pitch_diff_halved(str, 160), 55, str);
    object_create(10);
    y = object_bytes_sum_global() + 55;
    str = (*D_801461A8)->name;
    state_utility(sound_pitch_diff_halved(str, 160), y, str);
    y += object_bytes_sum_global();
    object_create(2);
    fcvt_wrapper(time, "%01d:%02d.%03d", (s32)((*D_801461A8)->time / 60.0f), (s32)(*D_801461A8)->time % 60,
                 (s32)((*D_801461A8)->time * 1000.0f) % 1000);
    state_utility(sound_pitch_diff_halved(time, 160), y, time);
    if (D_801460F0 == 1) {
        object_create(10);
        fcvt_wrapper(str1, countdown_state.fixed[223], (*D_80146198[D_801460C0])->name);
        func_800B669C(1, 3);
        camera_auto_follow(160, 110, 130, object_bytes_sum_global() * 6, -1, 0, str1);
        func_800B669C(0, 3);
    } else if (D_801460FC == 1) {
        object_create(10);
        y = object_bytes_sum_global();
        func_800B669C(1, 3);
        camera_auto_follow(160, 110, 130, y * 4, -1, 0, countdown_state.fixed[221]);
        func_800B669C(0, 3);
        y = object_bytes_sum_global() * 2 + 110;
        dispatch_handler(D_80146131 ? 22 : 1);
        str = countdown_state.strings[countdown_state.indices->h[24]];
        state_utility(sound_pitch_diff_halved(str, 145), y, str);
        dispatch_handler(D_80146131 ? 1 : 22);
        str = countdown_state.strings[countdown_state.indices->h[24] + 1];
        state_utility(sound_pitch_diff_halved(str, 175), y, str);
    } else if (D_80146148 == 1) {
        object_create(10);
        fcvt_wrapper(str1, countdown_state.fixed[222], D_8011EAD0[D_801460C0]);
        func_800B669C(1, 3);
        camera_auto_follow(160, 110, 130, object_bytes_sum_global() * 6, -1, 0, str1);
        func_800B669C(0, 3);
    } else {
        object_create(10);
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
                state_utility(sound_pitch_diff_halved(str, 160), y, str);
                y += object_bytes_sum_global();
                if (time[0]) {
                    object_create(2);
                    state_utility(sound_pitch_diff_halved(time, 160), y, time);
                    object_create(10);
                }
            } else {
                state_utility(95, y, str);
                if (i == 0) state_utility(sound_pitch_diff_calc(D_8011EAD0[D_801460C0], 225), y, D_8011EAD0[D_801460C0]);
            }
            dispatch_handler(1);
            y += object_bytes_sum_global();
        }
    }
}

void DrawScores(void)
{
    s32 i;
    s32 numWid;
    unsigned char str[16];

    object_create(13);
    dispatch_handler(1);
    state_utility(sound_pitch_diff_halved(countdown_state.strings[((HsIdx *)countdown_state.indices)->tableNames + D_801148D0], 160), 10, countdown_state.strings[((HsIdx *)countdown_state.indices)->tableNames + D_801148D0]);
    for (i = 0; i < 5; i++) {
        sprintf((char *)str, "%d", i + 1);
        object_create(5);
        dispatch_handler(D_80151AC0[D_801148D0][i] == -1 ? 1 : D_801146AC[D_80151AC0[D_801148D0][i]]);
        numWid = object_manager_update(str, -1);
        state_utility(16, 76 + i * 24, str);
        object_create(11);
        state_utility(numWid + 16, 78 + i * 24, countdown_state.strings[((HsIdx *)countdown_state.indices)->scoreNames + i]);
    }
    for (i = 0; i < 5; i++) {
        object_create(10);
        dispatch_handler(D_80151AC0[D_801148D0][i] == -1 ? 1 : D_801146AC[D_80151AC0[D_801148D0][i]]);
        music_fade(69, 80 + i * 24, 160, D_80143FE0[i]);
    }
    for (i = 0; i < 5; i++) {
        object_create(2);
        dispatch_handler(D_80151AC0[D_801148D0][i] == -1 ? 1 : D_801146AC[D_80151AC0[D_801148D0][i]]);
        state_utility(sound_pitch_diff_calc(D_80143F20[i], 304), 80 + i * 24, D_80143F20[i]);
    }
}

s32 highscore_animText(s32 arg0)
{
    s32 p;

    render_helper(0.0f);
    switch (gHighScoreState) {
    case 1:
        DrawUnlocked();
        break;
    case 2:
        DrawCircuitCode();
        break;
    case 3:
        for (p = 0; p < active_player_count; p++) {
            if (D_80143F18[p] == 1) DrawName(p);
        }
        break;
    case 4:
        DrawGhost();
        break;
    case 5:
        DrawScores();
        break;
    }
    render_helper(-1.0f);
    return 1;
}

void du_gfx_lock(void);

void du_gfx_unlock(void);
