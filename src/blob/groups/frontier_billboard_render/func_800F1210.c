/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800F1210 = GC `_SetHighScoreState(newState)` (highscorescr.c, confident pair, w28g). newState in $s2
 * (IPA register parameter). Old state's panes are removed (RemovePane = sound_handles_array_clear), the new
 * state's panes are built (InitPane = ambient_sound_set) and the countdown timer set (SetCountdownTimer =
 * viScheduleTick). GC names: gHighScoreState D_80140B20, blit D_801148CC (RemoveBlit = sound_stop,
 * NewMultiBlit = sound_control, mblit D_801148A8), namePanes D_80149418[], codePane D_80146208, ghost panes
 * D_80149798[3], score panes D_80149778[2], gNumPlayers active_player_count, gNumWindows D_80151AD0,
 * gPlayerRects D_801148E8, gRace D_8014A110, gTable D_801148D0, gGhostMenuSelect D_80144DA0,
 * gGhostPak D_801460C0, _UninitScreen func_800F1114, _FindGhosts func_800F0F44, _PrintScores func_800F0914,
 * HAL_SetFont = object_create (inlined font_set), HAL_GetTextWidth = object_manager_update,
 * TB_GetFontLineHeight = object_bytes_sum_global.
 * Shaping notes (w28a): the second switch reads the global back (`switch (D_80140B20)` after the store),
 * which is what makes uopt reload it per case for the final test; the font switches go through the
 * non-static internal helpers hs_font_set/hs_gfx_lock/hs_gfx_unlock (umerge inlines the split form, not a
 * one-piece wrapper with 4 sites); one function-level `p` for the player loops, block-scoped `i`/`pak`.
 * No unused locals, no compiled-out reads.
 */
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef struct Rect { s16 x,y,w,h; s16 unknown[2]; } Rect;
typedef struct TextInfo { char unknown[54]; u16 index; } TextInfo;
typedef struct TextState { s32 unknown[3]; TextInfo *info; char **strings; } TextState;
extern TextState countdown_state;
extern s32 D_80140B20, D_801148CC, D_8014A110, D_80144DA0;
extern s16 active_player_count, D_80151AD0;
extern s8 *D_80149418[], *D_80146208, *D_80149798[3], *D_80149778[2];
extern s8 D_801460C0, D_801148D0;
extern Rect D_801148E8[][4];
extern char D_80120600[], D_801148A8[];
extern void sound_stop(s32);
extern void sound_handles_array_clear(s8 *);
extern void func_800F1114(void);
extern void *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
extern void viScheduleTick(f32);
typedef struct { s32 opaque[6]; } OSMesgQueue;
extern OSMesgQueue D_801461D0;
extern s32 osRecvMesg(OSMesgQueue *, void **, s32), osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32);
void hs_gfx_lock(void) { osRecvMesg(&D_801461D0, (void *)0, 1); }
void hs_gfx_unlock(void) { osSendMesg(&D_801461D0, (void *)0, 0); }
s32 hs_font_set(s32 font)
{
    s32 old;
    hs_gfx_lock();
    old = slot_state_setup(font);
    hs_gfx_unlock();
    return old;
}
extern s32 object_manager_update(char *,s32);
extern s16 object_bytes_sum_global(void);
extern void func_800F0F44(s32), func_800F0914(void);
extern s32 sound_control(s32,s32,void *,s32);

void func_800F1210(s32 newState)
{
    s32 p;
    s32 width;

    if (newState == D_80140B20) return;
    if (D_801148CC != 0) {
        sound_stop(D_801148CC);
        D_801148CC = 0;
    }
    switch (D_80140B20) {
    case 1: {
        for (p = 0; p < active_player_count; p++) {
            if (D_80149418[p] != 0) {
                sound_handles_array_clear(D_80149418[p]);
                D_80149418[p] = 0;
            }
        }
        break;
    }
    case 2:
        if (D_80146208 != 0) {
            sound_handles_array_clear(D_80146208);
            D_80146208 = 0;
        }
        break;
    case 3: {
        for (p = 0; p < active_player_count; p++) {
            if (D_80149418[p] != 0) {
                sound_handles_array_clear(D_80149418[p]);
                D_80149418[p] = 0;
            }
        }
        break;
    }
    case 4: {
        s32 i;
        for (i = 0; i < 3; i++) {
            if (D_80149798[i] != 0) {
                sound_handles_array_clear(D_80149798[i]);
                D_80149798[i] = 0;
            }
        }
        break;
    }
    case 5: {
        s32 i;
        for (i = 0; i < 2; i++) {
            if (D_80149778[i] != 0) {
                sound_handles_array_clear(D_80149778[i]);
                D_80149778[i] = 0;
            }
        }
        break;
    }
    }
    D_80140B20 = newState;
    switch (D_80140B20) {
    case 0:
        func_800F1114();
        break;
    case 1: {
        s32 p;
        for (p = 0; p < active_player_count; p++) {
            if (D_80149418[p] == 0) {
                D_80149418[p] = ambient_sound_set(D_801148E8[D_80151AD0 - 1][p].x,
                    D_801148E8[D_80151AD0 - 1][p].y,
                    D_801148E8[D_80151AD0 - 1][p].x + D_801148E8[D_80151AD0 - 1][p].w,
                    D_801148E8[D_80151AD0 - 1][p].y + D_801148E8[D_80151AD0 - 1][p].h, 176, 0, 0, 0);
            }
        }
        viScheduleTick(600.0f);
        break;
    }
    case 2:
        D_80146208 = ambient_sound_set(83, 70, 237, 136, 192, 0, 0, 0);
        viScheduleTick(600.0f);
        break;
    case 3: {
        s32 p;
        for (p = 0; p < active_player_count; p++) {
            if (D_80149418[p] == 0) {
                D_80149418[p] = ambient_sound_set(D_801148E8[D_80151AD0 - 1][p].x,
                    D_801148E8[D_80151AD0 - 1][p].y,
                    D_801148E8[D_80151AD0 - 1][p].x + D_801148E8[D_80151AD0 - 1][p].w,
                    D_801148E8[D_80151AD0 - 1][p].y + D_801148E8[D_80151AD0 - 1][p].h, 176, 0, 0, 0);
            }
        }
        viScheduleTick(600.0f);
        break;
    }
    case 4: {
        s32 pak;
        if (D_80149798[0] == 0) {
            hs_font_set(13);
            width = object_manager_update(D_80120600, -1);
            D_80149798[0] = ambient_sound_set(160 - width / 2 - 8, 6, width / 2 + 168, object_bytes_sum_global() + 14, 176, 0, 0, 0);
        }
        if (D_80149798[1] == 0) {
            hs_font_set(10);
            D_80149798[1] = ambient_sound_set(91, 51, 229, object_bytes_sum_global() * 3 + 59, 176, 0, 0, 0);
        }
        if (D_80149798[2] == 0) {
            hs_font_set(10);
            D_80149798[2] = ambient_sound_set(91, 106, 229, object_bytes_sum_global() * 5 + 114, 176, 0, 0, 0);
        }
        viScheduleTick(600.0f);
        D_80144DA0 = 0;
        D_801460C0 = 0;
        for (pak = 0; pak < 4; pak++) {
            func_800F0F44(pak);
        }
        break;
    }
    case 5:
        D_801148D0 = (D_8014A110 == 2) ? 2 : 0;
        func_800F0914();
        if (D_80149778[0] == 0) {
            hs_font_set(13);
            width = object_manager_update(countdown_state.strings[countdown_state.info->index + D_801148D0], -1);
            D_80149778[0] = ambient_sound_set(160 - width / 2 - 8, 6, width / 2 + 168, object_bytes_sum_global() + 14, 176, 0, 0, 0);
        }
        if (D_80149778[1] == 0) {
            D_80149778[1] = ambient_sound_set(8, 75, 312, 200, 176, 0, 0, 0);
        }
        if (D_8014A110 == 2) {
            viScheduleTick(30.0f);
        } else {
            viScheduleTick(15.0f);
        }
        break;
    }
    if (D_80140B20 != 0) {
        D_801148CC = sound_control(0, 0, D_801148A8, 1);
    }
}
