/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * object_render (0x80087A08, 2,512 words) -- the name is a historical label. Real semantics:
 * "select texture image": make `img` the current RDP texture, emitting the load into the global
 * display list D_80149438, unless it is already the cached texture.
 *
 *   void object_render(void *img, u16 type, u16 siz, u16 width, u16 height,
 *                      u16 uls, u16 ult, u16 lrs, u16 lrt, u16 pal, s32 tile)
 *
 *   type  0 RGBA (siz 2 = 16b, else 32b) | 2 CI | 5 CI + second 4b tile | 4 I | 3 IA
 *   siz   G_IM_SIZ_* of the texels (0 = 4b, 1 = 8b, 2 = 16b, 3 = 32b)
 *   tile  0: load rows ult..lrt of the image as one block (gDPLoadTextureBlock, with the render
 *            tile's size starting at (uls, ult)); non-zero: gDPLoadTextureTile of the sub-rectangle.
 *
 * Steps: advance img / recompute height (block) or build the rectangle key (tile); choose the
 * render mode from `type` and the render-state word D_8012E608 and apply it through func_800878E0 /
 * func_80086A50; return if (img, key) equals the cache D_8012E684 / D_8012E688; compute the wrap
 * masks with func_80087804 (log2 ceiling); set the TLUT mode (cached in D_8012E680); emit the SDK
 * load macro for (format, size, tile-or-block); store the cache.
 * No arcade ancestor (N64 RDP state cache, same family as func_80086A50 / func_8008705C /
 * func_800878E0 / func_80087110). The 632-byte frame is the one stack slot per macro `Gfx *_g`.
 *
 * Shaping notes (all verified against the scorer):
 *  - SDK GBI macros on `D_80149438++`, exactly as in <PR/gbi.h>; only the block loads differ from
 *    the SDK (gDPLoadTextureBlockAt: tile size starts at (uls, ult), ends at uls+width-1, ult+height-1).
 *  - `s64 D_8012E688;` must be a DEFINITION, not an extern: as1 shares one `lui at` between the
 *    two halves of the 64-bit store only when it knows the symbol's alignment (retail is one unit
 *    in which the variable is defined). With `extern` the tail is one word longer (14 words off).
 *    blob_splice rebinds defined D_ symbols to their image address, so this splices as is.
 *  - The rectangle key is a 64-bit expression truncated into the 32-bit `tile` parameter. Its
 *    natural spelling is
 *        tile = ((s64)uls << 48) + ((s64)ult << 32) + ((s64)lrs << 16) + lrt;
 *    which compiles to the same 2,512 words but calls the IDO runtime helper by its compiler name
 *    `__ll_lshift`; this repository labels that routine (0x8000D994) `__ashldi3`, so the scorer and
 *    the splice link cannot resolve the three `jal`s. The explicit calls below are that expression
 *    with the helper spelled by the repository's label; replace them with the shift form once
 *    `__ll_lshift = 0x8000D994` is known to the symbol table (cloud/work/frontier/w2i/object_render/best.c).
 *  - `uls - lrs` / `ult - lrt` in the tile-mode mask computation are as retail has them (the
 *    differences are <= 0, so both masks are 0 in tile mode).
 *  - `mode` is left unset for a type outside 0,2,3,4,5, as in retail.
 */
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
/* Game variants of gDPLoadTextureBlock / _4b: identical to the SDK macros except that the render tile's
 * size starts at (uls, ult) instead of (0, 0). */
#define gDPLoadTextureBlockAt(pkt, timg, fmt, siz, width, height, uls, ult, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, siz##_LOAD_BLOCK, 1, timg); \
    gDPSetTile(pkt, fmt, siz##_LOAD_BLOCK, 0, 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height) + siz##_INCR) >> siz##_SHIFT) -1, CALC_DXT(width, siz##_BYTES)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, siz, (((width) * siz##_LINE_BYTES)+7)>>3, 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, (uls) << G_TEXTURE_IMAGE_FRAC, (ult) << G_TEXTURE_IMAGE_FRAC, \
        ((uls)+(width)-1) << G_TEXTURE_IMAGE_FRAC, ((ult)+(height)-1) << G_TEXTURE_IMAGE_FRAC) \
}
#define gDPLoadTextureBlockAt_4b(pkt, timg, fmt, width, height, uls, ult, pal, cms, cmt, masks, maskt, shifts, shiftt) \
{ \
    gDPSetTextureImage(pkt, fmt, G_IM_SIZ_16b, 1, timg); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0 , cmt, maskt, shiftt, cms, masks, shifts); \
    gDPLoadSync(pkt); \
    gDPLoadBlock(pkt, G_TX_LOADTILE, 0, 0, (((width)*(height)+3)>>2)-1, CALC_DXT_4b(width)); \
    gDPPipeSync(pkt); \
    gDPSetTile(pkt, fmt, G_IM_SIZ_4b, ((((width)>>1)+7)>>3), 0, G_TX_RENDERTILE, pal, cmt, maskt, shiftt, cms, masks, shifts); \
    gDPSetTileSize(pkt, G_TX_RENDERTILE, (uls) << G_TEXTURE_IMAGE_FRAC, (ult) << G_TEXTURE_IMAGE_FRAC, \
        ((uls)+(width)-1) << G_TEXTURE_IMAGE_FRAC, ((ult)+(height)-1) << G_TEXTURE_IMAGE_FRAC) \
}
extern u32 D_8012E608;
extern s32 D_8012E680;
extern void *D_8012E684;
s64 D_8012E688;
extern Gfx *D_80149438;
extern s32 D_8014A248;
extern void func_80086A50();
extern void func_800878E0();
extern s32 func_80087804();
extern s64 __ashldi3(s64, s64);

#define TILE(fmt, sz) gDPLoadTextureTile(D_80149438++, img, fmt, G_IM_SIZ_##sz, width, height, uls, ult, lrs, lrt, pal, G_TX_CLAMP, G_TX_CLAMP, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)
#define TILE4(fmt) gDPLoadTextureTile_4b(D_80149438++, img, fmt, width, height, uls, ult, lrs, lrt, pal, G_TX_CLAMP, G_TX_CLAMP, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)
#define BLOCK(fmt, sz) gDPLoadTextureBlockAt(D_80149438++, img, fmt, G_IM_SIZ_##sz, width, height, uls, ult, pal, G_TX_CLAMP, G_TX_CLAMP, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)
#define BLOCK4(fmt) gDPLoadTextureBlockAt_4b(D_80149438++, img, fmt, width, height, uls, ult, pal, G_TX_CLAMP, G_TX_CLAMP, masks, maskt, G_TX_NOLOD, G_TX_NOLOD)

void object_render(void *img, u16 type, u16 siz, u16 width, u16 height, u16 uls, u16 ult, u16 lrs, u16 lrt,
                   u16 pal, s32 tile)
{
    s32 mode;
    u16 masks;
    u16 maskt;

    if (tile == 0) {
        if (siz == 3) {
            img = (u32 *)img + ult * width;
        }
        else if (siz == 2) {
            img = (u16 *)img + ult * width;
        }
        else if (siz == 1) {
            img = (u8 *)img + ult * width;
        }
        else {
            img = (u8 *)img + ult * width / 2;
        }
        height = lrt - ult + 1;
    }
    else {
        tile = __ashldi3(uls, 48) + __ashldi3(ult, 32) + __ashldi3(lrs, 16) + lrt;
    }
    if (type == 0 || type == 2) {
        if ((D_8012E608 & 0x20) || (D_8012E608 & 0x10) || (D_8012E608 & 0x8000)) {
            mode = 1;
        }
        else {
            mode = 0;
        }
    }
    else if (type == 5) {
        mode = 4;
    }
    else if (type == 4 || type == 3) {
        if (D_8012E608 & 1) {
            mode = 3;
        }
        else {
            mode = 2;
        }
    }
    if (type == 3) {
        func_800878E0(0x20);
    }
    if (mode != D_8014A248) {
        func_80086A50(mode);
    }
    if (img == D_8012E684 && D_8012E688 == tile) {
        return;
    }
    if (tile != 0) {
        masks = func_80087804(uls - lrs);
        maskt = func_80087804(ult - lrt);
    }
    else {
        masks = func_80087804(width);
        maskt = func_80087804(height);
    }
    if (type == 0) {
        if (D_8012E680 != 0) {
            gDPSetTextureLUT(D_80149438++, G_TT_NONE);
            D_8012E680 = 0;
        }
        if (siz == 2) {
            if (tile != 0) {
                TILE(G_IM_FMT_RGBA, 16b);
            }
            else {
                BLOCK(G_IM_FMT_RGBA, 16b);
            }
        }
        else {
            if (tile != 0) {
                TILE(G_IM_FMT_RGBA, 32b);
            }
            else {
                BLOCK(G_IM_FMT_RGBA, 32b);
            }
        }
    }
    else if (type == 2 || type == 5) {
        if (D_8012E680 != 1) {
            gDPSetTextureLUT(D_80149438++, G_TT_RGBA16);
            D_8012E680 = 1;
        }
        if (siz == 1) {
            if (tile != 0) {
                TILE(G_IM_FMT_CI, 8b);
            }
            else {
                BLOCK(G_IM_FMT_CI, 8b);
            }
            if (type == 5) {
                gDPSetTile(D_80149438++, G_IM_FMT_CI, G_IM_SIZ_4b, ((width + 7) >> 3), 0, 1, pal, G_TX_CLAMP, maskt,
                           G_TX_NOLOD, G_TX_CLAMP, masks + 1, 15);
                gDPSetTileSize(D_80149438++, 1, uls << G_TEXTURE_IMAGE_FRAC, ult << G_TEXTURE_IMAGE_FRAC,
                               (uls + width - 1) << G_TEXTURE_IMAGE_FRAC, (ult + height - 1) << G_TEXTURE_IMAGE_FRAC);
            }
        }
        else {
            if (tile != 0) {
                TILE4(G_IM_FMT_CI);
            }
            else {
                BLOCK4(G_IM_FMT_CI);
            }
        }
    }
    else if (type == 4) {
        if (D_8012E680 != 0) {
            gDPSetTextureLUT(D_80149438++, G_TT_NONE);
            D_8012E680 = 0;
        }
        if (siz == 1) {
            if (tile != 0) {
                TILE(G_IM_FMT_I, 8b);
            }
            else {
                BLOCK(G_IM_FMT_I, 8b);
            }
        }
        else {
            if (tile != 0) {
                TILE4(G_IM_FMT_I);
            }
            else {
                BLOCK4(G_IM_FMT_I);
            }
        }
    }
    else if (type == 3) {
        if (D_8012E680 != 0) {
            gDPSetTextureLUT(D_80149438++, G_TT_NONE);
            D_8012E680 = 0;
        }
        if (siz == 2) {
            if (tile != 0) {
                TILE(G_IM_FMT_IA, 16b);
            }
            else {
                BLOCK(G_IM_FMT_IA, 16b);
            }
        }
        else if (siz == 1) {
            if (tile != 0) {
                TILE(G_IM_FMT_IA, 8b);
            }
            else {
                BLOCK(G_IM_FMT_IA, 8b);
            }
        }
        else {
            if (tile != 0) {
                TILE4(G_IM_FMT_IA);
            }
            else {
                BLOCK4(G_IM_FMT_IA);
            }
        }
    }
    D_8012E684 = img;
    D_8012E688 = tile;
}
