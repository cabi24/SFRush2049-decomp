/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Controls screen text (GC controlscr.c). w30a draft + w32b levers. func_80100564 (kept) = GC animText@controlscr.c;
 * game_results_exit (internal, only caller func_80100564) = GC animButtonText@controlscr.c(signed char p).
 * w32b: func_800D8154(u8 player, s32 item) = the locked animControls.c validOption (same file in GC,
 * controlscr.c; GC's call is validOption(extsb p, i)), incl. its compiled-out `if (player) {}` and named `valid`
 * (378 -> 261 rows); void ct_SetFont wrapper (GC HAL_SetFont) at the per-player font site: frame 352 -> 360 exact
 * (253 rows). NOT A MATCH: p is coloured (s8) where retail keeps it in memory and gives s8 to the constant 10
 * (ctrace: p save 12.83 vs constant-10 web 10.53), and p+1 is PRE'd between the sprintf and the increment.
 * Devices: inlined non-static helpers ct_* (disclosed), `if (player) {}` (from the locked validOption).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct CtIdx { u8 pad0[32]; u16 rumbleOff; u16 rumbleNames; u8 pad36[2]; u16 controlNames; } CtIdx;
typedef struct TextState { void *word0; char **fixed; void *word8; CtIdx *indices; char **strings; } TextState;
typedef struct PlayerRow { u8 vehicle; u8 pak; u8 other[70]; char **name; } PlayerRow;   /* 76 B */
typedef struct CtRect { s32 f0; s32 y; s32 w; s32 h; s32 x; s32 f20; } CtRect;          /* 24 B */
typedef struct PakSlot { s8 b0, b1, b2, b3, b4, b5, b6; u8 rest[765]; } PakSlot;          /* 772 B */
typedef struct ContData { s8 b0; u8 rest[15]; } ContData;                                   /* 16 B */

extern OSMesgQueue D_801461D0;
extern TextState countdown_state;                /* gText */
extern s16 gNumWindows;                           /* gNumWindows */
extern s32 D_80154618[];                         /* buttonsInited[p] */
extern PlayerRow input_rec0[];                   /* gPlayers */
extern s8 D_8015418C[];                          /* controlScrActive[p] */
extern CtRect D_80113EE0[][4];                   /* gPlayerRects (24-byte form) */
extern ContData D_80156CF0[];                    /* gControllerData */
extern s32 D_801146AC[];                         /* playerTextColor */
extern s32 state_word_a;                         /* gGameState */
extern s16 D_8014A100[];                         /* gControlFirst[p] */
extern s16 D_8014A0F8[];                         /* gControlSelect[p] */
extern s16 D_8014A10A;                           /* gControlMenuSize */
extern PakSlot D_80144030[];
extern s32 D_80118E20[2];                        /* TB_SetJust pair */
extern char D_80120240[];                        /* "CONTROLLER %d %s" */
extern s32 osRecvMesg(OSMesgQueue *, void **, s32), osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32);
extern u32 object_manager_update(char *, s32);   /* HAL_GetTextWidth */
extern s16 object_bytes_sum_global(void);        /* TB_GetFontLineHeight */
extern void dispatch_handler(s32);               /* TB_SetTextColor */
extern void state_utility(s16, s16, char *);     /* TB_DrawText */
extern s32 fcvt_wrapper(char *, char *, ...);    /* TB_Sprintf */
extern void render_helper(f32);                  /* HAL_SetTextDepth */
extern void race_position_update(char *, char *, s16);   /* TB_EllipseText (retail narrows the width: s16) */
extern void camera_auto_follow(s16, s16, s16, s16, s32, s32, char *);  /* TB_DrawTextScrollBox */
typedef struct CtButton { u8 pad[8]; f32 angle; u8 pad12[36]; u8 pos[8]; f32 depth; u8 alpha; u8 pad61[3]; } CtButton; /* 64 B */
typedef struct CtCamera { u8 data[152]; } CtCamera;
extern CtButton D_80111998[][17];                /* buttonInfo[p][17] */
extern CtCamera gCamera[];                    /* gCamera[p] */
extern f32 D_80112A9C;                           /* gButtonScale */
extern f32 D_80112AB0;                           /* buttonZ */
extern void brake_light_update(s32, void *, void *, void *, s16 *);  /* ZOID_ProjectWindow */
extern void func_800ED66C(f32);                  /* HAL_SetTextTranslucency */
extern void func_800F6928(f32);                  /* HAL_SetMenuTextDepth */
typedef union TextColor { u8 c[4]; u32 rgba; } TextColor;   /* GC union __anon_21da */
extern TextColor D_801146BC[2];                  /* flashColors */
extern TextColor D_80118E28, D_80118E2C;
void func_800BEA3C(TextColor first, TextColor second);   /* TB_FlashTextColors (locked, frontier_audioscr) */
void controls_animButtonText(s8 p);
void music_fade(s16 x, s16 y, s16 width, void *name);   /* TB_DrawEllipseText (locked, kept) */

void ct_gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
void ct_gfx_unlock(void) { osSendMesg(&D_801461D0, 0, 0); }
s32 ct_font_set(s32 font)
{
    s32 old;
    ct_gfx_lock();
    old = slot_state_setup(font);
    ct_gfx_unlock();
    return old;
}
void ct_SetFont(s32 font)
{
    ct_font_set(font);
}
s16 ct_CenterText(char *text, s16 centerX)
{
    return centerX - (object_manager_update(text, -1) >> 1);
}
s16 ct_RightText(char *text, s16 rightX)
{
    return rightX - object_manager_update(text, -1);
}
s8 func_80094FC4(s32 pak);                       /* PS_PakRumble (locked, menu_save_options) */
s32 func_800D8154(s8 player, s32 item);          /* validOption (w36k group member) */

void ct_SetJust(s32 h, s32 v)
{
    D_80118E20[0] = h;
    D_80118E20[1] = v;
}
void controls_animButtonText(s8 p)
{
    s32 i;
    s32 x;
    s32 y;
    char *str;
    u8 probe[64];
    f32 gTextPosScale;
    s16 screenPos[2];

    gTextPosScale = D_80112A9C;
    if (gNumWindows > 1) gTextPosScale *= 0.5f;
    ct_font_set(11);
    ct_SetJust(1, 1);
    if (input_rec0[p].pak != 5 && (D_8015418C[p] == 1 || (state_word_a & 0x7C0000))) {
        brake_light_update(p, D_80111998[p][13].pos, &gCamera[p], 0, screenPos);
        x = screenPos[0];
        y = screenPos[1];
        if (gNumWindows > 1) ct_font_set(11);
        else ct_font_set(13);
        dispatch_handler(1);
        str = *input_rec0[p].name + 20;
        music_fade(x, y, gNumWindows == 1 ? 150 : 69, str);
        ct_SetJust(0, 1);
        if (!D_8015418C[p]) {
            if (state_word_a & 0x7C0000) {
                if (gNumWindows == 1) ct_font_set(10);
                else ct_font_set(11);
                dispatch_handler(1);
                ct_SetJust(1, 1);
                camera_auto_follow(D_80113EE0[gNumWindows - 1][p].x, D_80113EE0[gNumWindows - 1][p].f20,
                                   D_80113EE0[gNumWindows - 1][p].w, D_80113EE0[gNumWindows - 1][p].h,
                                   -1, 0, countdown_state.fixed[38]);
                ct_SetJust(0, 3);
            }
        } else {
            if (gNumWindows == 1) ct_font_set(11);
            else ct_font_set(12);
            for (i = 0; i < 12; i++) {
                if (D_80111998[p][i].angle > 0.69813168f && D_80111998[p][i].angle < 2.443461f) continue;
                if (!D_80111998[p][i].alpha) continue;
                func_800ED66C(D_80111998[p][i].alpha);
                func_800F6928(D_80111998[p][i].depth - 10.0f);
                brake_light_update(p, D_80111998[p][i].pos, &gCamera[p], 0, screenPos);
                if (i == 10) x = screenPos[0] - 35.0f * gTextPosScale * 300.0f / D_80112AB0;
                else if (i == 11) x = screenPos[0] - 55.0f * gTextPosScale * 300.0f / D_80112AB0;
                else x = screenPos[0] - 45.0f * gTextPosScale * 300.0f / D_80112AB0;
                y = screenPos[1];
                if (D_80111998[p][i].angle < 1.5707964f) {
                    func_800BEA3C(D_801146BC[0], D_801146BC[1]);
                    dispatch_handler(1);
                    if (!func_80094FC4(input_rec0[p].pak) && i == 10) dispatch_handler(3);
                } else {
                    func_800BEA3C(D_801146BC[0], D_801146BC[1]);
                    dispatch_handler(22);
                }
                str = countdown_state.strings[countdown_state.indices->controlNames + i];
                state_utility(x, y, str);
                if (i == 10) {
                    brake_light_update(p, D_80111998[p][12].pos, &gCamera[p], 0, screenPos);
                    x = screenPos[0] - 25.0f * gTextPosScale * 300.0f / D_80112AB0;
                    y = screenPos[1];
                    if (!func_80094FC4(input_rec0[p].pak)) str = countdown_state.strings[countdown_state.indices->rumbleOff];
                    else str = countdown_state.strings[countdown_state.indices->rumbleNames + ((s8 *)*input_rec0[p].name)[43]];
                    state_utility(x, y, str);
                }
            }
        }
    }
    ct_SetJust(0, 3);
    func_800ED66C(-1.0f);
    render_helper(0.0f);
}

s32 controls_animText(s32 arg0)
{
    s32 i;
    s32 p;
    s32 x;
    s32 y;
    char *str;
    char str1[64];
    char pName[14];
    s32 itemsDrawn;

    render_helper(0.0f);
    for (p = 0; p < gNumWindows; p++) {
        if (D_80154618[p] == 1) {
            controls_animButtonText(p);
        } else {
            ct_SetFont(11);
            if (input_rec0[p].pak != 5 && (D_8015418C[p] == 1 || (state_word_a & 0x7C0000))) {
                y = D_80113EE0[gNumWindows - 1][p].y + 10;
                do {} while (0);
                x = D_80113EE0[gNumWindows - 1][p].x;
                dispatch_handler(D_801146AC[p]);
                if (!D_80156CF0[p].b0) {
                    fcvt_wrapper(str1, D_80120240, p + 1, countdown_state.fixed[233]);
                } else {
                    race_position_update(pName, *input_rec0[p].name + 20, 69);
                    fcvt_wrapper(str1, countdown_state.fixed[37], pName);
                }
                state_utility(ct_CenterText(str1, x), y, str1);
                object_bytes_sum_global();
                if (!D_8015418C[p]) {
                    if (state_word_a & 0x7C0000) {
                        dispatch_handler(1);
                        D_80118E20[1] = 1;
                        D_80118E20[0] = 1;
                        camera_auto_follow(D_80113EE0[gNumWindows - 1][p].x, D_80113EE0[gNumWindows - 1][p].f20,
                                           D_80113EE0[gNumWindows - 1][p].w / 2, D_80113EE0[gNumWindows - 1][p].h,
                                           -1, 0, countdown_state.fixed[38]);
                        D_80118E20[1] = 3;
                        D_80118E20[0] = 0;
                    }
                } else {
                    x = D_80113EE0[gNumWindows - 1][p].x + 10;
                    y = D_80113EE0[gNumWindows - 1][p].y + 24;
                    itemsDrawn = 0;
                    for (i = 0; i < 12; i++) {
                        if (i >= D_8014A100[p] && func_800D8154(p, i)) {
                            if (!func_80094FC4(input_rec0[p].pak) && i == 10)
                                dispatch_handler(3);
                            else
                                dispatch_handler(i == D_8014A0F8[p] ? 22 : 1);
                            str = countdown_state.strings[countdown_state.indices->controlNames + i];
                            if (i == 11)
                                state_utility(ct_CenterText(str, D_80113EE0[gNumWindows - 1][p].x), y, str);
                            else
                                state_utility(ct_RightText(str, x), y, str);
                            if (i == 10) {
                                if (!func_80094FC4(input_rec0[p].pak))
                                    str = countdown_state.strings[countdown_state.indices->rumbleOff];
                                else
                                    str = countdown_state.strings[countdown_state.indices->rumbleNames + ((s8 *)*input_rec0[p].name)[43]];
                                state_utility(x + 20, y, str);
                            }
                            y += 16;
                            if (++itemsDrawn >= D_8014A10A) break;
                        }
                    }
                }
            }
        }
    }
    render_helper(-1.0f);
    return 1;
}
