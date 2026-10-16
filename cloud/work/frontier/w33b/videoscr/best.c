/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Video-options screen text (GC videoscr.c), w32b first complete draft (NOT a match).
 * func_8010F218 (kept) = GC animText@videoscr.c with GC drawColorBars inlined at its head (as vs_drawColorBars:
 * PrimColor/FillRect GBI words on the D_80149438 display-list cursor: a white tick, 32 grey bars, a black bar, the
 * white/black/red/green/blue reference blocks); func_8010EA08 (internal, sole caller func_8010F218, unsaved
 * s0-s8/f20-f30) = GC animButtonText@videoscr.c (N64 drops GC's frequency row; keeps the test and confirm panes).
 * Callee map: w31b RESULTS §4. func_800BEA3C = TB_FlashTextColors with GC's union type (as the audioscr packet).
 * Function-local statics in GC (width$636/height$637, width$645/height$646) are written as their retail data
 * symbols here (D_80156988/D_8011AD80, D_801569B0/D_8011AD84); the two `sh 160` at EA08's entry are the widths.
 * w33b: the 32-bar loop as `for (c = 0; c < 32; c++)` with colour `c * 8` (uopt then unrolls by 4 with four
 * strength-reduced colour IVs and the exit test `!= 280`, as retail; `c += 8` / `i` counters do not), and the
 * 3-block loop branch-free (PrimColor word `(0xFF000000 >> (i * 8)) | 0xFF` = red/green/blue) with GC's
 * white/black pairs: F218 536/611 -> 518 words, rows 529 -> 324. Residual: retail fully unrolls the 3-block loop
 * (constant colours, x chain in s0/t4); here uopt keeps the loop (trip counts 3/4/8 alike: a body-size limit?),
 * so the constant webs -1/255 (s3/s1 in retail) and the frame (384 vs 456) differ.
 * Residuals (unit, w32b): F218 536/611 words -- retail's 32-bar loop is unrolled by 4 and the 3 colour blocks are
 * straight-line on an x induction register (uopt unrolling); none of 8 loop forms unrolled here. EA08 511/514
 * words, frame exact, 287 rows: the FlashTextColors argument copies (retail: first through a stack temp with an
 * `at` word copy, second loaded directly and stored to its home), colour of the loop pointers.
 * Shaping: inlined non-static helpers vs_* (GC HAL_SetFont / TB_CenterText / TB_RightText / TB_SetJust).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct { u32 w0; u32 w1; } Gwords;
typedef union { Gwords words; long long int force_structure_alignment; } Gfx;

#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define gDPNoParam(pkt, cmd) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(cmd, 24, 8); \
    _g->words.w1 = 0; \
}
#define gDPPipeSync(pkt) gDPNoParam(pkt, 0xE7)
#define gDPWords(pkt, a, b) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (a); \
    _g->words.w1 = (b); \
}
#define gDPSetPrimColor(pkt, m, l, r, g, b, a) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(0xFA, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)); \
    _g->words.w1 = (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); \
}
#define gDPFillRectangle(pkt, ulx, uly, lrx, lry) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(0xF6, 24, 8) | _SHIFTL((lrx), 14, 10) | _SHIFTL((lry), 2, 10)); \
    _g->words.w1 = (_SHIFTL((ulx), 14, 10) | _SHIFTL((uly), 2, 10)); \
}

typedef struct OSMesgQueue OSMesgQueue;
typedef struct VsIdx { u8 pad0[48]; u16 yesNo; u8 pad50[32]; u16 optionNames; } VsIdx;
typedef struct TextState { void *word0; char **fixed; void *word8; VsIdx *indices; char **strings; } TextState;
typedef struct ButtonInfo { u8 pad[8]; f32 angle; u8 pad12[36]; u8 pos[16]; } ButtonInfo;   /* 64 B */
typedef union TextColor { u8 c[4]; u32 rgba; } TextColor;   /* GC union __anon_21da */

extern Gfx *D_80149438;                          /* display-list cursor */
extern OSMesgQueue D_801461D0;
extern TextState countdown_state;                /* gText */
extern s32 D_8011AC94;                           /* buttonsInited */
extern s8 D_8011AD40[];                          /* validOption[4] */
extern s32 D_8011AD44;                           /* gVideoMenuSelect */
extern s32 D_8011AD4C;                           /* gVertCenter */
extern s32 D_8011AD50;                           /* gHorzCenter */
extern s8 D_8011AD54;                            /* gTestFrequency */
extern s8 D_8011AD58;                            /* gConfirmFrequency */
extern s8 D_8011AD5C;                            /* gYesFrequency */
extern void *D_8011AD60;                         /* gTestPane */
extern void *D_8011AD64;                         /* gConfirmPane */
extern s8 D_8011AD6C;                            /* gControllerRemoved */
extern s32 D_8011AD7C;                           /* menuRect.x */
extern s16 D_8011AD80;                           /* height$637 */
extern s16 D_8011AD84;                           /* height$646 */
extern s16 D_80156988;                           /* width$636 */
extern s16 D_801569B0;                           /* width$645 */
extern s32 D_8015698C;                           /* playerInPause */
extern s32 state_word_a;                         /* gGameState */
extern ButtonInfo D_8011A994[];                  /* buttonInfo */
extern u8 D_8011AC84[];                          /* buttonInfo + 0x2f0 (title position) */
extern u8 gCamera[];
extern TextColor D_801146BC[2];                  /* flashColors */
extern TextColor D_80118E28, D_80118E2C;
extern s32 D_80118E20[2];                        /* TB_SetJust pair */
extern s32 osRecvMesg(OSMesgQueue *, void **, s32), osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32);
extern u32 object_manager_update(char *, s32);   /* HAL_GetTextWidth */
extern s16 object_bytes_sum_global(void);        /* TB_GetFontLineHeight */
extern void dispatch_handler(s32);               /* TB_SetTextColor */
extern void state_utility(s16, s16, char *);     /* TB_DrawText */
extern s32 fcvt_wrapper(char *, char *, ...);    /* TB_Sprintf */
extern void render_helper(f32);                  /* HAL_SetTextDepth */
extern void func_800ED66C(f32);                  /* HAL_SetTextTranslucency */
extern void brake_light_update(s32, void *, void *, void *, s16 *);  /* ZOID_ProjectWindow */
extern s16 camera_zoom_fov(s16, char *);         /* TB_GetTextNumLines */
extern void *ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);   /* InitPane */
extern void camera_auto_follow(s16, s16, s16, s16, s32, s32, char *);    /* TB_DrawTextScrollBox */
extern void sound_handles_array_clear(void *);   /* RemovePane */

void func_800BEA3C(TextColor first, TextColor second)   /* TB_FlashTextColors (GC union type) */
{
    D_80118E28.rgba = first.rgba;
    D_80118E2C.rgba = second.rgba;
}
void func_8010EA08(void);

void vs_gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
void vs_gfx_unlock(void) { osSendMesg(&D_801461D0, 0, 0); }
s32 vs_font_set(s32 font)
{
    s32 old;
    vs_gfx_lock();
    old = slot_state_setup(font);
    vs_gfx_unlock();
    return old;
}
void vs_SetFont(s32 font)
{
    vs_font_set(font);
}
s16 vs_CenterText(char *text, s16 centerX)
{
    return centerX - (object_manager_update(text, -1) >> 1);
}
s16 vs_RightText(char *text, s16 rightX)
{
    return rightX - object_manager_update(text, -1);
}
void vs_SetJust(s32 h, s32 v)
{
    D_80118E20[1] = v;
    D_80118E20[0] = h;
}

void vs_drawColorBars(void)
{
    s32 x;
    s32 c;
    s32 i;

    gDPPipeSync(D_80149438++);
    gDPWords(D_80149438++, 0xFCFFFFFF, 0xFFFDF6FB);
    gDPWords(D_80149438++, 0xEE000000, 0x00100000);
    gDPSetPrimColor(D_80149438++, 0, 0, 255, 255, 255, 255);
    gDPFillRectangle(D_80149438++, 24, 140, 32, 160);
    x = 32;
    for (c = 0; c < 32; c++) {
        gDPSetPrimColor(D_80149438++, 0, 0, c * 8, c * 8, c * 8, 255);
        gDPFillRectangle(D_80149438++, x, 140, x + 8, 160);
        x += 8;
    }
    gDPSetPrimColor(D_80149438++, 0, 0, 0, 0, 0, 255);
    gDPFillRectangle(D_80149438++, x, 140, x + 8, 160);
    gDPSetPrimColor(D_80149438++, 0, 0, 255, 255, 255, 255);
    gDPFillRectangle(D_80149438++, 24, 160, 56, 180);
    gDPSetPrimColor(D_80149438++, 0, 0, 0, 0, 0, 255);
    gDPFillRectangle(D_80149438++, 24, 180, 56, 200);
    x = 56;
    for (i = 0; i < 3; i++) {
        gDPWords(D_80149438++, 0xFA000000, (0xFF000000 >> (i * 8)) | 0xFF);
        gDPFillRectangle(D_80149438++, x, 160, x + 48, 200);
        x += 48;
        gDPSetPrimColor(D_80149438++, 0, 0, 255, 255, 255, 255);
        gDPFillRectangle(D_80149438++, x, 160, x + 32, 180);
        gDPSetPrimColor(D_80149438++, 0, 0, 0, 0, 0, 255);
        gDPFillRectangle(D_80149438++, x, 180, x + 32, 200);
        x += 32;
    }
    gDPPipeSync(D_80149438++);
}

s32 func_8010F218(s32 arg0)
{
    s32 i;
    s32 x;
    s32 y;
    char *str;
    char str1[40];

    vs_drawColorBars();
    if (D_8011AC94 == 1) {
        func_8010EA08();
        return 1;
    }
    render_helper(0.0f);
    vs_SetFont(13);
    dispatch_handler(1);
    if ((state_word_a & 0x7C0000) && D_8011AD6C) {
        fcvt_wrapper(str1, "CONTROLLER %d %s", D_8015698C + 1, countdown_state.fixed[233]);
        state_utility(vs_CenterText(str1, 160), 10, str1);
    } else {
        str = countdown_state.fixed[53];
        state_utility(vs_CenterText(str, 160), 10, str);
    }
    y = object_bytes_sum_global() * 2 + 10;
    vs_font_set(11);
    x = D_8011AD7C + 5;
    for (i = 0; i < 4; i++) {
        if (!D_8011AD40[i]) continue;
        dispatch_handler(i == D_8011AD44 ? 22 : 1);
        str = countdown_state.strings[countdown_state.indices->optionNames + i];
        state_utility(vs_RightText(str, D_8011AD7C - 5), y, str);
        switch (i) {
        case 0:
            fcvt_wrapper(str1, "%s%d", D_8011AD4C > 0 ? "+" : "", D_8011AD4C);
            state_utility(x, y, str1);
            break;
        case 1:
            fcvt_wrapper(str1, "%s%d", D_8011AD50 > 0 ? "+" : "", D_8011AD50);
            state_utility(x, y, str1);
            break;
        }
        y += object_bytes_sum_global();
    }
    func_800ED66C(-1.0f);
    func_800BEA3C(D_801146BC[0], D_801146BC[1]);
    render_helper(-1.0f);
    return 1;
}

void func_8010EA08(void)
{
    s32 i;
    s32 x;
    s32 y;
    char *str;
    char str1[40];
    s16 screenPos[2];
    s16 lines;

    D_801569B0 = 160;
    D_80156988 = 160;
    render_helper(0.0f);
    vs_SetJust(0, 1);
    vs_SetFont(13);
    dispatch_handler(1);
    brake_light_update(0, D_8011AC84, gCamera, 0, screenPos);
    x = screenPos[0];
    y = screenPos[1];
    str = countdown_state.fixed[53];
    state_utility(vs_CenterText(str, x), y, str);
    vs_font_set(11);
    for (i = 0; i < 4; i++) {
        if (!D_8011AD40[i]) continue;
        if (D_8011A994[i].angle > 0.69813168f && D_8011A994[i].angle < 2.443461f) continue;
        if (D_8011A994[i].angle > 0.34906584f && D_8011A994[i].angle < 2.7925267f) func_800ED66C(128.0f);
        else func_800ED66C(-1.0f);
        if (D_8011A994[i].angle < 1.5707964f) {
            func_800BEA3C(D_801146BC[0], D_801146BC[1]);
            dispatch_handler(1);
        } else {
            func_800BEA3C(D_801146BC[0], D_801146BC[1]);
            dispatch_handler(22);
        }
        brake_light_update(0, D_8011A994[i].pos, gCamera, 0, screenPos);
        x = screenPos[0];
        y = screenPos[1];
        x -= 55;
        state_utility(x, y, countdown_state.strings[countdown_state.indices->optionNames + i]);
        brake_light_update(0, D_8011A994[i + 4].pos, gCamera, 0, screenPos);
        x = screenPos[0];
        y = screenPos[1];
        x -= 35;
        switch (i) {
        case 0:
            fcvt_wrapper(str1, "%s%d", D_8011AD4C > 0 ? "+" : "", D_8011AD4C);
            state_utility(x, y, str1);
            break;
        case 1:
            fcvt_wrapper(str1, "%s%d", D_8011AD50 > 0 ? "+" : "", D_8011AD50);
            state_utility(x, y, str1);
            break;
        }
    }
    vs_SetJust(1, 3);
    func_800ED66C(-1.0f);
    func_800BEA3C(D_801146BC[0], D_801146BC[1]);
    render_helper(-1.0f);
    if (D_8011AD54 == 1) {
        vs_SetFont(10);
        if (D_8011AD60 == 0) {
            lines = camera_zoom_fov(D_80156988, countdown_state.fixed[217]);
            D_8011AD80 = object_bytes_sum_global() * lines;
            D_8011AD60 = ambient_sound_set(152 - D_80156988 / 2, 51, D_80156988 / 2 + 168, D_8011AD80 + 59, 176, 0, 0, 0);
        }
        camera_auto_follow(160, 55, D_80156988, D_8011AD80, -1, 0, countdown_state.fixed[217]);
    } else if (D_8011AD60 != 0) {
        sound_handles_array_clear(D_8011AD60);
        D_8011AD60 = 0;
    }
    if (D_8011AD58 == 1) {
        vs_SetFont(10);
        if (D_8011AD64 == 0) {
            lines = camera_zoom_fov(D_801569B0, countdown_state.fixed[218]) + 1;
            D_8011AD84 = object_bytes_sum_global() * lines;
            D_8011AD64 = ambient_sound_set(152 - D_801569B0 / 2, 51, D_801569B0 / 2 + 168, D_8011AD84 + 59, 176, 0, 0, 0);
        }
        camera_auto_follow(160, 55, D_801569B0, D_8011AD84, -1, 0, countdown_state.fixed[218]);
        y = D_8011AD84 - object_bytes_sum_global() + 55;
        x = 160 - D_801569B0 / 4;
        str = countdown_state.strings[countdown_state.indices->yesNo];
        dispatch_handler(D_8011AD5C ? 22 : 1);
        state_utility(x, y, str);
        x = D_801569B0 / 4 + 160;
        str = countdown_state.strings[countdown_state.indices->yesNo + 1];
        dispatch_handler(D_8011AD5C ? 1 : 22);
        state_utility(x, y, str);
    } else if (D_8011AD64 != 0) {
        sound_handles_array_clear(D_8011AD64);
        D_8011AD64 = 0;
    }
    vs_SetJust(0, 3);
}
