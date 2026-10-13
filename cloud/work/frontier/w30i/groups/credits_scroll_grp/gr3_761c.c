/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * lap_complete (0x8010761C, 1,232 B) -- historical label; = GC hud.c animElapsedTimerText (static int (void);
 * GC locals t, digits[9], str[2], digitW, colonW, fixedWidth, x, y, digitIdx; D_801543CC = gModelTime,
 * D_8015723C = gShowElapsed, D_80115C88 = elapsedTimerPos, object_byte9_set = HAL_SetFontFixedWidth,
 * camera_shake_update = HAL_GetCharWidth, D_80118E20 = TB_SetJust's pair). Draws the race clock as "mm:ss.hh"
 * with a shadow pass. Lanes w27b/w28d drafts, w29c match (unit EQUAL, 0 locked bodies differ). No devices.
 * Levers: GC local list/order and statement forms (digitIdx index loop, `str[0] = digits[digitIdx]` then the
 * ':'/'.' test on str[0] through `if ((str[0] = digits[digitIdx]) != ':' && ...)`, `str[1] = 0;` before
 * y/x, y assigned before x); s32 y with state_utility(s16, s16, char *) (retail narrows both at every call);
 * the TB_SetJust pair written by an inlined pair_set(s32, s32) storing .second then .first on an s32 pair (as
 * the matched sibling func_80107EDC; the s32 1 joins the D_80151AD0 == 1 / osRecvMesg constant piece in v0,
 * and the two inlined sites size the 184-byte frame). gfx_lock/gfx_unlock/font_set/pair_set are the family's
 * inlined helpers, non-static (suffix _761C) so umerge leaves no deleted-procedure stubs (w17j, as gr3_edc.c).
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
extern OSMesgQueue D_801461D0;
extern s32 state_word_a;
extern s8 D_8015723C;
extern f32 D_801543CC;
extern s16 gNumWindows;
extern Pair D_80118E20;
extern Pos D_80115C88[];
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32 selection);
extern void render_helper(f32);
extern s32 camera_shake_update(u16 id);
extern s8 object_byte9_set(s8 value);
extern void dispatch_handler(s32);
extern void state_utility(s16, s16, char *);

void gfx_lock_761C(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_761C(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_761C(s32 font)
{
    s32 old;

    gfx_lock_761C();
    old = slot_state_setup(font);
    gfx_unlock_761C();
    return old;
}

void pair_set_761C(s32 first, s32 second)
{
    D_80118E20.second = second;
    D_80118E20.first = first;
}

s32 animElapsedTimerText(s32 arg0)
{
    s32 t;
    char digits[9];
    char str[2];
    s32 digitW;
    s32 colonW;
    s8 fixedWidth;
    s32 x;
    s32 y;
    s32 digitIdx;

    if ((state_word_a & 8) || !(state_word_a & 0x400000) || !D_8015723C) {
        return 1;
    }
    t = D_801543CC * 1000.0f;
    if (t < 0) t = 0;
    digits[0] = t / 600000 % 6 + '0';
    digits[1] = t / 60000 % 10 + '0';
    digits[2] = ':';
    digits[3] = t / 10000 % 6 + '0';
    digits[4] = t / 1000 % 10 + '0';
    digits[5] = '.';
    digits[6] = t / 100 % 10 + '0';
    digits[7] = t / 10 % 10 + '0';
    digits[8] = t % 10 + '0';
    render_helper(0.0f);
    pair_set_761C(1, 3);
    if (gNumWindows == 1) {
        font_set_761C(1);
    } else {
        font_set_761C(2);
    }
    digitW = camera_shake_update('8') + 1;
    fixedWidth = object_byte9_set(0);
    colonW = camera_shake_update(':') + 1;
    object_byte9_set(fixedWidth);
    str[1] = 0;
    y = D_80115C88[gNumWindows - 1].y;
    x = D_80115C88[gNumWindows - 1].x - (digitW * 5 + colonW * 2) / 2;
    for (digitIdx = 0; digitIdx < 8; digitIdx++) {
        if ((str[0] = digits[digitIdx]) != ':' && str[0] != '.') {
            str[0] = '8';
            dispatch_handler(0);
            state_utility(x, y, str);
            str[0] = digits[digitIdx];
        }
        dispatch_handler(1);
        state_utility(x, y, str);
        if (digitIdx == 1 || digitIdx == 2 || digitIdx == 4 || digitIdx == 5) {
            x += (digitW + colonW) / 2;
        } else {
            x += digitW;
        }
    }
    pair_set_761C(0, 3);
    render_helper(-1.0f);
    return 1;
}
