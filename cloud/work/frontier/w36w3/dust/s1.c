/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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

typedef struct Player { u8 pad0[0x22]; s16 car; u8 pad24[0x48 - 0x24]; } Player;   /* 0x8014AA16 = &D_8014A9F4[0].car? */
typedef struct Car { u8 pad0[239]; s8 f239; u8 padF0[952 - 240]; } Car;
typedef struct Pos { s32 x, y; } Pos;

extern OSMesgQueue D_801461D0;
extern s16 D_80151AD0;
#define gNumWindows D_80151AD0
extern s8 D_8015F72C;
extern s16 D_8014AA16[];
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
    s32 win;
    s32 mode;
    s32 digitW;

    win = blt->data & 15;
    mode = ((u32)blt->data & 0xF0) >> 4;
    if (Hidden_dust(blt, (mode == 1 && gNumWindows >= 2) || (mode == 2 && gNumWindows < 2) || win >= gNumWindows)) {
        blt->state = 0;
        return 1;
    }
    if (Hidden_dust(blt, !D_8015F72C || D_80152818[D_8014AA16[win * 36]].f239 == 1)) {
        return 1;
    }
    if (gNumWindows == 1) {
        font_set_dust(4);
    } else {
        font_set_dust(1);
    }
    digitW = camera_shake_update(56);
    blt->x = D_80115A68[gNumWindows - 1][win].x - digitW * 2 - 2;
    blt->y = D_80115A68[gNumWindows - 1][win].y + (gNumWindows == 1);
    blt->half18 = 100;
    blt->alpha = D_80116187;
    Input_ApplyPadConfig(blt);
    return 1;
}
