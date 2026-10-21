/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * animPlace (0x80107AF4, 1,000 B; historical label race_finish) = GC hud.c `static int animPlace(void)` (GC locals
 * digits[3], str[2], digitW, x, y, digitIdx, slot). Per-player race position "P/N" HUD (place + 1, '/', car count),
 * drawn glyph by glyph with a shadow pass at (x+1, y+1); the place digit in the player colour D_80116168[i].
 * GC-derived names: func_800CF604 = AreWeSolid, D_80115CA8 = placePos, D_80116168 = playerColor, D_8016137C =
 * gShowPlace, camera_shake_update = HAL_GetCharWidth, render_helper = HAL_SetTextDepth, D_80118E20 = TB_SetJust's
 * pair, dispatch_handler = TB_SetTextColor, state_utility = TB_DrawText (s16, s16, char *).
 * Lanes w28d/w29c/w30d. EQUAL in the whole-program unit (w30d). Same TU as animElapsedTimerText/func_80107EDC
 * (credits_scroll_grp, hud.c): helpers *_7AF4 are non-static and not kept (w17j; no deleted-procedure stubs; the
 * locked stub func_80107AEC before animPlace stays untouched).
 * Shaping (disclosed):
 *  - DEVICE: compiled-out `if (i) {}` at the end of the outer loop body (raises i over the int-1 constant web so i
 *    gets s8 as in retail; without it 87 rows).
 *  - `x = placePos.x; ...; x -= w;` split assignment (w11c/w16 split): the placePos address expression is then
 *    numbered before y (retail address in v0, y in v1) and the loads schedule y, place, x, w as retail.
 *  - pair_set_7AF4 writes the global directly (.second then .first, as animElapsedTimerText's pair_set_761C) at
 *    both sites; the w29c `Pair *p` form put one more spill home before (s16)(y+1)/(s16)y (homes 72/68 vs 76/72).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
typedef struct { s32 first, second; } Pair;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 r, g, b, a; } Rgba;
typedef struct {
    u8 pad000[238];
    s8 place;        /* 238 */
    s8 finished;     /* 239 */
    u8 pad0F0[952 - 240];
} Player952;
extern OSMesgQueue D_801461D0;
extern s32 state_word_a;
extern s16 D_801543CA;
extern s32 gameplay_mode;
extern s8 D_8016137C;
extern s16 gNumWindows;
extern Pair D_80118E20;
extern Pos D_80115CA8[4][4];
extern Rgba D_80116168[];
extern Player952 player_array[];
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32 selection);
extern void render_helper(f32);
extern s32 camera_shake_update(u16 id);
extern void dispatch_handler(s32);
extern void state_utility(s16, s16, char *);
extern s32 func_800CF604(s16);
extern void func_800B7360(s32, s32, s32, s32);

void gfx_lock_7AF4(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_7AF4(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_7AF4(s32 font)
{
    s32 old;

    gfx_lock_7AF4();
    old = slot_state_setup(font);
    gfx_unlock_7AF4();
    return old;
}

void pair_set_7AF4(s32 first, s32 second)
{
    D_80118E20.second = second;
    D_80118E20.first = first;
}

s32 animPlace(s32 arg0)
{
    char str[4];
    char ch[2];
    s32 w;
    s32 i;
    s32 x;
    s32 y;
    s32 k;

    if ((state_word_a & 8) || D_801543CA < 2 || gameplay_mode == 1 || !D_8016137C) {
        return 1;
    }
    render_helper(0.0f);
    pair_set_7AF4(1, 3);
    if (gNumWindows == 1) {
        font_set_7AF4(8);
    } else {
        font_set_7AF4(5);
    }
    w = camera_shake_update('8');
    str[1] = '/';
    str[2] = D_801543CA + '0';
    for (i = 0; i < gNumWindows; i++) {
        if (func_800CF604(i) == 0 || player_array[i].finished == 1) {
            continue;
        }
        x = D_80115CA8[gNumWindows - 1][i].x;
        y = D_80115CA8[gNumWindows - 1][i].y;
        str[0] = player_array[i].place + '1';
        x -= w;
        for (k = 0; k < 3; k++) {
            ch[0] = str[k];
            ch[1] = 0;
            if (k < 2) {
                if (gNumWindows == 1) {
                    font_set_7AF4(8);
                } else {
                    font_set_7AF4(5);
                }
            } else {
                if (gNumWindows == 1) {
                    font_set_7AF4(5);
                } else {
                    font_set_7AF4(6);
                }
            }
            dispatch_handler(0);
            state_utility(x + 1, y + 1, ch);
            if (k == 0) {
                func_800B7360(D_80116168[i].r, D_80116168[i].g, D_80116168[i].b, 255);
            } else {
                dispatch_handler(1);
            }
            state_utility(x, y, ch);
            x += w;
        }
        if (i) {}
    }
    pair_set_7AF4(0, 3);
    render_helper(-1.0f);
    return 1;
}
