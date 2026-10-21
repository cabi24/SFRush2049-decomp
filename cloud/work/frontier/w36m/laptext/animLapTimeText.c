/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_801084D4 (0x801084D4, 1500 B) = GC hud.c animLapTimeText (MAT3 decomp/src/GameSource/src/hud.c
 * animLapTimeText; GC locals t, remaining, digits[9], str[2], digitW, colonW, x, y, digitIdx, slot,
 * fixedWidth). Per-window lap split "m:ss.hhh" with a shadow pass, fading out over the last 3 s of
 * gCheckPtTimeout. gCheckPtTimeout = D_80142740, gLapTime = D_80142770, gShowElapsed = D_8015723C,
 * lapTimePos = D_80115EA8, gPauseState = D_801170FC, gFrameSeconds = D_8002EB94 (volatile, as MAT3),
 * HAL_SetTextTranslucency = func_800ED66C, TB_DrawText = state_utility, TB_SetTextColor = dispatch_handler,
 * HAL_GetCharWidth = camera_shake_update, HAL_SetFontFixedWidth = object_byte9_set, HAL_SetFont =
 * slot_state_setup (s2 IPA parameter), TB_SetJust pair = D_80118E20.
 * N64 differences from GC: an unused s32 parameter (stored to its home, as the sibling animElapsedTimerText);
 * `remaining` declared last and assigned after the continue test; the loop's zero is spelled `0.f` (a different
 * uopt constant from render_helper's 0.0f: retail hoists only the loop zero); gLapTime * 1000.0f and
 * remaining * 255.0f operand order. No shaping devices. Helpers gfx_lock/gfx_unlock/font_set/pair_set are the
 * family's inlined helpers, non-static (suffix _84D4), not on keep (w17j stub rule). w36m (wave 36).
 */typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
typedef struct { s32 first, second; } Pair;
typedef struct { s32 x, y; } Pos;

typedef struct Car {
    u8 pad0[239];
    s8 place_locked;
    u8 padF0[952 - 240];
} Car;

extern OSMesgQueue D_801461D0;
extern u32 D_801174B4;
extern s8 D_8015723C;
extern s16 gNumWindows;
extern Pair D_80118E20;
extern f32 D_80142740[];
extern f32 D_80142770[];
extern Car D_80152818[];
extern Pos D_80115EA8[][4];
extern s32 D_801170FC;
extern volatile f32 D_8002EB94;

extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32 selection);
extern void render_helper(f32);
extern s32 camera_shake_update(u16 id);
extern s8 object_byte9_set(s8 value);
extern void dispatch_handler(s32);
extern void state_utility(s16, s16, u8 *);
extern void func_800ED66C(f32);

void gfx_lock_84D4(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_84D4(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_84D4(s32 font)
{
    s32 old;

    gfx_lock_84D4();
    old = slot_state_setup(font);
    gfx_unlock_84D4();
    return old;
}

void pair_set_84D4(s32 first, s32 second)
{
    D_80118E20.second = second;
    D_80118E20.first = first;
}

s32 func_801084D4(s32 arg0)
{
    s32 t;
    u8 digits[9];
    u8 str[2];
    s32 digitW;
    s32 colonW;
    s32 x;
    s32 y;
    s32 digitIdx;
    s32 slot;
    s8 fixedWidth;
    f32 remaining;

    if ((D_801174B4 & 8) || !D_8015723C) {
        return 1;
    }
    render_helper(0.0f);
    pair_set_84D4(1, 3);
    if (gNumWindows == 1) {
        font_set_84D4(1);
    } else {
        font_set_84D4(2);
    }
    digitW = camera_shake_update('8') + 1;
    fixedWidth = object_byte9_set(0);
    colonW = camera_shake_update(':') + 1;
    object_byte9_set(fixedWidth);
    for (slot = 0; slot < gNumWindows; slot++) {
        if (D_801170FC == 0) {
            D_80142740[slot] -= D_8002EB94;
        }
        if (D_80142740[slot] <= 0.f || D_80152818[slot].place_locked == 1) {
            D_80142740[slot] = 0.f;
            continue;
        }
        remaining = D_80142740[slot];
        t = D_80142770[slot] * 1000.0f;
        if (t < 0) {
            t = 0;
        }
        digits[0] = t / 600000 + '0';
        digits[1] = t / 60000 % 10 + '0';
        digits[2] = ':';
        digits[3] = t / 10000 % 6 + '0';
        digits[4] = t / 1000 % 10 + '0';
        digits[5] = '.';
        digits[6] = t / 100 % 10 + '0';
        digits[7] = t / 10 % 10 + '0';
        digits[8] = t % 10 + '0';
        y = D_80115EA8[gNumWindows - 1][slot].y;
        str[1] = 0;
        if (remaining < 3.0f) {
            func_800ED66C((s32)(remaining * 255.0f / 3.0f));
        }
        x = D_80115EA8[gNumWindows - 1][slot].x - (digitW * 6 + colonW * 2) / 2;
        for (digitIdx = 0; digitIdx < 9; digitIdx++) {
            str[0] = digits[digitIdx];
            if (digits[digitIdx] != ':' && digits[digitIdx] != '.') {
                str[0] = '8';
                dispatch_handler(0);
                state_utility(x, y, str);
                str[0] = digits[digitIdx];
            }
            dispatch_handler(22);
            state_utility(x, y, str);
            if (digitIdx == 1 || digitIdx == 2 || digitIdx == 4 || digitIdx == 5) {
                x += (digitW + colonW) / 2;
            } else {
                x += digitW;
            }
        }
        func_800ED66C(-1.0f);
    }
    pair_set_84D4(0, 3);
    render_helper(-1.0f);
    return 1;
}
