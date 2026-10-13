/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * attract_or_transition (0x800EDDC0, 2,076 B), historical label: per-frame display-list prologue. Selects the
 * frame-buffer record D_80156BE0[D_8015F72D] (128-byte records shared with the matched func_800F7448; volatile
 * s8 index), sets the command buffer (D_8015B250/D_8015B260, D_801497C8 = +0x9CC0), then segment 0, two static
 * display lists, the depth image, a FILL-cycle clear of the depth buffer (0xFFFC) and of the colour buffer
 * (D_80124FC8), the split-screen divider bars by wheel_render_full(0)'s layout (1: 3/4-width panes, 2: halves,
 * with the 4th quadrant blanked for 3 players), texture filter and colour dither from the options
 * (D_8015F738, D_80161380), VI dither filter / divot from D_80161398 / D_801613A4, then sound_init and
 * world_trigger_check. N64-only (no arcade or GC counterpart: correlation has only weak immediate leads).
 * Source: lane w28d, from near_miss_B136's command list rewritten with the SDK GBI macros (copied from the
 * matched sibling func_800F7448's local definitions plus the F3DEX2 gSPSegment/gSPDisplayList/gDPSetDepthImage/
 * gDPSetCycleType/gSPSetGeometryMode/gDPSetTextureFilter/gDPSetColorDither forms). No shaping devices.
 */
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

#define G_SETZIMG 0xFE
#define G_MOVEWORD 0xDB
#define G_MW_SEGMENT 0x06
#define G_DL 0xDE
#define G_DL_PUSH 0x00
#define G_SETOTHERMODE_H 0xE3
#define G_GEOMETRYMODE 0xD9
#define G_MDSFT_CYCLETYPE 20
#define G_MDSFT_TEXTFILT 12
#define G_MDSFT_RGBDITHER 6
#define G_CYC_FILL (3 << G_MDSFT_CYCLETYPE)
#define G_TF_POINT (0 << G_MDSFT_TEXTFILT)
#define G_TF_AVERAGE (3 << G_MDSFT_TEXTFILT)
#define G_TF_BILERP (2 << G_MDSFT_TEXTFILT)
#define G_CD_MAGICSQ (0 << G_MDSFT_RGBDITHER)
#define G_CD_BAYER (1 << G_MDSFT_RGBDITHER)
#define G_CD_NOISE (2 << G_MDSFT_RGBDITHER)
#define G_CD_DISABLE (3 << G_MDSFT_RGBDITHER)
#define G_ZBUFFER 0x00000001
#define gDPSetDepthImage(pkt, i) gSetImage(pkt, G_SETZIMG, 0, 0, 1, i)
#define gMoveWd(pkt, index, offset, data) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_MOVEWORD, 24, 8) | _SHIFTL(offset, 0, 16) | _SHIFTL(index, 16, 8); \
    _g->words.w1 = (unsigned int)(data); \
}
#define gSPSegment(pkt, segment, base) gMoveWd(pkt, G_MW_SEGMENT, (segment) * 4, base)
#define gDma1p(pkt, c, s, l, p) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL((c), 24, 8) | _SHIFTL((p), 16, 8) | _SHIFTL((l), 0, 16)); \
    _g->words.w1 = (unsigned int)(s); \
}
#define gSPDisplayList(pkt, dl) gDma1p(pkt, G_DL, dl, 0, G_DL_PUSH)
#define gSPSetOtherMode(pkt, cmd, sft, len, data) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(cmd, 24, 8) | _SHIFTL(32 - (sft) - (len), 8, 8) | _SHIFTL((len) - 1, 0, 8)); \
    _g->words.w1 = (unsigned int)(data); \
}
#define gDPSetCycleType(pkt, type) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_CYCLETYPE, 2, type)
#define gDPSetTextureFilter(pkt, type) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTFILT, 2, type)
#define gDPSetColorDither(pkt, mode) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_RGBDITHER, 2, mode)
#define gSPGeometryMode(pkt, c, s) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_GEOMETRYMODE, 24, 8) | _SHIFTL(~(u32)(c), 0, 24); \
    _g->words.w1 = (u32)(s); \
}
#define gSPSetGeometryMode(pkt, word) gSPGeometryMode((pkt), 0, word)

typedef struct {
    u8 pad00[88];
    u8 *commands;      /* 88 */
    u8 pad5C[32];
    void *colorBuffer; /* 124 */
} FrameBuf;

extern Gfx *D_801497C8;
extern s32 D_8002AFC0;
extern s32 D_8002AFC4;
extern volatile s8 D_8015F72D;
extern FrameBuf D_80156BE0[];
extern u8 *D_8015B250;
extern u8 *D_8015B260;
extern Gfx D_8002E528[];
extern Gfx D_8002E4A0[];
extern void *D_8002EBB0;
extern u32 D_80124FC8;
extern s16 gNumWindows;
extern s32 D_8015F738, D_80161380, D_80161398, D_801613A4;
u32 osVirtualToPhysical(void *);
void osViSetSpecialFeatures(u32);
#define OS_VI_DIVOT_ON 0x0010
#define OS_VI_DIVOT_OFF 0x0020
#define OS_VI_DITHER_FILTER_ON 0x0040
#define OS_VI_DITHER_FILTER_OFF 0x0080
s32 wheel_render_full(s32);
void sound_init(void);
void world_trigger_check(void);

void gfx_frame_begin(void) {
    u32 color;
    FrameBuf *frame;
    frame=&D_80156BE0[D_8015F72D];
    D_8015B250=frame->commands;
    D_8015B260=D_8015B250+1728;
    D_801497C8=(Gfx *)(D_8015B250+0x9CC0);
    gSPSegment(D_801497C8++, 0, 0);
    gSPDisplayList(D_801497C8++, D_8002E528);
    gSPDisplayList(D_801497C8++, D_8002E4A0);
    gDPSetDepthImage(D_801497C8++, osVirtualToPhysical(D_8002EBB0));
    gDPPipeSync(D_801497C8++);
    gDPSetCycleType(D_801497C8++, G_CYC_FILL);
    gDPSetColorImage(D_801497C8++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_8002AFC0, osVirtualToPhysical(D_8002EBB0));
    gDPSetFillColor(D_801497C8++, 0xFFFCFFFC);
    gDPFillRectangle(D_801497C8++, 0,0,D_8002AFC0-1,D_8002AFC4-1);
    gSPSetGeometryMode(D_801497C8++, G_ZBUFFER);
    color=D_80124FC8;
    gDPPipeSync(D_801497C8++);
    gDPSetColorImage(D_801497C8++, G_IM_FMT_RGBA, G_IM_SIZ_16b, D_8002AFC0, osVirtualToPhysical(frame->colorBuffer));
    gDPSetFillColor(D_801497C8++, color);
    gDPFillRectangle(D_801497C8++, 0,0,D_8002AFC0-1,D_8002AFC4-1);
    switch(wheel_render_full(0)) {
    case 2:
        if(gNumWindows==3) {
            gDPPipeSync(D_801497C8++);
            gDPSetFillColor(D_801497C8++, 0x00010001);
            gDPFillRectangle(D_801497C8++, D_8002AFC0/2,D_8002AFC4/2,D_8002AFC0-1,D_8002AFC4-1);
        }
        gDPPipeSync(D_801497C8++);
        gDPSetFillColor(D_801497C8++, 0x00010001);
        gDPFillRectangle(D_801497C8++, D_8002AFC0/2-1,0,D_8002AFC0/2,D_8002AFC4-1);
        gDPPipeSync(D_801497C8++);
        gDPFillRectangle(D_801497C8++, 0,D_8002AFC4/2-1,D_8002AFC0-1,D_8002AFC4/2);
        break;
    case 1:
        gDPPipeSync(D_801497C8++);
        gDPSetFillColor(D_801497C8++, 0x00010001);
        gDPFillRectangle(D_801497C8++, (D_8002AFC0*3)/4,0,D_8002AFC0-1,D_8002AFC4-1);
        gDPPipeSync(D_801497C8++);
        gDPFillRectangle(D_801497C8++, 0,D_8002AFC4/2-1,D_8002AFC0-1,D_8002AFC4/2);
        gDPPipeSync(D_801497C8++);
        gDPFillRectangle(D_801497C8++, 0,0,1,D_8002AFC4-1);
        break;
    }
    gDPPipeSync(D_801497C8++);
    switch(D_8015F738) {
    case 0:gDPSetTextureFilter(D_801497C8++, G_TF_BILERP);break;
    case 1:gDPSetTextureFilter(D_801497C8++, G_TF_AVERAGE);break;
    case 2:gDPSetTextureFilter(D_801497C8++, G_TF_POINT);break;
    }
    switch(D_80161380) {
    case 0:gDPSetColorDither(D_801497C8++, G_CD_DISABLE);break;
    case 1:gDPSetColorDither(D_801497C8++, G_CD_BAYER);break;
    case 2:gDPSetColorDither(D_801497C8++, G_CD_MAGICSQ);break;
    case 3:gDPSetColorDither(D_801497C8++, G_CD_NOISE);break;
    }
    switch(D_80161398) {
    case 0:osViSetSpecialFeatures(OS_VI_DITHER_FILTER_OFF);break;
    case 1:osViSetSpecialFeatures(OS_VI_DITHER_FILTER_ON);break;
    }
    if(D_801613A4)osViSetSpecialFeatures(OS_VI_DIVOT_ON);
    else osViSetSpecialFeatures(OS_VI_DIVOT_OFF);
    sound_init();
    world_trigger_check();
}
