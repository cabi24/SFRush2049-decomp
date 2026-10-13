/*
 * func_80108154 (0x80108154, 896 B) = animLapTime(struct _tBlit *blt) of hud.c (GC rush_d.elf hud.c animLapTime:
 * locals w, digitW, colonW, win, show, fixedWidth; float 192). Lap-time box for window `win` (blit data word):
 * hidden past the window count or when D_801174B4 bit 3 / !D_8015723C / lap timer D_80142740[win] <= 0 / car
 * f239 == 1; font 1 (one window) or 2; width = 7 digit widths ('8'+1) + 2 colon widths (':'+1) with
 * fixed-width digits forced (object_byte9_set); fade alpha D_80116188[win][3] = timer*192/3 below 3 s.
 * Shaping (disclosed): Hidden/gfx_lock/gfx_unlock/font_set are inlined helpers (non-static: no
 * deleted-procedure stubs); `x` is unused: exact frame residual (one 4-byte named slot; retail frame 152 with
 * the locals at 148..127; without `x` the frame stays 152 but every named slot moves 4 B, 10 words differ).
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
extern s16 D_80151AD0;
extern s8 D_8015723C;
extern f32 D_80142740[];
extern Car D_80152818[];
extern u8 D_80116188[][4];
extern Pos D_80115EA8[][4];

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void Input_ApplyPadConfig(void *);
s32 camera_shake_update(u16 id);
s8 object_byte9_set(s8 value);
s16 object_bytes_sum_global(void);

void gfx_lock_8154(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_8154(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_8154(s32 font)
{
    s32 old;

    gfx_lock_8154();
    old = slot_state_setup(font);
    gfx_unlock_8154();
    return old;
}

s32 Hidden_8154(Blit *blt, s32 hide)
{
    if (blt->hidden != hide) {
        blt->hidden = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->hidden;
}

s32 func_80108154(Blit *blt)
{
    s32 x;
    s32 w;
    s32 digitW;
    s32 colonW;
    s32 show;
    s32 win;
    s8 fixedWidth;

    win = blt->data;
    if (win >= D_80151AD0) {
        blt->state = 0;
        Hidden_8154(blt, 1);
        return 1;
    }
    show = (D_801174B4 & 8) || !D_8015723C || D_80142740[win] <= 0.0f || D_80152818[win].f239 == 1;
    if (Hidden_8154(blt, show)) {
        return 1;
    }
    if (D_80151AD0 == 1) {
        font_set_8154(1);
    } else {
        font_set_8154(2);
    }
    digitW = camera_shake_update(56) + 1;
    fixedWidth = object_byte9_set(0);
    colonW = camera_shake_update(58) + 1;
    object_byte9_set(fixedWidth);
    if (D_80142740[win] < 3.0f) {
        D_80116188[win][3] = D_80142740[win] * 192.0f / 3.0f;
    } else {
        D_80116188[win][3] = 192;
    }
    w = digitW * 7 + colonW * 2;
    blt->x = D_80115EA8[D_80151AD0 - 1][win].x - w / 2;
    blt->w = w;
    blt->y = D_80115EA8[D_80151AD0 - 1][win].y;
    blt->h = object_bytes_sum_global();
    blt->alpha = D_80116188[win][3];
    blt->half18 = 100;
    blt->rgba = D_80116188[win];
    Input_ApplyPadConfig(blt);
    return 1;
}
