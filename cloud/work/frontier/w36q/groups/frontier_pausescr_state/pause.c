/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800B4FB0 (0x800B4FB0, 1472 B) = GC setPauseState@pausescr.c (MAT3 pausescr.c, GC 0x80078F08) with GC
 * updatePauseStateCreate (GC 0x800788A4) inlined; updatePauseStateCleanup = func_800B4E68 (jal), _TitlePane = voice_stop.
 * w36q (wave 36): first typed port; levers: GC `width /= 2` / `max /= 2` are halvings inside the InitPane arguments
 * on the N64 (retail divides after the TB_GetFontLineHeight() argument call, and the rumble-pane loop's half width is
 * hoisted into the loop preheader), and the rumble-pane loop counter is `i` (GC reuses `max`: one variable = one web
 * gives max and the loop counter one register; retail has s2 / s1). Helpers ps_gfx_lock/ps_gfx_unlock/ps_font_set/
 * ps_SetFont (GC HAL_SetFont) and ps_updatePauseStateCreate are non-static and not on keep (w17j stub rule).
 * No unused locals, no block breaks.
 * N64 differences: the pause menu has 8 entries (text table 56, entries +1..+8, no GC i>3 skip) and its pane is
 * 8 lines tall; string indices single[226/171/170/216] (GC 238/181/180/227).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct OSMesgQueue OSMesgQueue;
typedef struct Blit Blit;
typedef struct MultiBlit MultiBlit;
typedef struct TextState { void *word0; char **single; void *word8; u16 *tab; char **taba; } TextState;
typedef struct { s16 x, y; } Pos;
typedef struct { s32 x, y, w, h, cx, cy; } Rect;   /* 0x18 */

extern OSMesgQueue D_801461D0;
extern TextState countdown_state;          /* gText */
extern s32 D_801170FC;                     /* gPauseState */
extern Blit *D_80117338;                   /* DimBlit */
extern Blit *D_8011733C;                   /* TitleBlit */
extern Blit *D_80117340;                   /* PauseBlit */
extern Blit *D_80117344;                   /* SureBlit */
extern Blit *D_80117348;                   /* RumbleBlit */
extern const MultiBlit D_80117284, D_801172A8, D_801172CC, D_801172F0, D_80117314;
            /* DimMultiBlit, TitleMultiBlit, PauseMultiBlit, SureMultiBlit, RumbleMultiBlit */
extern Pos D_80117280;                     /* titlePos */
extern s8 D_80117354;                      /* gYes */
extern Rect D_80117100[][4];               /* gPlayerRects */
extern s16 D_8014A108;                     /* gNumPlayers */

s32 osRecvMesg(OSMesgQueue *, void *, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32);
u32 object_manager_update(void *, s16);    /* HAL_GetTextWidth */
s16 object_bytes_sum_global(void);         /* TB_GetFontLineHeight */
void *ambient_sound_set(s32, s32, s32, s32, s32, void *, s32, s32);   /* InitPane */
Blit *sound_control(s16, s16, const MultiBlit *, s16);   /* NewMultiBlit */
void func_800B4E68(void);                  /* updatePauseStateCleanup */
void voice_stop(void);                     /* _TitlePane@pausescr.c */
s32 func_80094FE8();                       /* animRumblePane */

void ps_gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
void ps_gfx_unlock(void) { osSendMesg(&D_801461D0, 0, 0); }
s32 ps_font_set(s32 font)
{
    s32 old;
    ps_gfx_lock();
    old = slot_state_setup(font);
    ps_gfx_unlock();
    return old;
}
void ps_SetFont(s32 font)
{
    ps_font_set(font);
}

void ps_updatePauseStateCreate(void)
{
    s32 max;
    s32 width;
    s32 i;
    s32 x;
    s32 y;

    switch (D_801170FC) {
    case 1:
        if (!D_80117338) D_80117338 = sound_control(0, 0, &D_80117284, 1);
        voice_stop();
        ps_SetFont(13);
        x = D_80117280.x;
        y = D_80117280.y + 2 * object_bytes_sum_global();
        ps_SetFont(10);
        max = 0;
        for (i = 0; i < 8; i++) {
            width = object_manager_update(countdown_state.taba[i + 1 + countdown_state.tab[56]], -1);
            if (max < width) max = width;
        }
        ambient_sound_set(x - max / 2 - 8, y - 4, x + max / 2 + 8, y + 8 * object_bytes_sum_global() + 4, 176, 0, 0, 0);
        D_8011733C = sound_control(0, 0, &D_801172A8, 1);
        D_80117340 = sound_control(0, 0, &D_801172CC, 1);
        break;
    case 6:
        D_80117354 = 0;
        voice_stop();
        ps_SetFont(10);
        x = D_80117100[0][1].cx;
        y = D_80117100[0][1].cy - object_bytes_sum_global() / 2;
        width = object_manager_update(countdown_state.single[226], -1);
        ambient_sound_set(x - width / 2 - 8, y - 4, x + width / 2 + 8, y + object_bytes_sum_global() + 4, 176, func_80094FE8, 0, 0);
        ps_SetFont(11);
        width = object_manager_update(countdown_state.single[171], -1);
        width = (u32)width > object_manager_update(countdown_state.single[170], -1) ? width : object_manager_update(countdown_state.single[170], -1);
        for (i = 0; i < D_8014A108; i++) {
            x = D_80117100[D_8014A108 - 1][i].cx;
            y = D_80117100[D_8014A108 - 1][i].cy - object_bytes_sum_global();
            ambient_sound_set(x - width / 2 - 8, y - 4, x + width / 2 + 8, y + 2 * object_bytes_sum_global() + 4, 176, 0, 0, 0);
        }
        D_8011733C = sound_control(0, 0, &D_801172A8, 1);
        D_80117348 = sound_control(0, 0, &D_80117314, 1);
        break;
    case 7:
    case 8:
        D_80117354 = 0;
        voice_stop();
        ps_SetFont(13);
        x = D_80117280.x;
        y = D_80117280.y + 2 * object_bytes_sum_global();
        ps_SetFont(10);
        width = object_manager_update(countdown_state.single[216], -1);
        ambient_sound_set(x - width / 2 - 8, y - 4, x + width / 2 + 8, y + 2 * object_bytes_sum_global() + 4, 176, 0, 0, 0);
        D_8011733C = sound_control(0, 0, &D_801172A8, 1);
        D_80117344 = sound_control(0, 0, &D_801172F0, 1);
        break;
    }
}

void func_800B4FB0(s32 state)
{
    D_801170FC = state;
    func_800B4E68();
    ps_updatePauseStateCreate();
}
