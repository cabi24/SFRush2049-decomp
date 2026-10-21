/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * animResults (0x80105EA8, 2508 B) = GC hud.c animResults (MAT3 decomp/src/GameSource/src/hud.c animResults,
 * GC locals playerIdx, x, y, str[128], gNumPlayersFinished, gAPlayerFinished; block locals min/sec/thou and
 * str1[32]/str2[32]/place as in GC). End-of-race result panes and texts. The N64 build creates the panes here
 * (GC moved them to CheckResults): header pane resultPane[4] (D_80154350[4]) and one pane per finished player
 * (InitPane = ambient_sound_set, ChangePaneColor = tournament_unlock_check with playerPaneColors D_80116148),
 * each sized from TB_GetFontLineHeight (object_bytes_sum_global) in a block-scoped `h` (GC CheckResults style).
 * GC-derived names (w31a/w32a/w33a): model D_8014A250 (2056 B), gGameCar player_array (952 B), gNumLaps
 * D_80152734, gDeathMode D_80142760, gRankedPlayer D_80143F54, ties D_80150B60, gLap D_80144018, gStuntPlayer
 * D_8015204C, wrongWayPos D_80115F28, gPlayers input_rec0, gText countdown_state, TB_Sprintf fcvt_wrapper,
 * TB_DrawTextScrollBox camera_auto_follow, HAL_SetTextDepth render_helper, TB_SetTextColor dispatch_handler,
 * HAL_SetFont = slot_state_setup (s2 IPA parameter).
 * History: w31a draft 562/627 -> w33a 150/627 (playerIdx << 4, h, time converted once) -> w36m EQUAL.
 * w36m levers (fresh MAT3 typed port of the tail):
 *   - ar_font_set DEFINED AFTER animResults (prototype first): as1's .loc tie puts the TB_SetJust stores before
 *     the osRecvMesg arguments (w29b func_80106B3C lever) -> ops 15 -> 9;
 *   - GC block scoping: the two pane heights are block locals `h` (header block, player block), str1/str2/place
 *     and the else-block min/sec/thou are default-case block locals (retail str at T-140, str1 200, str2 168);
 *   - time blocks: `min = (u32)t; min = min / 60 % 100; sec = (u32)t % 60; thou = (u32)(1000.0f * t);
 *     thou %= 1000;` (the conversion CSE copy is retail's `move v0,t0`; GC's separate `thou %= 1000` statement
 *     gives the round-mode constant and mfhi/ctc1 order);
 *   - GC's two extra varargs on the laps-text TB_Sprintf (`model[..].laps, gNumLaps`, the values the compare just
 *     loaded: retail keeps them in a2/a3) -> 65 rows -> EQUAL.
 * Shaping (disclosed, from w31a/w33a): inlined non-static helpers ar_font_set / ar_SetFont (void wrapper) /
 * ar_SetJust, not on keep; `playerIdx << 4` for the pane colour argument.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct ArIdx { u16 h[64]; } ArIdx;
typedef struct TextState { void *word0; char **fixed; void *word8; ArIdx *indices; char **strings; } TextState;
typedef struct Model { u8 pad0[1990]; s16 car; u8 pad1992[4]; s8 f1996; u8 pad1997[27]; s8 lap; u8 pad2025[31]; } Model; /* 2056 B */
typedef struct GameCar { u8 pad0[238]; s8 place; s8 f239; f32 time; u8 pad244[687]; s8 f931; u8 pad932[20]; } GameCar; /* 952 B */
typedef struct PlayerRow { u8 vehicle; u8 pak; u8 other[70]; char **name; } PlayerRow;   /* 76 B */
typedef struct Pos { s32 x, y; } Pos;
typedef struct StuntPlayer { s32 score; u8 pad[116]; } StuntPlayer;                      /* 120 B */

extern OSMesgQueue D_801461D0;
extern TextState countdown_state;                /* gText */
extern s16 gNumWindows;
extern s16 D_80152734;                           /* gNumLaps */
extern Model D_8014A250[];                       /* model */
extern GameCar player_array[];                   /* gGameCar */
extern PlayerRow input_rec0[];                   /* gPlayers */
extern Pos D_80115F28[][4];                      /* wrongWayPos */
extern s32 gameplay_mode;                        /* gRace */
extern s8 D_80142760;                            /* gDeathMode */
extern s8 D_80143F54;                            /* gRankedPlayer */
extern s32 D_80150B60;                           /* ties */
extern u8 D_80144018[];                          /* gLap[p] */
extern StuntPlayer D_8015204C[];                 /* gStuntPlayer[p] (+0x14 in GC) */
extern s32 *D_80154350[];                        /* result panes [p], [4] = the header pane */
extern Pos D_80116148[];
extern s32 D_80118E20[2];                        /* TB_SetJust pair */
extern s32 osRecvMesg(OSMesgQueue *, void **, s32), osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32);
extern s16 object_bytes_sum_global(void);        /* TB_GetFontLineHeight */
extern void dispatch_handler(s32);               /* TB_SetTextColor */
extern s32 fcvt_wrapper(char *, char *, ...);    /* TB_Sprintf */
extern void render_helper(f32);                  /* HAL_SetTextDepth */
extern s32 *ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);   /* CreatePane-like */
extern void camera_auto_follow(s16, s16, s16, s16, s16, s16, char *);   /* TB_DrawTextScrollBox */
extern void tournament_unlock_check(void *, void *, void *);

s32 ar_font_set(s32 font);

void ar_SetJust(s32 h, s32 v)
{
    D_80118E20[0] = h;
    D_80118E20[1] = v;
}

void ar_SetFont(s32 font)
{
    ar_font_set(font);
}

s32 animResults(s32 arg0)
{
    s32 playerIdx;
    s32 x;
    s32 y;
    char str[128];
    s32 numFinished;
    s32 aFinished;

    numFinished = 0;
    aFinished = 0;
    for (playerIdx = 0; playerIdx < gNumWindows; playerIdx++) {
        if (D_80152734 == D_8014A250[playerIdx].lap) aFinished = 1;
        if (player_array[D_8014A250[playerIdx].car].f239 == 1) numFinished++;
    }
    render_helper(0.0f);
    ar_SetJust(1, 1);
    ar_SetFont(10);
    if (gNumWindows >= 2 && numFinished == gNumWindows && (!D_80142760 || (D_80142760 && aFinished))) {
        if (D_80154350[4] == 0) {
            s32 h = object_bytes_sum_global() / 2;
            D_80154350[4] = ambient_sound_set(91, 106 - h, 229, h + 114, 192, 0, 0, 1);
        }
        dispatch_handler(22);
        if (D_8014A250[D_8014A250[D_80143F54].car].f1996 == 1) {
            fcvt_wrapper(str, countdown_state.fixed[200]);
        } else if (D_80150B60) {
            fcvt_wrapper(str, countdown_state.fixed[201], D_80150B60 + 1);
        } else {
            fcvt_wrapper(str, countdown_state.fixed[199], *input_rec0[D_80143F54].name + 20);
        }
        camera_auto_follow(160, 110, 160, 110, -1, 0, str);
    }
    dispatch_handler(1);
    for (playerIdx = 0; playerIdx < gNumWindows; playerIdx++) {
        if (D_80142760 && D_80152734 != D_8014A250[playerIdx].lap) continue;
        if (player_array[D_8014A250[playerIdx].car].f239 != 1) continue;
        x = D_80115F28[gNumWindows - 1][playerIdx].x;
        y = D_80115F28[gNumWindows - 1][playerIdx].y;
        if (gNumWindows == 4 && playerIdx >= 2) y += 12;
        if (D_80154350[playerIdx] == 0) {
            s32 h = object_bytes_sum_global() * 3 / 2;
            D_80154350[playerIdx] = ambient_sound_set(x - 69, y - h - 4, x + 69, h + y + 4, 192, 0, playerIdx << 4, 1);
            tournament_unlock_check(D_80154350[playerIdx], &D_80116148[playerIdx].x, &D_80116148[playerIdx].y);
        }
        if (gameplay_mode == 4) {
            fcvt_wrapper(str, "%s\n%d %s", *input_rec0[playerIdx].name + 20, D_8015204C[playerIdx].score,
                         countdown_state.fixed[130]);
        } else if (gameplay_mode == 5) {
            if (D_80144018[playerIdx] != 1) {
                fcvt_wrapper(str, "%s\n%s", *input_rec0[playerIdx].name + 20, countdown_state.fixed[214]);
            } else {
                u32 min;
                u32 sec;
                u32 thou;

                min = (u32)player_array[input_rec0[playerIdx].vehicle].time;
                min = min / 60 % 100;
                sec = (u32)player_array[input_rec0[playerIdx].vehicle].time % 60;
                thou = (u32)(1000.0f * player_array[input_rec0[playerIdx].vehicle].time);
                thou %= 1000;
                fcvt_wrapper(str, "%s\n%s %d:%02d.%03d", *input_rec0[playerIdx].name + 20, countdown_state.fixed[208],
                             min, sec, thou);
            }
        } else if (gameplay_mode == 6) {
            fcvt_wrapper(str, "%s\n%d %s", *input_rec0[playerIdx].name + 20, player_array[playerIdx].f931,
                         countdown_state.fixed[130]);
        } else {
            char str1[32], str2[32];
            s32 place = player_array[input_rec0[playerIdx].vehicle].place;

            fcvt_wrapper(str1, countdown_state.fixed[198], place + 1,
                         countdown_state.strings[countdown_state.indices->h[55] + place]);
            if (D_80152734 != D_8014A250[input_rec0[playerIdx].vehicle].lap) {
                fcvt_wrapper(str2, countdown_state.fixed[207], D_8014A250[input_rec0[playerIdx].vehicle].lap, D_80152734);
            } else {
                u32 min;
                u32 sec;
                u32 thou;

                min = (u32)player_array[input_rec0[playerIdx].vehicle].time;
                min = min / 60 % 100;
                sec = (u32)player_array[input_rec0[playerIdx].vehicle].time % 60;
                thou = (u32)(1000.0f * player_array[input_rec0[playerIdx].vehicle].time);
                thou %= 1000;
                fcvt_wrapper(str2, "%s %d:%02d.%03d", countdown_state.fixed[208], min, sec, thou);
            }
            fcvt_wrapper(str, "%s\n%s\n%s", *input_rec0[playerIdx].name + 20, str1, str2);
        }
        camera_auto_follow(x, y, 160, 110, -1, 0, str);
    }
    render_helper(-1.0f);
    ar_SetJust(0, 3);
    return 1;
}

s32 ar_font_set(s32 font)
{
    s32 old;
    osRecvMesg(&D_801461D0, 0, 1);
    old = slot_state_setup(font);
    osSendMesg(&D_801461D0, 0, 0);
    return old;
}

