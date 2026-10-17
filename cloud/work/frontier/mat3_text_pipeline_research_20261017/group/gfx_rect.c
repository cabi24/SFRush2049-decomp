/*
 * func_8008A46C(x0, y0, x1, y1, color): clipped solid rectangle (N64-only RDP
 * helper, no arcade ancestor).  Clamp the rectangle to the scissor box
 * (D_8012E60C/D_8012E668 .. D_8012E610/D_8012E674), return if empty, then
 * gDPPipeSync, gDPSetPrimColor(color[0..3]), remember the alpha in
 * D_8011EACF, func_8008A148(color, -1, -1, 0) (texture/colour cache),
 * func_80086A50(1) (render mode), func_800878E0(0x4000) (set state bit 14:
 * primitive-colour combine), gDPFillRectangle(x0, y0, x1 + 1, y1 + 1),
 * gDPPipeSync, func_8008705C(0x4000).
 *
 * Whole-program context: the real func_80086A50 must be an internal
 * procedure of the same -O3 unit.  Its register summary is why retail keeps
 * the four coordinates in t2-t5 and does not save them around that call
 * (it does around func_8008A148 and func_800878E0, which are kept).  The
 * group is src/blob/groups/gfx_modes (func_80086A50.c, modes.c unchanged)
 * plus src/blob/func_8008A148.c (unchanged) plus this file.
 * No shaping quirks: SDK GBI macros on the global Gfx *.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

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
#define G_SETPRIMCOLOR 0xFA
#define G_FILLRECT 0xF6
#define gDPNoParam(pkt, cmd) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(cmd, 24, 8); \
    _g->words.w1 = 0; \
}
#define gDPPipeSync(pkt) gDPNoParam(pkt, G_RDPPIPESYNC)
#define gDPSetPrimColor(pkt, m, l, r, g, b, a) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_SETPRIMCOLOR, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)); \
    _g->words.w1 = (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); \
}
#define gDPFillRectangle(pkt, ulx, uly, lrx, lry) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_FILLRECT, 24, 8) | _SHIFTL((lrx), 14, 10) | _SHIFTL((lry), 2, 10)); \
    _g->words.w1 = (_SHIFTL((ulx), 14, 10) | _SHIFTL((uly), 2, 10)); \
}

extern Gfx *D_80149438;
extern s32 D_8012E60C;
extern s32 D_8012E668;
extern s32 D_8012E610;
extern s32 D_8012E674;
extern u8 D_8011EACF;

void func_8008A148(u8 *img, s32 mode, s32 pal, s32 bank);
void func_80086A50(s32 mode);
void func_800878E0(u32 mask);
void func_8008705C(u32 mask);

void func_8008A46C(s32 x0, s32 y0, s32 x1, s32 y1, u8 *color) {
    if (x0 < D_8012E60C) {
        x0 = D_8012E60C;
    }
    if (y0 < D_8012E668) {
        y0 = D_8012E668;
    }
    if (x1 > D_8012E610) {
        x1 = D_8012E610;
    }
    if (y1 > D_8012E674) {
        y1 = D_8012E674;
    }
    if (x1 < x0 || y1 < y0) {
        return;
    }
    gDPPipeSync(D_80149438++);
    gDPSetPrimColor(D_80149438++, 0, 0, color[0], color[1], color[2], color[3]);
    D_8011EACF = color[3];
    func_8008A148(color, -1, -1, 0);
    func_80086A50(1);
    func_800878E0(0x4000);
    gDPFillRectangle(D_80149438++, x0, y0, x1 + 1, y1 + 1);
    gDPPipeSync(D_80149438++);
    func_8008705C(0x4000);
}
