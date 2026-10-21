/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * checkpoint_hit (0x801073D8, 580 B) = animElapsedTimer(struct _tBlit *blt) of hud.c (MAT3 GameCube hud.c
 * animElapsedTimer; w36w). Elapsed-time box for window `win` (blit data word): removed (state = 0) when the
 * window does not exist, hidden in attract/when not racing/!gShowElapsed (D_8015723C); font 1 or 2; width =
 * 7 digit widths ('8'+1) + 2 colon widths (':'+1, digits fixed-width via HAL_SetFontFixedWidth =
 * object_byte9_set); position elapsedTimerPos[gNumWindows-1] (D_80115C88); N64: alpha from the colour byte
 * D_80116184[3] and half18 = 100 (GC: alpha = 128); UpdateBlit = Input_ApplyPadConfig.
 * Same helper shapes as animLapTime (gr3_8154.c): Hidden/gfx_lock/gfx_unlock/font_set inlined, non-static.
 * The alpha store is written before the half18 store (retail's schedule).
 * DISCLOSED: x, w, show (before win) and y (after fixedWidth) are UNUSED locals = exact frame residual (the
 * names follow sibling animLapTime). Retail frame 144 with digitW at sp+124, fixedWidth at sp+119; without them
 * the frame is 128 (8 words differ); x/w/show alone give 136 with the right top offsets.
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

typedef struct Car {
    u8 pad0[239];
    s8 f239;
    u8 padF0[952 - 240];
} Car;

typedef struct Pos {
    s32 x, y;
} Pos;

extern OSMesgQueue D_801461D0;
extern u32 D_801174B4;
extern s16 gNumWindows;
extern s8 D_8015723C;
extern f32 D_80142740[];
extern Car D_80152818[];
extern u8 D_80116188[][4];
extern Pos D_80115EA8[][4];
extern Pos D_80115C88[];     /* elapsedTimerPos[gNumWindows - 1] */
extern u8 D_80116184[4];     /* elapsed-timer colour (alpha byte) */

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void Input_ApplyPadConfig(void *);
s32 camera_shake_update(u16 id);
s8 object_byte9_set(s8 value);
s16 object_bytes_sum_global(void);

void gfx_lock_73d8(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_73d8(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_73d8(s32 font)
{
    s32 old;

    gfx_lock_73d8();
    old = slot_state_setup(font);
    gfx_unlock_73d8();
    return old;
}

s32 Hidden_73d8(Blit *blt, s32 hide)
{
    if (blt->hidden != hide) {
        blt->hidden = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->hidden;
}

s32 checkpoint_hit(Blit *blt)
{
    s32 x;
    s32 w;
    s32 show;
    s32 win;
    s32 digitW;
    s32 colonW;
    s8 fixedWidth;
    s32 y;

    win = blt->data;
    if (Hidden_73d8(blt, (win == 1 && gNumWindows > 1) || (win == 2 && gNumWindows < 2))) {
        blt->state = 0;
        return 1;
    }
    if (Hidden_73d8(blt, (D_801174B4 & 8) || !(D_801174B4 & 0x400000) || !D_8015723C)) {
        return 1;
    }
    if (gNumWindows == 1) {
        font_set_73d8(1);
    } else {
        font_set_73d8(2);
    }
    digitW = camera_shake_update(56) + 1;
    fixedWidth = object_byte9_set(0);
    colonW = camera_shake_update(58) + 1;
    object_byte9_set(fixedWidth);
    blt->x = D_80115C88[gNumWindows - 1].x - (7 * digitW + 2 * colonW) / 2;
    blt->y = D_80115C88[gNumWindows - 1].y - 1;
    blt->alpha = D_80116184[3];
    blt->half18 = 100;
    Input_ApplyPadConfig(blt);
    return 1;
}
