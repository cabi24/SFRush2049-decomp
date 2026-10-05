/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
#define NULL ((void *)0)
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;

typedef struct {
    u32 w0;
    u32 w1;
} Gwords;

typedef union {
    Gwords words;
    long long int force_structure_alignment;
} Gfx;

/* SDK forms, reference/repos/ultralib/include/PR/gbi.h */
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define G_SETTIMG 0xFD
#define G_SETTILE 0xF5
#define G_LOADBLOCK 0xF3
#define G_LOADTILE 0xF4
#define G_SETTILESIZE 0xF2
#define G_RDPLOADSYNC 0xE6
#define G_RDPPIPESYNC 0xE7
#define G_SETOTHERMODE_H 0xE3
#define G_MDSFT_TEXTLUT 14
#define G_TT_NONE (0 << G_MDSFT_TEXTLUT)
#define G_TT_RGBA16 (2 << G_MDSFT_TEXTLUT)
#define G_IM_FMT_RGBA 0
#define G_IM_FMT_YUV 1
#define G_IM_FMT_CI 2
#define G_IM_FMT_IA 3
#define G_IM_FMT_I 4
#define G_IM_SIZ_4b 0
#define G_IM_SIZ_8b 1
#define G_IM_SIZ_16b 2
#define G_IM_SIZ_32b 3
#define G_IM_SIZ_8b_BYTES 1
#define G_IM_SIZ_8b_TILE_BYTES G_IM_SIZ_8b_BYTES
#define G_IM_SIZ_8b_LINE_BYTES G_IM_SIZ_8b_BYTES
#define G_IM_SIZ_16b_BYTES 2
#define G_IM_SIZ_16b_TILE_BYTES G_IM_SIZ_16b_BYTES
#define G_IM_SIZ_16b_LINE_BYTES G_IM_SIZ_16b_BYTES
#define G_IM_SIZ_32b_BYTES 4
#define G_IM_SIZ_32b_TILE_BYTES 2
#define G_IM_SIZ_32b_LINE_BYTES 2
#define G_IM_SIZ_4b_LOAD_BLOCK G_IM_SIZ_16b
#define G_IM_SIZ_8b_LOAD_BLOCK G_IM_SIZ_16b
#define G_IM_SIZ_16b_LOAD_BLOCK G_IM_SIZ_16b
#define G_IM_SIZ_32b_LOAD_BLOCK G_IM_SIZ_32b
#define G_IM_SIZ_4b_SHIFT 2
#define G_IM_SIZ_8b_SHIFT 1
#define G_IM_SIZ_16b_SHIFT 0
#define G_IM_SIZ_32b_SHIFT 0
#define G_IM_SIZ_4b_INCR 3
#define G_IM_SIZ_8b_INCR 1
#define G_IM_SIZ_16b_INCR 0
#define G_IM_SIZ_32b_INCR 0
#define G_TEXTURE_IMAGE_FRAC 2
#define G_TX_LOADTILE 7
#define G_TX_RENDERTILE 0
#define G_TX_NOMIRROR 0
#define G_TX_WRAP 0
#define G_TX_MIRROR 0x1
#define G_TX_CLAMP 0x2
#define G_TX_NOMASK 0
#define G_TX_NOLOD 0
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define G_TX_DXT_FRAC 11
#define G_TX_LDBLK_MAX_TXL 2047
#define TXL2WORDS(txls, b_txl) MAX(1, ((txls)*(b_txl)/8))
#define CALC_DXT(width, b_txl) \
    (((1 << G_TX_DXT_FRAC) + TXL2WORDS(width, b_txl) - 1) / TXL2WORDS(width, b_txl))
#define TXL2WORDS_4b(txls) MAX(1, ((txls)/16))
#define CALC_DXT_4b(width) \
    (((1 << G_TX_DXT_FRAC) + TXL2WORDS_4b(width) - 1) / TXL2WORDS_4b(width))

#define gSPSetOtherMode(pkt, cmd, sft, len, data) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(cmd, 24, 8) | _SHIFTL(32 - (sft) - (len), 8, 8) | _SHIFTL((len) - 1, 0, 8)); \
    _g->words.w1 = (unsigned int)(data); \
}
#define gDPSetTextureLUT(pkt, type) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTLUT, 2, type)
#define gSetImage(pkt, cmd, fmt, siz, width, i) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(cmd, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL((width)-1, 0, 12); \
    _g->words.w1 = (unsigned int)(i); \
}
#define gDPSetTextureImage(pkt, f, s, w, i) gSetImage(pkt, G_SETTIMG, f, s, w, i)
#define gDPLoadTileGeneric(pkt, c, tile, uls, ult, lrs, lrt) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(c, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12); \
    _g->words.w1 = _SHIFTL(tile, 24, 3) | _SHIFTL(lrs, 12, 12) | _SHIFTL(lrt, 0, 12); \
}
#define gDPSetTileSize(pkt, t, uls, ult, lrs, lrt) gDPLoadTileGeneric(pkt, G_SETTILESIZE, t, uls, ult, lrs, lrt)
#define gDPLoadTile(pkt, t, uls, ult, lrs, lrt) gDPLoadTileGeneric(pkt, G_LOADTILE, t, uls, ult, lrs, lrt)
#define gDPSetTile(pkt, fmt, siz, line, tmem, tile, palette, cmt, maskt, shiftt, cms, masks, shifts) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_SETTILE, 24, 8) | _SHIFTL(fmt, 21, 3) | _SHIFTL(siz, 19, 2) | _SHIFTL(line, 9, 9) | _SHIFTL(tmem, 0, 9); \
    _g->words.w1 = _SHIFTL(tile, 24, 3) | _SHIFTL(palette, 20, 4) | _SHIFTL(cmt, 18, 2) | _SHIFTL(maskt, 14, 4) | \
                   _SHIFTL(shiftt, 10, 4) | _SHIFTL(cms, 8, 2) | _SHIFTL(masks, 4, 4) | _SHIFTL(shifts, 0, 4); \
}
#define gDPLoadBlock(pkt, tile, uls, ult, lrs, dxt) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_LOADBLOCK, 24, 8) | _SHIFTL(uls, 12, 12) | _SHIFTL(ult, 0, 12)); \
    _g->words.w1 = (_SHIFTL(tile, 24, 3) | _SHIFTL((MIN(lrs,G_TX_LDBLK_MAX_TXL)), 12, 12) | _SHIFTL(dxt, 0, 12)); \
}
#define gDPNoParam(pkt, cmd) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(cmd, 24, 8); \
    _g->words.w1 = 0; \
}
#define gDPPipeSync(pkt) gDPNoParam(pkt, G_RDPPIPESYNC)
#define gDPLoadSync(pkt) gDPNoParam(pkt, G_RDPLOADSYNC)

#define gDPLoadTextureTile(pkt, timg, fmt, siz, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz, width, timg); \
    gDPSetTile(pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_TILE_BYTES)+7)>>3), 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadTile(pkt, G_TX_LOADTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((((lrs)-(uls)+1) * siz##_LINE_BYTES)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC) \
}
#define gDPLoadTextureTile_4b(pkt, timg, fmt, width, height, uls, ult, lrs, lrt, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, G_IM_SIZ_8b, ((width)>>1), timg); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_8b, (((((lrs)-(uls)+1)>>1)+7)>>3), 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadTile(pkt, G_TX_LOADTILE, (uls)<<(G_TEXTURE_IMAGE_FRAC-1), (ult)<<(G_TEXTURE_IMAGE_FRAC), (lrs)<<(G_TEXTURE_IMAGE_FRAC-1), (lrt)<<(G_TEXTURE_IMAGE_FRAC)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_4b, (((((lrs)-(uls)+1)>>1)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, (uls)<<G_TEXTURE_IMAGE_FRAC, (ult)<<G_TEXTURE_IMAGE_FRAC, (lrs)<<G_TEXTURE_IMAGE_FRAC, (lrt)<<G_TEXTURE_IMAGE_FRAC) \
}
#define gDPLoadTextureBlock(pkt, timg, fmt, siz, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg); \
    gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT) -1, CALC_DXT(width, siz##_BYTES)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((width) * siz##_LINE_BYTES)+7)>>3, 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}
#define gDPLoadTextureBlock_4b(pkt, timg, fmt, width, height, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, G_IM_SIZ_16b, 1, timg); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1, CALC_DXT_4b(width)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, 0, 0, ((width)-1) << G_TEXTURE_IMAGE_FRAC, ((height)-1) << G_TEXTURE_IMAGE_FRAC) \
}
#define G_DL 0xDE
#define G_DL_PUSH 0x00
#define gDma1p(pkt, c, s, l, p) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL((c), 24, 8) | _SHIFTL((p), 16, 8) | _SHIFTL((l), 0, 16)); \
    _g->words.w1 = (unsigned int)(s); \
}
#define gSPDisplayList(pkt,dl) gDma1p(pkt,G_DL,dl,0,G_DL_PUSH)
#define RDL_STORAGE
#define TEX_NOPALETTE 0x20000000
#define TEX_TEXTURE 0x08000000
#define TEX_MIRROR_T 0x00080000
#define TEX_MIRROR_S 0x00040000
#define TEX_CLAMP_T 0x00020000
#define TEX_CLAMP_S 0x00010000
typedef struct {
    u8 pad0[16];
    u16 width;
    u16 height;
    u8 siz;
    u8 fmt;
    s16 palette;
    void *image;
    u32 flags;
} Texture;
typedef struct {
    u16 x0, y0, x1, y1;
} TexRect;
typedef struct {
    u8 pad0[16];
    u8 first;
    u8 last;
    u8 pad12[2];
    s32 table;
} Palette;
extern s32 D_80179A38;
extern s32 func_80087804();

#define G_LOADTLUT 0xF0
#define G_RDPTILESYNC 0xE8
#define gDPTileSync(pkt) gDPNoParam(pkt, G_RDPTILESYNC)
#define gDPLoadTLUTCmd(pkt, tile, count) \
{ \
    Gfx *_g = (Gfx *)pkt; \
    _g->words.w0 = _SHIFTL(G_LOADTLUT, 24, 8); \
    _g->words.w1 = _SHIFTL((tile), 24, 3) | _SHIFTL((count), 14, 10); \
}
#define gDPLoadTLUT(pkt, count, tmemaddr, dram) \
{ \
    gDPSetTextureImage(pkt, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, dram); \
    gDPTileSync(pkt); \
    gDPSetTile(pkt, 0, 0, 0, tmemaddr, G_TX_LOADTILE, 0, 0, 0, 0, 0, 0, 0); \
    gDPLoadSync(pkt); \
    gDPLoadTLUTCmd(pkt, G_TX_LOADTILE, ((count) - 1)); \
    gDPPipeSync(pkt); \
}

extern s32 D_8017A4B0;
extern s32 D_80161430;

void func_80099B30(Gfx **gfxp, Palette *tex) {
    Gfx *gfx = *gfxp;

    if (D_8017A4B0 != tex->table || D_80161430 != 0) {
        D_8017A4B0 = tex->table;
        gDPLoadTLUT(gfx++, tex->last - tex->first + 1, 0x100 + tex->first, tex->table);
        *gfxp = gfx;
    }
}

#define TILE(fmt, sz) gDPLoadTextureTile(gfx++, tex->image, fmt, G_IM_SIZ_##sz, tex->width, tex->height, rect->x0 >> 5, rect->y0 >> 5, rect->x1 >> 5, rect->y1 >> 5, 0, cms, cmt, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)
#define TILE4(fmt) gDPLoadTextureTile_4b(gfx++, tex->image, fmt, tex->width, tex->height, rect->x0 >> 5, rect->y0 >> 5, rect->x1 >> 5, rect->y1 >> 5, 0, cms, cmt, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)
#define BLOCK(fmt, sz) gDPLoadTextureBlock(gfx++, tex->image, fmt, G_IM_SIZ_##sz, tex->width, tex->height, 0, cms, cmt, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)
#define BLOCK4(fmt) gDPLoadTextureBlock_4b(gfx++, tex->image, fmt, tex->width, tex->height, 0, cms, cmt, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)

RDL_STORAGE void render_display_list(Texture *tex, TexRect *rect, TexRect *clip, Gfx **gp, Palette *palettes)
{
    Gfx *gfx = *gp;
    u16 masks, maskt;
    u16 cms, cmt;

    if (tex == NULL) {
        return;
    }
    if (!(tex->flags & TEX_TEXTURE)) {
        gSPDisplayList(gfx++, tex->image);
    } else {
        if (rect != NULL) {
            masks = func_80087804(rect->x0 - rect->x1) - 5;
            maskt = func_80087804(rect->y0 - rect->y1) - 5;
        } else {
            masks = func_80087804(tex->width);
            maskt = func_80087804(tex->height);
        }
        if (tex->flags & TEX_CLAMP_S) {
            cms = G_TX_CLAMP;
            masks = 0;
        } else {
            cms = G_TX_WRAP;
        }
        cms = (tex->flags & TEX_MIRROR_S) ? cms | G_TX_MIRROR : cms;
        if (tex->flags & TEX_CLAMP_T) {
            cmt = G_TX_CLAMP;
            maskt = 0;
        } else {
            cmt = G_TX_WRAP;
        }
        cmt = (tex->flags & TEX_MIRROR_T) ? cmt | G_TX_MIRROR : cmt;
        if (tex->fmt == G_IM_FMT_RGBA) {
            if (D_80179A38 != 0) {
                gDPSetTextureLUT(gfx++, G_TT_NONE);
                D_80179A38 = 0;
            }
            if (tex->siz == G_IM_SIZ_16b) {
                if (rect != NULL) {
                    TILE(G_IM_FMT_RGBA, 16b);
                } else {
                    BLOCK(G_IM_FMT_RGBA, 16b);
                }
            } else {
                if (rect != NULL) {
                    TILE(G_IM_FMT_RGBA, 32b);
                } else {
                    BLOCK(G_IM_FMT_RGBA, 32b);
                }
            }
        } else if (tex->fmt == G_IM_FMT_CI) {
            if (D_80179A38 != 1) {
                gDPSetTextureLUT(gfx++, G_TT_RGBA16);
                D_80179A38 = 1;
            }
            if (tex->siz == G_IM_SIZ_8b) {
                if (rect != NULL) {
                    TILE(G_IM_FMT_CI, 8b);
                } else {
                    BLOCK(G_IM_FMT_CI, 8b);
                }
            } else {
                if (rect != NULL) {
                    TILE4(G_IM_FMT_CI);
                } else {
                    BLOCK4(G_IM_FMT_CI);
                }
            }
        } else if (tex->fmt == G_IM_FMT_I) {
            if (D_80179A38 != 0) {
                gDPSetTextureLUT(gfx++, G_TT_NONE);
                D_80179A38 = 0;
            }
            if (tex->siz == G_IM_SIZ_8b) {
                if (rect != NULL) {
                    TILE(G_IM_FMT_I, 8b);
                } else {
                    BLOCK(G_IM_FMT_I, 8b);
                }
            } else {
                if (rect != NULL) {
                    TILE4(G_IM_FMT_I);
                } else {
                    BLOCK4(G_IM_FMT_I);
                }
            }
        } else if (tex->fmt == G_IM_FMT_IA) {
            if (D_80179A38 != 0) {
                gDPSetTextureLUT(gfx++, G_TT_NONE);
                D_80179A38 = 0;
            }
            if (tex->siz == G_IM_SIZ_16b) {
                if (rect != NULL) {
                    TILE(G_IM_FMT_IA, 16b);
                } else {
                    BLOCK(G_IM_FMT_IA, 16b);
                }
            } else if (tex->siz == G_IM_SIZ_8b) {
                if (rect != NULL) {
                    TILE(G_IM_FMT_IA, 8b);
                } else {
                    BLOCK(G_IM_FMT_IA, 8b);
                }
            } else {
                if (rect != NULL) {
                    TILE4(G_IM_FMT_IA);
                } else {
                    BLOCK4(G_IM_FMT_IA);
                }
            }
        }
    }
    if (clip != NULL) {
        gfx--;
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, clip->x0 >> 3, clip->y0 >> 3, clip->x1 >> 3, clip->y1 >> 3);
    } else if (rect != NULL) {
        gfx--;
        gDPSetTileSize(gfx++, G_TX_RENDERTILE, rect->x0 >> 3, rect->y0 >> 3, rect->x1 >> 3, rect->y1 >> 3);
    }
    if (tex->palette >= 0 && !(tex->flags & TEX_NOPALETTE) && palettes != NULL) {
        func_80099B30(&gfx, &palettes[tex->palette]);
    }
    *gp = gfx;
}

extern Gfx *D_80149438;
void zz_a(Texture *t, TexRect *r, TexRect *c, Palette *p, Palette *q) {
    Gfx *g = D_80149438;
    render_display_list(t, r, c, &g, p);
    func_80099B30(&g, q);
    D_80149438 = g;
}
void zz_b(Texture *t, TexRect *r, Palette *p, Palette *q) {
    Gfx *g = D_80149438;
    render_display_list(t, r, 0, &g, p);
    func_80099B30(&g, q);
    render_display_list(t, 0, 0, &g, p);
    D_80149438 = g;
}
