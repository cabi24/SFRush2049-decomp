/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * dust_cloud_effect (0x800EFEA8, 600 B; historical label), w36w3 (wave 36): an N64 hud.c blit animation callback
 * (the function right before ShowHUD = skid_mark_render), same family as the matched animLapTime (gr3_8154.c) and
 * animPlace: the blit's data word packs the window (low nibble) and a display mode (bits 4..7: 1 = single-window
 * only, 2 = split-screen only). Hidden when the mode does not fit gNumWindows (D_80151AD0) or the window does not
 * exist (then state = 0), or when D_8015F72C is clear or the window's car (gGameCar = D_80152818, 952 B, indexed
 * by model[win].slot: D_8014A250, 0x808 B, s16 at +1990) has place_locked (+239) == 1. Otherwise font 4 (one
 * window) or 1 under the font lock, digit width = camera_shake_update('8') (HAL_GetCharWidth), position from the
 * per-window-count table D_80115A68[4][4] {x, y}: x = pos.x - 2 * digitW - 2, y = pos.y + 1 in single-window,
 * alpha = D_80116187, half18 = 100, UpdateBlit (Input_ApplyPadConfig). MAT3 identity open (the GC hud.c has no
 * blit per window for this element); structure from the matched N64 siblings.
 * Needs slot_state_setup's IPA convention (parameter in s2, s0/s1/s3 unsaved): group context ctx_gr3_b.c.
 * Shaping (each verified necessary): Hidden / gfx_lock / gfx_unlock / font_set are non-static inlined helpers as
 * in animLapTime; the single-window y step is a `?:` (IDO's li v0,1 / move v0,zero arms); alpha is stored before
 * half18 (ugen temp order: retail t7 = alpha, t8 = 100).
 * Disclosed frame residual: x, y, yoff, w and h are UNUSED named locals (5 slots, 20 bytes). Retail frame 144
 * with win's home at sp+136 (one named slot above it) and the inlined font_set homes at 76/52; without the five
 * the frame is 120 (win at 116); with x, y, yoff only it is 136 (win at 128).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct Blit {
    u32 word0;
    void *rgba;
    u32 word8;
    u16 half12;
    s16 x, y;
    u16 half18;
    s16 w, h;
    u8 alpha, byte25;
    s8 hidden;
    u8 byte27;
    s16 left, right, top, bottom;
    u32 word36;
    s32 state;
    s32 data;
} Blit;

typedef struct Model { u8 pad0[1990]; s16 slot; u8 pad7C8[2056 - 1992]; } Model;   /* model[] 0x808 B */
typedef struct Car { u8 pad0[239]; s8 f239; u8 padF0[952 - 240]; } Car;
typedef struct Pos { s32 x, y; } Pos;

extern OSMesgQueue D_801461D0;
extern s16 D_80151AD0;
#define gNumWindows D_80151AD0
extern s8 D_8015F72C;
extern Model D_8014A250[];
extern Car D_80152818[];
extern u8 D_80116187;
extern Pos D_80115A68[][4];

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void Input_ApplyPadConfig(void *);
s32 camera_shake_update(u16 id);

void gfx_lock_dust(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_dust(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_dust(s32 font)
{
    s32 old;

    gfx_lock_dust();
    old = slot_state_setup(font);
    gfx_unlock_dust();
    return old;
}

s32 Hidden_dust(Blit *blt, s32 hide)
{
    if (blt->hidden != hide) {
        blt->hidden = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->hidden;
}

s32 dust_cloud_effect(Blit *blt)
{
    s32 x;          /* unused (frame residual) */
    s32 win;
    s32 mode;
    s32 hide;
    s32 digitW;
    s32 y;          /* unused (frame residual) */
    s32 yoff;       /* unused (frame residual) */
    s32 w;          /* unused (frame residual) */
    s32 h;          /* unused (frame residual) */

    win = blt->data & 15;
    mode = ((u32)blt->data & 0xF0) >> 4;
    hide = (mode == 1 && gNumWindows >= 2) || (mode == 2 && gNumWindows < 2) || win >= gNumWindows;
    if (Hidden_dust(blt, hide)) {
        blt->state = 0;
        return 1;
    }
    hide = !D_8015F72C || D_80152818[D_8014A250[win].slot].f239 == 1;
    if (Hidden_dust(blt, hide)) {
        return 1;
    }
    if (gNumWindows == 1) {
        font_set_dust(4);
    } else {
        font_set_dust(1);
    }
    digitW = camera_shake_update(56);
    blt->x = D_80115A68[gNumWindows - 1][win].x - digitW * 2 - 2;
    blt->y = D_80115A68[gNumWindows - 1][win].y + ((gNumWindows == 1) ? 1 : 0);
    blt->alpha = D_80116187;
    blt->half18 = 100;
    Input_ApplyPadConfig(blt);
    return 1;
}
