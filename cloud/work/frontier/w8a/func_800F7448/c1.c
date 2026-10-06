typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u32 w0;
    u32 w1;
} Gwords;

typedef union {
    Gwords words;
    long long int force_structure_alignment;
} Gfx;

#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define G_RDPPIPESYNC 0xE7
#define G_SETCIMG 0xFF
#define G_SETFILLCOLOR 0xF7
#define G_FILLRECT 0xF6
#define G_IM_FMT_RGBA 0
#define G_IM_SIZ_16b 2

#define gSetImage(pkt, cmd, fmt, siz, width, i) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(cmd, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL((width) - 1, 0, 12); \
    _g->words.w1 = (u32)(i); \
}
#define gDPSetColorImage(pkt, f, s, w, i) gSetImage(pkt, G_SETCIMG, f, s, w, i)
#define gDPNoParam(pkt, cmd) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(cmd, 24, 8); \
    _g->words.w1 = 0; \
}
#define gDPPipeSync(pkt) gDPNoParam(pkt, G_RDPPIPESYNC)
#define gDPSetColor(pkt, c, d) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(c, 24, 8); \
    _g->words.w1 = (unsigned int)(d); \
}
#define gDPSetFillColor(pkt, d) gDPSetColor(pkt, G_SETFILLCOLOR, d)
#define gDPFillRectangle(pkt, ulx, uly, lrx, lry) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_FILLRECT, 24, 8) | _SHIFTL((lrx), 14, 10) | _SHIFTL((lry), 2, 10)); \
    _g->words.w1 = (_SHIFTL((ulx), 14, 10) | _SHIFTL((uly), 2, 10)); \
}

typedef struct { void *buf; u8 pad[124]; } FrameBuf;

extern Gfx *D_801497C8;
extern s32 D_8002AFC0;
extern s32 D_8002AFC4;
extern volatile s8 D_8015F72D;
extern FrameBuf D_80156C5C[];
u32 osVirtualToPhysical(void *);

void func_800F7448(u16 color)
{
    gDPPipeSync(D_801497C8++);
    gDPSetColorImage(D_801497C8++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_8002AFC0, osVirtualToPhysical(D_80156C5C[D_8015F72D].buf));
    gDPSetFillColor(D_801497C8++, (color << 16) | color);
    gDPFillRectangle(D_801497C8++, 0, 0, D_8002AFC0 - 1, D_8002AFC4 - 1);
    gDPPipeSync(D_801497C8++);
}
