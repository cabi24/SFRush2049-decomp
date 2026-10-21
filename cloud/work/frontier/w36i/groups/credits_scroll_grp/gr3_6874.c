/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80106874 (0x80106874, 712 B): per-window background pane behind the HUD title text that the neighbour
 * func_80106B3C draws (StrTable +0x388, font 11, or font 12 when wider than the window limit + 8). N64-only
 * hud.c Blit AnimFunc (GC hud.c has nothing between animResults and animOdometer = func_80106D94); written in the
 * GC CheckResults pane idiom (MAT3 hud.c CheckResults: InitPane(..., 192, 0, 0, 1) + ChangePaneColor(pane,
 * playerPaneColors[p][0], playerPaneColors[p][1]) + RemovePane). InitPane = ambient_sound_set, ChangePaneColor =
 * tournament_unlock_check, RemovePane = sound_handles_array_clear, HAL_GetStringWidth = object_manager_update,
 * HAL_GetFontHeight = object_bytes_sum_global, playerPaneColors = D_80116198 (u8 [4][2][4]), the pane handles =
 * D_80154350[4], the window positions = D_80115FA8 (shared with func_80106B3C), gGameState = D_801174B4 (bit 21
 * shows the panes, bit 22 retires the blit), gPauseState = D_801170FC, D_8014A118[i].control /
 * D_80144030[ctl].b6 as in func_80106B3C. HAL_SetFont = slot_state_setup (s2 IPA parameter).
 * Lever (from the w29b func_80106B3C match): the b6 test through an inlined getter (its value return takes v0).
 * Helpers gfx_lock/gfx_unlock/font_set/getB6/Hidden_6874 are inlined, non-static, not on keep (w17j stub rule).
 * No other shaping. w36m (wave 36).
 */typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct Blit {
    u8 pad0[0x1A];
    s8 hidden;
    u8 pad1B[0x28 - 0x1B];
    s32 state;
} Blit;

typedef struct TblHdr {
    u8 pad0[0x388];
    char *text;
} TblHdr;

typedef struct Player {
    u8 b0;
    u8 control;
    u8 pad2[74];
} Player;

typedef struct Control {
    u8 pad0[6];
    s8 b6;
    u8 pad7[772 - 7];
} Control;

typedef struct Pos {
    s32 x, y;
} Pos;

extern OSMesgQueue D_801461D0;
extern u32 D_801174B4;
extern TblHdr *D_8017A4E4;
extern s16 gNumWindows;
extern s32 D_801170FC;
extern Player D_8014A118[];
extern Control D_80144030[];
extern Pos D_80115FA8[][4];
extern u8 D_80116198[4][2][4];
extern void *D_80154350[4];

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void Input_ApplyPadConfig(void *);
s32 object_manager_update(void *, s32);
s16 object_bytes_sum_global(void);
void *ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);
void tournament_unlock_check(void *, u8 *, u8 *);
void sound_handles_array_clear(void *);

void gfx_lock_6874(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_6874(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_6874(s32 font)
{
    s32 old;

    gfx_lock_6874();
    old = slot_state_setup(font);
    gfx_unlock_6874();
    return old;
}

s32 getB6_6874(s32 ctl)
{
    return D_80144030[ctl].b6;
}

s32 Hidden_6874(Blit *blt, s32 hide)
{
    if (blt->hidden != hide) {
        blt->hidden = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->hidden;
}

s32 func_80106874(Blit *blt)
{
    s32 w;
    s32 h;
    s32 limit;
    s32 i;
    s32 x;
    s32 y;

    font_set_6874(11);
    w = object_manager_update(D_8017A4E4->text, -1);
    limit = (gNumWindows >= 3) ? 160 : ((gNumWindows >= 2) ? 240 : 320);
    if (limit + 8 < w) {
        font_set_6874(12);
        w = object_manager_update(D_8017A4E4->text, -1);
    }
    h = object_bytes_sum_global();
    for (i = 0; i < gNumWindows; i++) {
        if ((D_801174B4 & 0x200000) && !D_801170FC && !getB6_6874(D_8014A118[i].control)) {
            if (!D_80154350[i]) {
                x = D_80115FA8[gNumWindows - 1][i].x;
                y = D_80115FA8[gNumWindows - 1][i].y;
                D_80154350[i] = ambient_sound_set(x - w / 2 - 8, y - h / 2 - 4, x + w / 2 + 8, y + h / 2 + 4,
                                                  255, 0, i, 1);
                tournament_unlock_check(D_80154350[i], D_80116198[i][0], D_80116198[i][1]);
            }
        } else if (D_80154350[i]) {
            sound_handles_array_clear(D_80154350[i]);
            D_80154350[i] = 0;
        }
    }
    if (D_801174B4 & 0x400000) {
        blt->state = 0;
        Hidden_6874(blt, 1);
        return 0;
    }
    return 1;
}
