/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Controls screen text (GC controlscr.c). controls_animText (kept) = GC animText@controlscr.c; controls_animButtonText
 * (internal, only caller controls_animText) = GC animButtonText@controlscr.c(signed char p). NOT A MATCH.
 * w30a draft + w32b levers (ct_validOption = locked animControls.c validOption incl. its `if (player) {}`;
 * void ct_SetFont wrapper at animText's per-player font site) + w33b:
 *  - controls_animText 253 -> 155 rows, frame 360 exact: the sprintf argument is written `(p * 1) + 1`
 *    (DISCLOSED no-code identity device). Plain `p + 1` (and `1 + p`, `p - -1`, `p + 1U`, a ct_next() helper) is the
 *    same uopt expression as the loop increment, so PRE inserts p+1 on every `continue` edge (bits INSERT 5,11,14,77),
 *    p's web ends early, p is coloured (s8) and the constant 10 loses s8. With the identity p stays in memory (352)
 *    and the tail recomputes p+1 as retail. `(p << 0) + 1` is equivalent.
 *  - GC TB_SetJust as an inlined non-static helper ct_SetJust(h, v) at the two animText sites (the 8 B the PRE temp
 *    slot used to fill: +8 B per inlined site) and at three of the five animButtonText sites (first, second, end):
 *    animButtonText frame 520 -> 544 exact, 234 -> 227 rows; the animText font site is ct_font_set again (frame
 *    exact with the two SetJust sites instead of the SetFont wrapper).
 *  Residual animText: &D_8014A0F8 index p*2 (save 120/11 = 10.91) takes s8 over the constant 10 (200/19 = 10.53);
 *  continue edges get the gNumWindows load PRE-inserted (retail loads it in the tail). animButtonText: the end
 *  SetJust stores are duplicated onto each `goto end` edge; retail keeps one shared end with &D_80118E20 in t0.
 * Devices: inlined non-static helpers ct_* (disclosed), `if (player) {}` (from the locked validOption),
 * `(p * 1) + 1`.
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
void func_800BEA3C(TextColor first, TextColor second)   /* TB_FlashTextColors */
{
    D_80118E28.rgba = first.rgba;
    D_80118E2C.rgba = second.rgba;
}
void controls_animButtonText(s8 p);
void ct_SetJust(s32 h, s32 v)
{
    D_80118E20[1] = v;
    D_80118E20[0] = h;
}


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
s32 ct_PakRumble(s32 pak)
{
    return D_80144030[pak].b6;
}
s32 ct_validOption(u8 player, s32 item)
{
    s32 valid;

    valid = 1;
    if (player) {}
    if (item == 8) {
        valid = 0;
    }
    return valid;
}

void controls_animButtonText(s8 p)
{
    s32 i;
    s32 x;
    s32 y;
    char *str;
    s16 screenPos[2];
    f32 gTextPosScale;
    char label[224];

    gTextPosScale = D_80112A9C;
    if (gNumWindows >= 2) gTextPosScale *= 0.5f;
    ct_font_set(11);
    ct_SetJust(1, 1);
    if (input_rec0[p].pak == 5) goto end;
    if (D_8015418C[p] != 1 && !(state_word_a & 0x7C0000)) goto end;
    brake_light_update(p, D_80111998[p][13].pos, &gCamera[p], 0, screenPos);
    x = screenPos[0];
    y = screenPos[1];
    if (gNumWindows >= 2) ct_font_set(11);
    else ct_font_set(13);
    dispatch_handler(1);
    race_position_update(label, *input_rec0[p].name + 20, gNumWindows == 1 ? 150 : 69);
    state_utility(x, y, label);
    ct_SetJust(0, 1);
    if (D_8015418C[p] == 0) {
        if (state_word_a & 0x7C0000) {
            if (gNumWindows == 1) ct_font_set(10);
            else ct_font_set(11);
            dispatch_handler(1);
            D_80118E20[0] = 1;
            D_80118E20[1] = 1;
            camera_auto_follow(D_80113EE0[gNumWindows - 1][p].x, D_80113EE0[gNumWindows - 1][p].f20,
                               D_80113EE0[gNumWindows - 1][p].w, D_80113EE0[gNumWindows - 1][p].h,
                               -1, 0, countdown_state.fixed[38]);
            D_80118E20[1] = 3;
            D_80118E20[0] = 0;
        }
        goto end;
    }
    if (gNumWindows == 1) ct_font_set(11);
    else ct_font_set(12);
    for (i = 0; i < 12; i++) {
        if (D_80111998[p][i].angle > 0.69813168f && D_80111998[p][i].angle < 2.443461f) continue;
        if (D_80111998[p][i].alpha == 0) continue;
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
            if (!ct_PakRumble(input_rec0[p].pak) && i == 10) dispatch_handler(3);
        } else {
            func_800BEA3C(D_801146BC[0], D_801146BC[1]);
            dispatch_handler(22);
        }
        state_utility(x, y, countdown_state.strings[countdown_state.indices->controlNames + i]);
        if (i == 10) {
            brake_light_update(p, D_80111998[p][12].pos, &gCamera[p], 0, screenPos);
            x = screenPos[0] - 25.0f * gTextPosScale * 300.0f / D_80112AB0;
            y = screenPos[1];
            if (!ct_PakRumble(input_rec0[p].pak)) str = countdown_state.strings[countdown_state.indices->rumbleOff];
            else str = countdown_state.strings[countdown_state.indices->rumbleNames + ((s8 *)*input_rec0[p].name)[43]];
            state_utility(x, y, str);
        }
    }
end:
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
            continue;
        }
        ct_font_set(11);
        if (input_rec0[p].pak == 5) continue;
        if (D_8015418C[p] != 1 && !(state_word_a & 0x7C0000)) continue;
        y = D_80113EE0[gNumWindows - 1][p].y + 10;
        x = D_80113EE0[gNumWindows - 1][p].x;
        dispatch_handler(D_801146AC[p]);
        if (D_80156CF0[p].b0 == 0) {
            fcvt_wrapper(str1, D_80120240, (p * 1) + 1, countdown_state.fixed[233]);
        } else {
            race_position_update(pName, *input_rec0[p].name + 20, 69);
            fcvt_wrapper(str1, countdown_state.fixed[37], pName);
        }
        state_utility(ct_CenterText(str1, x), y, str1);
        object_bytes_sum_global();
        if (D_8015418C[p] == 0) {
            if (!(state_word_a & 0x7C0000)) continue;
            dispatch_handler(1);
            ct_SetJust(1, 1);
            camera_auto_follow(D_80113EE0[gNumWindows - 1][p].x, D_80113EE0[gNumWindows - 1][p].f20,
                               D_80113EE0[gNumWindows - 1][p].w / 2, D_80113EE0[gNumWindows - 1][p].h,
                               -1, 0, countdown_state.fixed[38]);
            ct_SetJust(0, 3);
            continue;
        }
        x = D_80113EE0[gNumWindows - 1][p].x + 10;
        y = D_80113EE0[gNumWindows - 1][p].y + 24;
        itemsDrawn = 0;
        for (i = 0; i < 12; i++) {
            if (i < D_8014A100[p]) continue;
            if (!ct_validOption(p, i)) continue;
            if (!ct_PakRumble(input_rec0[p].pak) && i == 10) dispatch_handler(3);
            else dispatch_handler(i == D_8014A0F8[p] ? 22 : 1);
            str = countdown_state.strings[countdown_state.indices->controlNames + i];
            if (i == 11) state_utility(ct_CenterText(str, D_80113EE0[gNumWindows - 1][p].x), y, str);
            else state_utility(ct_RightText(str, x), y, str);
            if (i == 10) {
                if (!ct_PakRumble(input_rec0[p].pak)) str = countdown_state.strings[countdown_state.indices->rumbleOff];
                else str = countdown_state.strings[countdown_state.indices->rumbleNames + ((s8 *)*input_rec0[p].name)[43]];
                state_utility(x + 20, y, str);
            }
            itemsDrawn++;
            y += 16;
            if (itemsDrawn >= D_8014A10A) break;
        }
    }
    render_helper(-1.0f);
    return 1;
}
