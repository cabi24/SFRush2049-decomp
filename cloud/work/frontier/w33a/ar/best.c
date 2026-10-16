/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * w33a (animResults, from w32a 562/627, 358 rows, ops 41): three source levers, no devices added:
 *   (1) pane colour argument `playerIdx << 4` instead of `playerIdx * 16`: uopt no longer PRE-hoists it above the
 *       font-height call into a callee-saved web, so x is coloured (s8) again -> 324/627, 240 rows, ops 17;
 *   (2) the per-player pane half-height in its own named local `h` (declared after y) instead of reusing `min`:
 *       frame 392 -> 400 exact (retail's dead `move v1,t9` copy is h) -> 321/627;
 *   (3) the race time converted once and reused, GC-style (GC animResults converts `time` once for min and sec):
 *       `min = (u32)time; min = min / 60 % 100; sec = (u32)time % 60;` (retail's `move v0,t0` is the first
 *       assignment) -> length 627/627, `150 of 627 words differ | words 228 ops 15`, frame 400/400.
 * Remaining (ops 15): gNumLaps load order in the head loop, SetJust stores vs osRecvMesg arguments (as1 tie), the
 * two round-mode `li 1` constants hoisted to the mode-5 head in retail (t0, v1), mfhi/ctc1 order.
 * w32a/w31a header follows.
 */
/*
 * func_80105EA8 = GC animResults@hud.c (w31a fork draft, first attempt by any lane): end-of-race result panes.
 * Head: count gNumPlayersFinished (gGameCar[model[p].car].f239 == 1) and gAPlayerFinished (model[p].lap == gNumLaps)
 * over gNumWindows; TB_SetJust(1,1); HAL_SetFont(10); multi-window "all finished" header pane (index 4 of the pane
 * array D_80154350, created on demand with ambient_sound_set = pane create), winner / tie / ranked-player text
 * (fixed[200]/[201]/[199]) drawn with camera_auto_follow = TB_DrawTextScrollBox; then per finished player a pane at
 * wrongWayPos D_80115F28[gNumWindows-1][p] (+12 y for players 2,3 in 4-window), text by gRace (gameplay_mode):
 * 4 stunt "%s\n%d %s", 5 lap/time "%s\n%s" or "%s\n%s %d:%02d.%03d", 6 "%s\n%d %s" (gGameCar[p].f931),
 * default place + time "%s\n%s\n%s"; TB_SetJust(0,3) at the end.
 * GC-derived names: model D_8014A250 (2056 B: car +1990, f1996, lap +2024), gGameCar player_array (952 B: place +238,
 * f239 finished, time f32 +240), gNumLaps D_80152734, gDeathMode D_80142760, gRankedPlayer D_80143F54, ties
 * D_80150B60, gLap D_80144018[p], gStuntPlayer D_8015204C (stride 120), wrongWayPos D_80115F28, gPlayers input_rec0.
 * Shaping devices (disclosed): inlined non-static helpers ar_font_set / ar_SetFont (void wrapper, +8 B) / ar_SetJust
 * (+16 B; frame 368 -> 400 exact); the header pane's half height is held in `y` (variable reuse; y is dead there) and
 * the per-player pane height in `min` (reuse); GC block-scoped place/min/sec/thou in the default case.
 * Unit (tag w31ae, --internal ar_font_set --internal ar_SetJust --internal ar_SetFont):
 *   FAIL func_80105EA8: 562 of 627 words differ | words 358 ops 41 norm 41 | frame 400/400
 * Residual: callee-saved colouring -- x is split (not coloured, lives at its home 392) because ours colours
 * playerIdx*16 (computed before the font-height call, s3) while retail computes it after the call in t6; every
 * s-register is then one off. Plus as1 order of the first SetJust stores vs osRecvMesg arguments.
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

s32 ar_font_set(s32 font)
{
    s32 old;
    osRecvMesg(&D_801461D0, 0, 1);
    old = slot_state_setup(font);
    osSendMesg(&D_801461D0, 0, 0);
    return old;
}

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
    s32 h;
    char str[128];
    s32 numFinished;
    s32 aFinished;
    u32 min;
    u32 sec;
    u32 thou;
    char str1[32];
    char str2[32];

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
            y = object_bytes_sum_global() / 2;
            D_80154350[4] = ambient_sound_set(91, 106 - y, 229, y + 114, 192, 0, 0, 1);
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
            h = object_bytes_sum_global() * 3 / 2;
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
                min = (u32)player_array[input_rec0[playerIdx].vehicle].time;
                min = min / 60 % 100;
                sec = (u32)player_array[input_rec0[playerIdx].vehicle].time % 60;
                thou = (u32)(player_array[input_rec0[playerIdx].vehicle].time * 1000.0f) % 1000;
                fcvt_wrapper(str, "%s\n%s %d:%02d.%03d", *input_rec0[playerIdx].name + 20, countdown_state.fixed[208],
                             min, sec, thou);
            }
        } else if (gameplay_mode == 6) {
            fcvt_wrapper(str, "%s\n%d %s", *input_rec0[playerIdx].name + 20, player_array[playerIdx].f931,
                         countdown_state.fixed[130]);
        } else {
            s32 place;
            u32 min;
            u32 sec;
            u32 thou;

            place = player_array[input_rec0[playerIdx].vehicle].place;
            fcvt_wrapper(str1, countdown_state.fixed[198], place + 1,
                         countdown_state.strings[countdown_state.indices->h[55] + place]);
            if (D_80152734 != D_8014A250[input_rec0[playerIdx].vehicle].lap) {
                fcvt_wrapper(str2, countdown_state.fixed[207]);
            } else {
                min = (u32)player_array[input_rec0[playerIdx].vehicle].time;
                min = min / 60 % 100;
                sec = (u32)player_array[input_rec0[playerIdx].vehicle].time % 60;
                thou = (u32)(player_array[input_rec0[playerIdx].vehicle].time * 1000.0f) % 1000;
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
