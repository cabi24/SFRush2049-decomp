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
    u32 unk20;
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
extern s32 D_8017A638;
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


/* ---- F3DEX2 forms (reference/repos/ultralib/include/PR/gbi.h, F3DEX_GBI_2) ---- */
#define G_VTX 0x01
#define G_CULLDL 0x03
#define G_TRI1 0x05
#define G_TRI2 0x06
#define G_TEXTURE 0xD7
#define G_POPMTX 0xD8
#define G_GEOMETRYMODE 0xD9
#define G_MTX 0xDA
#define G_MOVEWORD 0xDB
#define G_MOVEMEM 0xDC
#define G_ENDDL 0xDF
#define G_SETOTHERMODE_L 0xE2
#define G_SETPRIMDEPTH 0xEE
#define G_SETSCISSOR 0xED
#define G_FILLRECT 0xF6
#define G_SETFOGCOLOR 0xF8
#define G_SETPRIMCOLOR 0xFA
#define G_SETENVCOLOR 0xFB
#define G_SETCOMBINE 0xFC
#define G_DL_NOPUSH 0x01
#define G_MV_VIEWPORT 8
#define G_MV_LIGHT 10
#define G_MVO_LOOKATX 0
#define G_MVO_LOOKATY 24
#define G_MW_NUMLIGHT 0x02
#define G_MW_FOG 0x08
#define G_MW_PERSPNORM 0x0E
#define G_MWO_NUMLIGHT 0x00
#define G_MWO_FOG 0x00
#define NUML(n) ((n) * 24)
#define NUMLIGHTS_0 1
#define NUMLIGHTS_1 1
#define NUMLIGHTS_2 2
#define G_MTX_NOPUSH 0x00
#define G_MTX_PUSH 0x01
#define G_MTX_MUL 0x00
#define G_MTX_LOAD 0x02
#define G_MTX_MODELVIEW 0x00
#define G_MTX_PROJECTION 0x04
#define G_FOG 0x00010000
#define G_LIGHTING 0x00020000
#define G_TEXTURE_GEN 0x00040000
#define G_MDSFT_CYCLETYPE 20
#define G_MDSFT_ZSRCSEL 2
#define G_MDSFT_RENDERMODE 3
#define G_CYC_1CYCLE (0 << G_MDSFT_CYCLETYPE)
#define G_CYC_2CYCLE (1 << G_MDSFT_CYCLETYPE)
#define G_ZS_PIXEL (0 << G_MDSFT_ZSRCSEL)
#define G_ZS_PRIM (1 << G_MDSFT_ZSRCSEL)
#define G_ON 1
#define G_SC_NON_INTERLACE 0
#define BOWTIE_VAL 0

#define gDma2p(pkt, c, adrs, len, idx, ofs) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL((c), 24, 8) | _SHIFTL(((len) - 1) / 8, 19, 5) | _SHIFTL((ofs) / 8, 8, 8) | _SHIFTL((idx), 0, 8)); \
    _g->words.w1 = (unsigned int)(adrs); \
}
#define gSPBranchList(pkt, dl) gDma1p(pkt, G_DL, dl, 0, G_DL_NOPUSH)
#define gSPEndDisplayList(pkt) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_ENDDL, 24, 8); \
    _g->words.w1 = 0; \
}
#define gSPMatrix(pkt, m, p) gDma2p((pkt), G_MTX, (m), sizeof(Mtx), (p) ^ G_MTX_PUSH, 0)
#define gSPPopMatrixN(pkt, n, num) gDma2p((pkt), G_POPMTX, (num) * 64, 64, 2, 0)
#define gSPPopMatrix(pkt, n) gSPPopMatrixN((pkt), (n), 1)
#define gSPViewport(pkt, v) gDma2p((pkt), G_MOVEMEM, (v), sizeof(Vp), G_MV_VIEWPORT, 0)
#define gSPVertex(pkt, v, n, v0) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_VTX, 24, 8) | _SHIFTL((n), 12, 8) | _SHIFTL((v0) + (n), 1, 7); \
    _g->words.w1 = (unsigned int)(v); \
}
#define gSPCullDisplayList(pkt, vstart, vend) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_CULLDL, 24, 8) | _SHIFTL((vstart) * 2, 0, 16); \
    _g->words.w1 = _SHIFTL((vend) * 2, 0, 16); \
}
#define __gsSP1Triangle_w1(v0, v1, v2) (_SHIFTL((v0) * 2, 16, 8) | _SHIFTL((v1) * 2, 8, 8) | _SHIFTL((v2) * 2, 0, 8))
#define __gsSP1Triangle_w1f(v0, v1, v2, flag) \
    (((flag) == 0) ? __gsSP1Triangle_w1(v0, v1, v2) : \
     ((flag) == 1) ? __gsSP1Triangle_w1(v1, v2, v0) : \
                     __gsSP1Triangle_w1(v2, v0, v1))
#define gSP1Triangle(pkt, v0, v1, v2, flag) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_TRI1, 24, 8) | __gsSP1Triangle_w1f(v0, v1, v2, flag); \
    _g->words.w1 = 0; \
}
#define gSP2Triangles(pkt, v00, v01, v02, flag0, v10, v11, v12, flag1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_TRI2, 24, 8) | __gsSP1Triangle_w1f(v00, v01, v02, flag0)); \
    _g->words.w1 = __gsSP1Triangle_w1f(v10, v11, v12, flag1); \
}
#define gMoveWd(pkt, index, offset, data) gDma1p((pkt), G_MOVEWORD, data, offset, index)
#define gSPNumLights(pkt, n) gMoveWd(pkt, G_MW_NUMLIGHT, G_MWO_NUMLIGHT, NUML(n))
#define gSPLight(pkt, l, n) gDma2p((pkt), G_MOVEMEM, (l), sizeof(Light), G_MV_LIGHT, (n) * 24 + 24)
#define gSPSetLights0(pkt, name) \
{ \
    gSPNumLights(pkt, NUMLIGHTS_0); \
    gSPLight(pkt, &name.l[0], 1); \
    gSPLight(pkt, &name.a, 2); \
}
#define gSPSetLights1(pkt, name) \
{ \
    gSPNumLights(pkt, NUMLIGHTS_1); \
    gSPLight(pkt, &name.l[0], 1); \
    gSPLight(pkt, &name.a, 2); \
}
#define gSPSetLights2(pkt, name) \
{ \
    gSPNumLights(pkt, NUMLIGHTS_2); \
    gSPLight(pkt, &name.l[0], 1); \
    gSPLight(pkt, &name.l[1], 2); \
    gSPLight(pkt, &name.a, 3); \
}
#define gSPLookAtX(pkt, l) gDma2p((pkt), G_MOVEMEM, (l), sizeof(Light), G_MV_LIGHT, G_MVO_LOOKATX)
#define gSPLookAtY(pkt, l) gDma2p((pkt), G_MOVEMEM, (l), sizeof(Light), G_MV_LIGHT, G_MVO_LOOKATY)
#define gSPLookAt(pkt, la) \
{ \
    gSPLookAtX(pkt, la) \
    gSPLookAtY(pkt, (char *)(la) + 16) \
}
#define gSPGeometryMode(pkt, c, s) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_GEOMETRYMODE, 24, 8) | _SHIFTL(~(u32)(c), 0, 24); \
    _g->words.w1 = (u32)(s); \
}
#define gSPSetGeometryMode(pkt, word) gSPGeometryMode((pkt), 0, (word))
#define gSPClearGeometryMode(pkt, word) gSPGeometryMode((pkt), (word), 0)
#define gSPTexture(pkt, s, t, level, tile, on) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_TEXTURE, 24, 8) | _SHIFTL(BOWTIE_VAL, 16, 8) | _SHIFTL((level), 11, 3) | \
                    _SHIFTL((tile), 8, 3) | _SHIFTL((on), 1, 7)); \
    _g->words.w1 = (_SHIFTL((s), 16, 16) | _SHIFTL((t), 0, 16)); \
}
#define gSPPerspNormalize(pkt, s) gMoveWd(pkt, G_MW_PERSPNORM, 0, (s))
#define gSPFogFactor(pkt, fm, fo) gMoveWd(pkt, G_MW_FOG, G_MWO_FOG, (_SHIFTL(fm, 16, 16) | _SHIFTL(fo, 0, 16)))
#define gSPFogPosition(pkt, min, max) \
    gSPFogFactor(pkt, (500 * 128 / ((max) - (min))), (500 - (min)) * 256 / ((max) - (min)))
#define gDPSetCycleType(pkt, type) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_CYCLETYPE, 2, type)
#define gDPSetDepthSource(pkt, src) gSPSetOtherMode(pkt, G_SETOTHERMODE_L, G_MDSFT_ZSRCSEL, 1, src)
#define gDPSetRenderMode(pkt, c0, c1) gSPSetOtherMode(pkt, G_SETOTHERMODE_L, G_MDSFT_RENDERMODE, 29, (c0) | (c1))
#define gDPSetCombine(pkt, muxs0, muxs1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_SETCOMBINE, 24, 8) | _SHIFTL(muxs0, 0, 24); \
    _g->words.w1 = (unsigned int)(muxs1); \
}
#define gDPSetColor(pkt, c, d) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(c, 24, 8); \
    _g->words.w1 = (unsigned int)(d); \
}
#define DPRGBColor(pkt, cmd, r, g, b, a) \
    gDPSetColor(pkt, cmd, (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)))
#define gDPSetEnvColor(pkt, r, g, b, a) DPRGBColor(pkt, G_SETENVCOLOR, r, g, b, a)
#define gDPSetFogColor(pkt, r, g, b, a) DPRGBColor(pkt, G_SETFOGCOLOR, r, g, b, a)
#define gDPSetPrimColor(pkt, m, l, r, g, b, a) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_SETPRIMCOLOR, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)); \
    _g->words.w1 = (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); \
}
#define gDPSetPrimDepth(pkt, z, dz) gDPSetColor(pkt, G_SETPRIMDEPTH, _SHIFTL(z, 16, 16) | _SHIFTL(dz, 0, 16))
#define gDPSetScissor(pkt, mode, ulx, uly, lrx, lry) \
{ \
    Gfx *_g = (Gfx *)pkt; \
    _g->words.w0 = _SHIFTL(G_SETSCISSOR, 24, 8) | _SHIFTL((int)((float)(ulx) * 4.0F), 12, 12) | \
                   _SHIFTL((int)((float)(uly) * 4.0F), 0, 12); \
    _g->words.w1 = _SHIFTL(mode, 24, 2) | _SHIFTL((int)((float)(lrx) * 4.0F), 12, 12) | \
                   _SHIFTL((int)((float)(lry) * 4.0F), 0, 12); \
}
#define gDPFillRectangle(pkt, ulx, uly, lrx, lry) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(G_FILLRECT, 24, 8) | _SHIFTL((lrx), 14, 10) | _SHIFTL((lry), 2, 10)); \
    _g->words.w1 = (_SHIFTL((ulx), 14, 10) | _SHIFTL((uly), 2, 10)); \
}

/* ---- types ---- */
typedef s32 Mtx_t[4][4];
typedef union {
    Mtx_t m;
    long long int force_structure_alignment;
} Mtx;
typedef struct {
    short vscale[4];
    short vtrans[4];
} Vp_t;
typedef union {
    Vp_t vp;
    long long int force_structure_alignment[2];
} Vp;
typedef struct {
    short ob[3];
    unsigned short flag;
    short tc[2];
    unsigned char cn[4];
} Vtx_t;
typedef union {
    Vtx_t v;
    long long int force_structure_alignment;
} Vtx;
typedef struct {
    unsigned char col[3];
    char pad1;
    unsigned char colc[3];
    char pad2;
    signed char dir[3];
    char pad3;
} Light_t;
typedef union {
    Light_t l;
    long long int force_structure_alignment[2];
} Light;
typedef struct {
    unsigned char col[3];
    char pad1;
    unsigned char colc[3];
    char pad2;
} Ambient_t;
typedef union {
    Ambient_t l;
    long long int force_structure_alignment[1];
} Ambient;
typedef struct {
    Ambient a;
    Light l[2];
} Lights2;
typedef struct {
    Light l[2];
} LookAt;
typedef f32 MtxF[4][4];

/* per-view graphics buffer (*D_8015B250) */
typedef struct {
    Vp vp[4];       /* 0x000 */
    Vtx vtx[4];     /* 0x040 */
    Mtx proj[4];    /* 0x080 */
    Mtx look[4];    /* 0x180 */
    LookAt la[32];  /* 0x280 */
    Mtx ident;      /* 0x680 */
} GfxBuf;

/* view record D_80150B70[] (0x98 bytes) */
typedef struct {
    f32 rot[3][3];
    f32 pos[3];
    u8 pad30[0x98 - 0x30];
} View;

/* LOD level of a model; the set's levels start at +20 (count is the s16 at +22) */
typedef struct {
    s16 unk0;
    s16 count;
    u16 tex;
    u16 flags;
    f32 dist;
    Gfx *dl;
} LodLevel;
typedef struct {
    u8 pad0[20];
    LodLevel lv[4];
    u8 pad54[4];
} LodSet; /* 88 bytes */

/* scene node D_8012E700[] (0x44 bytes) */
typedef struct Node {
    u32 flags;
    u32 lights;
    View *obj;
    f32 scale;
    f32 w;
    u16 model;
    s16 child;
    s16 next;
    s16 mode;
    Lights2 *light;
    Texture *tex[4];
    TexRect *rect;
    TexRect *clip;
    Palette *pal;
    u8 prim[4];
    u8 env[4];
} Node;

typedef struct {
    LodSet *sets;
    s32 count;
} LodTable;
typedef struct {
    Texture *tex;
    s32 count;
} TexTable;
typedef struct {
    Palette *pal;
    s32 count;
} PalTable;

extern GfxBuf *D_8015B250;
extern Mtx *D_8015B260;
extern Node D_8012E700[];
extern View D_80150B70[];
extern LodTable D_801161F4[];
extern TexTable D_80151AE8[];
extern PalTable D_80138670[];
extern f32 D_80124EE8;
extern f32 D_80124EF0[4][4];
extern f32 D_80124F30[4][4];
extern f32 D_801403D8[4][4];
extern f32 D_80124F78[3];
extern s8 D_80124F84;
extern s16 D_80124FC4;
extern s16 D_80124FCC;
extern s16 D_80124FE0;
extern s16 D_80124FE2;
extern Texture *D_8017A508;
extern Vtx D_80157248[];
extern Vtx D_8011EF10[];
extern Vtx D_8011EF90[];
extern f32 D_80151AA0;

void camera_update_d(LookAt *l, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt,
                     f32 xUp, f32 yUp, f32 zUp);
f32 func_8008E0B8(f32 *v);
f32 func_8008C768(f32 y, f32 x);
s32 track_collision_edge(Node *node, s32 view);
s32 func_8009D99C(s32 idx, f32 (*m)[4], f32 *pos, Mtx *out, f32 w, s32 absolute);
void func_8009D708(s32 idx, View *obj, Mtx *m, f32 scale, s32 absolute);
s32 func_8009D45C(s32 idx, View *src, Mtx *out, f32 w, s32 absolute);
float sqrtf(float);
#pragma intrinsic(sqrtf)
float fabsf(float);
#pragma intrinsic(fabsf)
extern Gfx *D_80161430;
s32 particle_system(Gfx **gp, Node *parent, s32 count, s32 flag, Node *node, s32 view, Palette *pal);
s32 track_collision_wall(Gfx **gp, Node *parent, Node *node, s32 view, s32 count, s32 more_in, s32 flags6, Palette *pal);
f32 func_8009C3F8(s32 arg0, f32 input);
void render_display_list(Texture *tex, TexRect *rect, TexRect *clip, Gfx **gp, Palette *palettes);
void func_80099B30(Gfx **gfxp, Palette *tex);
#define gDPSetTileSizeS(pkt, t, uls, ult, lrs, lrt) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (G_SETTILESIZE << 24) | (((uls) & 0xFFF) << 12) | ((ult) & 0xFFF); \
    _g->words.w1 = (((t) & 7) << 24) | (((lrs) & 0xFFF) << 12) | ((lrt) & 0xFFF); \
}
extern s32 D_8017A4B0;

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
    u16 masks, maskt;
    u16 cms, cmt;
    Gfx *gfx = *gp;

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
            if (D_8017A638 != 0) {
                gDPSetTextureLUT(gfx++, G_TT_NONE);
                D_8017A638 = 0;
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
            if (D_8017A638 != 1) {
                gDPSetTextureLUT(gfx++, G_TT_RGBA16);
                D_8017A638 = 1;
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
            if (D_8017A638 != 0) {
                gDPSetTextureLUT(gfx++, G_TT_NONE);
                D_8017A638 = 0;
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
            if (D_8017A638 != 0) {
                gDPSetTextureLUT(gfx++, G_TT_NONE);
                D_8017A638 = 0;
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
        gDPSetTileSizeS(gfx++, G_TX_RENDERTILE, clip->x0 >> 3, clip->y0 >> 3, clip->x1 >> 3, clip->y1 >> 3);
    } else if (rect != NULL) {
        gfx--;
        gDPSetTileSizeS(gfx++, G_TX_RENDERTILE, rect->x0 >> 3, rect->y0 >> 3, rect->x1 >> 3, rect->y1 >> 3);
    }
    if (tex->palette >= 0 && !(tex->flags & TEX_NOPALETTE) && palettes != NULL) {
        func_80099B30(&gfx, &palettes[tex->palette]);
    }
    *gp = gfx;
}

extern f32 D_8011F010[4];
f32 func_8009C3F8(s32 arg0, f32 input)
{
    f32 g;
    f32 y;
    f32 r;
    s32 i;

    y = fabsf(input);
    i = arg0;
    if (y < 2.3e-10f) {
        r = y;
    } else if (y >= 1.0f) {
        r = 1.5707964f;
    } else {
        if (y > 0.5f) {
            i = 1 - arg0;
            g = ((0.5f - y) + 0.5f) * 0.5f;
            y = sqrtf(g);
            y = -(y + y);
        } else {
            g = y * y;
        }
        r = (((((-0.69674575f * g + 10.152522f) * g + -39.688862f) * g + 57.20823f) * g + -27.368494f) * g) /
            (((((g + -23.823858f) * g + 150.95271f) * g + -381.86304f) * g + 417.14432f) * g + -164.21097f) * y + y;
    }
    if (arg0 != 0) {
        if (input < 0.0f) {
            return D_8011F010[i + 2] + (D_8011F010[i + 2] + r);
        }
        return D_8011F010[i] + (D_8011F010[i] - r);
    }
    r = D_8011F010[i] + (D_8011F010[i] + r);
    if (input < 0.0f) {
        r = -r;
    }
    return r;
}

s32 particle_system(Gfx **gp, Node *parent, s32 count, s32 flag, Node *node, s32 view, Palette *pal)
{
    Gfx *gfx = *gp;
    s32 drawn = 0;
    LodSet *set;
    s32 lod;
    Gfx *dl;
    View *v;
    TexRect rect;
    f32 d[3];
    f32 dir[3];
    Palette *pals;
    u8 alpha;
    f32 x, y, z;

    set = &D_801161F4[node->model >> 10].sets[node->model & 0x3FF];
    lod = set->lv[0].count - 1;
    if (node->flags & 4) {
        lod = node->flags & 3;
        if (lod == 0) {
            D_80124EE8 = 0.0f;
        } else {
            D_80124EE8 = set->lv[lod - 1].dist;
        }
    } else if ((set->lv[lod].dist != 0.0f && !(node->flags & (0x100 << view)) && !flag) || (node->flags & 8)) {
        if (count < 2) {
            if (count > 0) {
                d[0] = parent->obj->pos[0] + node->obj->pos[0];
                d[1] = parent->obj->pos[1] + node->obj->pos[1];
                d[2] = parent->obj->pos[2] + node->obj->pos[2];
                v = &D_80150B70[view];
                d[0] = d[0] - v->pos[0];
                d[1] = d[1] - v->pos[1];
                d[2] = d[2] - v->pos[2];
            } else {
                v = &D_80150B70[view];
                d[0] = node->obj->pos[0] - v->pos[0];
                d[1] = node->obj->pos[1] - v->pos[1];
                d[2] = node->obj->pos[2] - v->pos[2];
            }
        }
        x = d[0];
        y = d[1];
        z = d[2];
        D_80124EE8 = sqrtf(x * x + y * y + z * z);
    }
    if (!(node->flags & (0x100 << view))) {
        if (set->lv[lod].dist == 0.0f || !(set->lv[lod].dist * node->scale < D_80124EE8)) {
            if (node->flags & 4) {
                lod = node->flags & 3;
                if (lod >= set->lv[0].count) {
                    lod = set->lv[0].count - 1;
                }
            } else if (lod > 0) {
                while (lod != 0 && D_80124EE8 < set->lv[lod - 1].dist * node->scale) {
                    lod--;
                }
            }
            dl = set->lv[lod].dl;
            if (dl != NULL) {
                if (node->flags & 0x80000) {
                    v = &D_80150B70[view];
                    camera_update_d(&D_8015B250->la[D_80124F84], -v->pos[0], v->pos[1], v->pos[2],
                                    -D_80124F78[0], D_80124F78[1], D_80124F78[2], 0.0f, 1.0f, 0.0f);
                    gSPLookAt(gfx++, &D_8015B250->la[D_80124F84]);
                    gSPSetGeometryMode(gfx++, G_TEXTURE_GEN);
                    D_80124F84++;
                    gSPClearGeometryMode(gfx++, G_FOG);
                    gDPPipeSync(gfx++);
                    gDPSetCycleType(gfx++, G_CYC_1CYCLE);
                    if (node->tex[lod] != NULL) {
                        gSPTexture(gfx++, node->tex[lod]->width << 7, node->tex[lod]->height << 6, 0,
                                   G_TX_RENDERTILE, G_ON);
                    }
                    dir[0] = D_80124F78[0] - v->pos[0];
                    dir[1] = D_80124F78[1] - v->pos[1];
                    dir[2] = D_80124F78[2] - v->pos[2];
                    func_8008E0B8(dir);
                    rect.x0 = -func_8008C768(dir[0], dir[2]) * 1303.7972f + 6144.0f;
                    rect.y0 = -func_8009C3F8(0, dir[1]) * 1303.7972f + 2048.0f;
                    rect.x1 = rect.x0 + 4064;
                    rect.y1 = rect.y0 + 2016;
                    node->clip = &rect;
                }
                if (set->lv[lod].flags & 0x10) {
                    gSPSetGeometryMode(gfx++, G_LIGHTING);
                }
                if (node->pal != NULL) {
                    func_80099B30(&gfx, node->pal);
                }
                if (set->lv[lod].flags & 1) {
                    pals = (node->pal != NULL) ? NULL : D_80138670[node->model >> 10].pal;
                    if (node->tex[lod] != NULL) {
                        render_display_list(node->tex[lod], node->rect, node->clip, &gfx, pals);
                    } else if (set->lv[lod].flags & 0x8000) {
                        render_display_list(&D_80151AE8[set->lv[lod].tex >> 10].tex[set->lv[lod].tex & 0x3FF], node->rect,
                                            node->clip, &gfx, pals);
                    }
                } else if (node->clip != NULL) {
                    gDPSetTileSize(gfx++, G_TX_RENDERTILE, node->clip->x0 >> 3, node->clip->y0 >> 3,
                                   node->clip->x1 >> 3, node->clip->y1 >> 3);
                } else if (node->rect != NULL) {
                    gDPSetTileSize(gfx++, G_TX_RENDERTILE, node->rect->x0 >> 3, node->rect->y0 >> 3,
                                   node->rect->x1 >> 3, node->rect->y1 >> 3);
                }
                if ((set->lv[lod].flags & 4) || (node->flags & 0x2000)) {
                    alpha = node->prim[3];
                    if ((node->flags & 0x200000) && D_80124EE8 > 24.0f) {
                        if (D_80124EE8 > 80.0f) {
                            alpha = 0;
                        } else {
                            alpha = alpha * (80.0f - D_80124EE8) / 56.0f;
                        }
                    }
                    gDPSetPrimColor(gfx++, 0, 0, node->prim[0], node->prim[1], node->prim[2], alpha);
                }
                if (node->flags & 0x4000) {
                    gDPSetEnvColor(gfx++, node->env[0], node->env[1], node->env[2], node->env[3]);
                }
                gSPDisplayList(gfx++, dl);
                if (D_8017A638 == 0) {
                    gDPSetTextureLUT(gfx++, G_TT_RGBA16);
                    D_8017A638 = 1;
                }
                if (set->lv[lod].flags & 2) {
                    D_8017A4B0 = 0;
                    if (pal != NULL) {
                        func_80099B30(&gfx, pal);
                    }
                }
                drawn = 1;
                if (node->flags & 0x80000) {
                    gDPPipeSync(gfx++);
                    gDPSetCycleType(gfx++, G_CYC_2CYCLE);
                    gSPSetGeometryMode(gfx++, G_FOG);
                    gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
                    gSPClearGeometryMode(gfx++, G_TEXTURE_GEN);
                    node->clip = NULL;
                }
                if (set->lv[lod].flags & 0x10) {
                    gSPClearGeometryMode(gfx++, G_LIGHTING);
                }
            }
        }
    }
    *gp = gfx;
    return drawn;
}

s32 track_collision_wall(Gfx **gp, Node *parent, Node *node, s32 view, s32 count, s32 more_in, s32 flags6,
                         Palette *pal)
{
    Gfx *gfx = *gp;
    s32 more;
    s32 pushed = 0;
    s32 total = 0;
    s32 saved = 0;
    s32 i, j;
    s32 n;
    Vtx *vtx;
    View *v;
    f32 x, z, r;

    for (;;) {
        if (!(node->flags & 0x80000000)) {
            if (node->next >= 0 || more_in) {
                more = 1;
            } else {
                more = 0;
            }
            if (D_80124FC4 == 0 && (node->flags & 0x40000)) {
                if (D_80124FCC < 0) {
                    D_80124FCC = node - D_8012E700;
                }
            } else if (D_80124FE0 == 0 && (node->flags & 0x800000)) {
                if (D_80124FE2 < 0) {
                    D_80124FE2 = node - D_8012E700;
                }
            } else if (track_collision_edge(node, view)) {
                if (node->child < 0 && (node->flags & (0x100 << view))) {
                    more = 0;
                    goto children;
                }
                if (node->flags & 0x1000) {
                    gSPVertex(gfx++, &D_8015B250->vtx[view], 1, 8);
                }
                if (node->flags & 0x80) {
                    more = 0;
                } else {
                    D_80124F78[0] = node->obj->pos[0];
                    D_80124F78[1] = node->obj->pos[1];
                    D_80124F78[2] = node->obj->pos[2];
                    if (node->flags & 0x10000) {
                        if (node->scale == 1.0f) {
                            if (!func_8009D99C(view, D_80124EF0, node->obj->pos, D_8015B260, node->w, count)) {
                                goto next;
                            }
                        } else {
                            for (i = 0; i < 3; i++) {
                                for (j = 0; j < 3; j++) {
                                    D_801403D8[i][j] = D_80124EF0[i][j] * node->scale;
                                }
                            }
                            for (i = 0; i < 3; i++) {
                                D_801403D8[3][i] = D_80124EF0[3][i];
                                D_801403D8[i][3] = D_80124EF0[i][3];
                            }
                            if (!func_8009D99C(view, D_801403D8, node->obj->pos, D_8015B260, node->w, count)) {
                                goto next;
                            }
                        }
                    } else if (node->flags & 0x8000) {
                        if (!func_8009D99C(view, D_80124F30, node->obj->pos, D_8015B260, node->w, count)) {
                            goto next;
                        }
                    } else if (node->flags & 0x400000) {
                        func_8009D708(view, node->obj, D_8015B260, node->w, count);
                    } else if (!func_8009D45C(view, node->obj, D_8015B260, node->w, count)) {
                        goto next;
                    }
                    gSPMatrix(gfx++, D_8015B260, more);
                    D_8015B260++;
                }
                if (node->mode >= 0) {
                    switch (node->mode) {
                        case 0:
                            gSPSetLights0(gfx++, (*node->light));
                            break;
                        case 1:
                            gSPSetLights1(gfx++, (*node->light));
                            break;
                        case 2:
                            gSPSetLights2(gfx++, (*node->light));
                            break;
                    }
                }
                if ((node->flags & 0x10) || (node->flags & 0x20)) {
                    D_80161430 = gfx + 2;
                    gSPDisplayList(gfx++, D_80161430);
                    gSPBranchList(gfx++, NULL);
                    pushed = 1;
                    saved = total;
                    total = 0;
                    if (node->flags & 0x10) {
                        gSPVertex(gfx++, D_8011EF10, 8, 0);
                    } else {
                        gSPVertex(gfx++, D_8011EF90, 8, 0);
                    }
                    gSPCullDisplayList(gfx++, 0, 7);
                } else {
                    n = node->lights & 7;
                    if (n != 0) {
                        vtx = &D_80157248[(node->lights & 0x1FF8) >> 3];
                        if (!(node->flags & 0x400000) && D_80151AA0 < 2000.0f) {
                            v = &D_80150B70[view];
                            x = (vtx[0].v.ob[0] < vtx[7].v.ob[0]) ? vtx[7].v.ob[0] : vtx[0].v.ob[0];
                            z = (vtx[0].v.ob[2] < vtx[7].v.ob[2]) ? vtx[7].v.ob[2] : vtx[0].v.ob[2];
                            r = sqrtf(x * x + z * z) * 0.0625f;
                            x = node->obj->pos[0] - v->pos[0];
                            z = node->obj->pos[2] - v->pos[2];
                            if (D_80151AA0 < sqrtf(x * x + z * z) - r) {
                                goto children;
                            }
                        }
                        D_80161430 = gfx + 2;
                        gSPDisplayList(gfx++, D_80161430);
                        gSPBranchList(gfx++, NULL);
                        D_8017A4B0 = 0;
                        pushed = 1;
                        D_8017A508 = NULL;
                        saved = total;
                        gSPVertex(gfx++, vtx, n + 1, 0);
                        gSPCullDisplayList(gfx++, 0, n);
                        total = 0;
                    }
                }
                total += particle_system(&gfx, parent, count, flags6, node, view,
                                         (node->pal != NULL) ? node->pal : pal);
            children:
                if (node->child >= 0) {
                    total += track_collision_wall(&gfx, node, &D_8012E700[node->child], view,
                                                  (more == 1) ? count + 1 : count, node->next >= 0,
                                                  (node->flags & 8) | flags6,
                                                  (node->pal != NULL) ? node->pal : pal);
                }
                if (pushed) {
                    pushed = 0;
                    if (total == 0) {
                        gfx = D_80161430 - 2;
                    } else {
                        gSPEndDisplayList(gfx++);
                        D_80161430[-1].words.w1 = (u32)gfx;
                    }
                    D_80161430 = NULL;
                    total += saved;
                    saved = 0;
                }
                if (more == 1) {
                    if (total == 0) {
                        gfx--;
                    } else {
                        gSPPopMatrix(gfx++, G_MTX_MODELVIEW);
                    }
                }
            }
        }
    next:
        do {
            if (node->next < 0) {
                *gp = gfx;
                return total;
            }
            node = &D_8012E700[node->next];
        } while ((D_80124FC4 != 0 && !(node->flags & 0x40000)) || (D_80124FE0 != 0 && !(node->flags & 0x800000)));
    }
}

typedef struct {
    Vp *vp;          /* 0x00 */
    u16 *scissor;    /* 0x04 */
    s32 ortho;       /* 0x08 */
    u8 pad0C[4];
    f32 fov;         /* 0x10 */
    u8 pad14[0x10];
    f32 w;           /* 0x24 */
    f32 h;           /* 0x28 */
    f32 x;           /* 0x2C */
    f32 y;           /* 0x30 */
    f32 aspect;      /* 0x34 */
    f32 near;        /* 0x38 */
    f32 far;         /* 0x3C */
    u16 fogmin;      /* 0x40 */
    u16 fogmax;      /* 0x42 */
    u8 fog[4];       /* 0x44 */
} Camera; /* 0x48 */

typedef struct {
    f32 pos[3];
    s16 tc[2];
    u8 cn[4];
} PolyVtx; /* 20 bytes */

typedef struct {
    s16 count;       /* 0x00 */
    u16 flags;       /* 0x02 */
    u16 tex;         /* 0x04 */
    s16 z;           /* 0x06 */
    union {
        PolyVtx v[4];
        struct {
            f32 x0, y0;      /* 0x08 */
            u8 pad10[8];
            u8 col[4];       /* 0x18 */
            f32 x1, y1;      /* 0x1C */
        } rect;
    } u;
} Poly; /* 0x58 */

extern Gfx *D_801497C8;
extern Camera D_8017A510[];
extern s8 D_80151AD8;
extern s8 D_80140A04;
extern f32 D_801613B8;
extern MtxF D_801613F0;
extern s16 D_8014A108;
extern s16 D_8015B254;
extern s32 D_801613B4;
extern Poly D_8015B268[];
extern Vtx *D_80161434;

void physics_float_calc(s32 view, f32 *pos, s32 arg2);
void *memcpy(void *, const void *, unsigned int);
void math_utility(void *arg0, void *arg1);
void func_8009EA68(f32 angle, f32 uv[][3]);
void func_8009E9D8(f32 *m);
void func_8009E8B4(f32 matrix[4][4], u32 *fixed);
void func_8009E820(f32 *arg0, f32 *arg1, f32 *arg2);
void guOrtho(Mtx *m, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f, f32 scale);
void guPerspective(Mtx *m, u16 *perspNorm, f32 fovy, f32 aspect, f32 near, f32 far, f32 scale);
void guLookAtF(f32 mf[4][4], f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp,
               f32 zUp);
void guMtxIdent(Mtx *m);

void func_8009F058(s32 view, s32 arg1, s32 arg2, View *eye)
{
    Vp *vp = &D_8015B250->vp[view];
    Camera *cam;
    View *v;
    u16 ulx, uly, lrx, lry;
    u16 perspNorm;
    f32 uv[3][3];
    f32 p[3];
    s32 i, j;
    s32 depth;
    s32 fog;
    s32 mode;
    Poly *poly;
    PolyVtx *src;
    Texture *tex;
    f32 scale;

    physics_float_calc(view, eye->pos, 0);
    cam = &D_8017A510[view];
    memcpy(vp, cam->vp, sizeof(Vp));
    vp->vp.vscale[0] = (s16)cam->w * 2;
    vp->vp.vscale[1] = (s16)cam->h * 2;
    vp->vp.vtrans[0] = (s16)cam->x * 4;
    vp->vp.vtrans[1] = (s16)cam->y * 4;
    ulx = cam->scissor[0];
    uly = cam->scissor[1];
    lrx = cam->scissor[2];
    lry = cam->scissor[3];
    gSPViewport(D_801497C8++, &D_8015B250->vp[view]);
    gDPPipeSync(D_801497C8++);
    gDPSetScissor(D_801497C8++, G_SC_NON_INTERLACE, ulx, uly, lrx, lry);
    gDPSetFogColor(D_801497C8++, cam->fog[0], cam->fog[1], cam->fog[2], cam->fog[3]);
    gSPFogPosition(D_801497C8++, cam->fogmin, cam->fogmax);
    gDPSetCycleType(D_801497C8++, G_CYC_2CYCLE);
    gSPSetGeometryMode(D_801497C8++, G_FOG);
    depth = -1;
    gDPSetDepthSource(D_801497C8++, G_ZS_PIXEL);
    if (D_8017A638 != 1) {
        gDPSetTextureLUT(D_801497C8++, G_TT_RGBA16);
        D_8017A638 = 1;
    }
    if (cam->ortho) {
        guOrtho(&D_8015B250->proj[view], -cam->w * 0.5f, cam->w * 0.5f, -cam->h * 0.5f, cam->h * 0.5f,
                cam->near * 16.0f, cam->far * 16.0f, 1.0f);
    } else {
        guPerspective(&D_8015B250->proj[view], &perspNorm, cam->fov * 57.29578f, cam->aspect, cam->near * 16.0f,
                      cam->far * 16.0f, 1.0f);
        gSPPerspNormalize(D_801497C8++, perspNorm);
    }
    gSPMatrix(D_801497C8++, &D_8015B250->proj[view], G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    if (D_80151AD8) {
        v = &D_80150B70[view];
        math_utility(v, uv);
        func_8009EA68(3.1415927f, uv);
        if (D_80140A04) {
            uv[2][0] *= -1.0f;
            uv[1][0] *= -1.0f;
        }
        guLookAtF(D_801613F0, 0.0f, 0.0f, 0.0f, -uv[2][0], uv[2][1], uv[2][2], -uv[1][0], uv[1][1], uv[1][2]);
    } else if (D_80140A04) {
        v = &D_80150B70[view];
        guLookAtF(D_801613F0, 0.0f, 0.0f, 0.0f, -(-v->rot[2][0]), v->rot[2][1], v->rot[2][2], -(-v->rot[1][0]),
                  v->rot[1][1], v->rot[1][2]);
    } else {
        v = &D_80150B70[view];
        guLookAtF(D_801613F0, 0.0f, 0.0f, 0.0f, -v->rot[2][0], v->rot[2][1], v->rot[2][2], -v->rot[1][0],
                  v->rot[1][1], v->rot[1][2]);
    }
    guLookAtF(D_80124EF0, v->rot[2][0], v->rot[2][1], v->rot[2][2], 0.0f, 0.0f, 0.0f, v->rot[1][0], v->rot[1][1],
              v->rot[1][2]);
    if (D_801613B8 != 1.0f) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                D_80124EF0[i][j] *= D_801613B8;
            }
        }
    }
    func_8009E9D8(D_80124F30[0]);
    if (D_8014A108 < 2) {
        D_8015B250->vtx[view].v.ob[0] = D_80124EF0[0][2] * 480.0f;
        D_8015B250->vtx[view].v.ob[1] = D_80124EF0[1][2] * 480.0f;
        D_8015B250->vtx[view].v.ob[2] = D_80124EF0[2][2] * 480.0f;
    } else {
        D_8015B250->vtx[view].v.ob[0] = 0;
        D_8015B250->vtx[view].v.ob[1] = 0;
        D_8015B250->vtx[view].v.ob[2] = 0;
    }
    func_8009E8B4(D_801613F0, (u32 *)&D_8015B250->look[view]);
    gSPMatrix(D_801497C8++, &D_8015B250->look[view], G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
    guMtxIdent(&D_8015B250->ident);
    if (!D_80140A04) {
        D_8015B250->ident.m[0][0] = 0xFFFF0000;
    }
    gSPMatrix(D_801497C8++, &D_8015B250->ident, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    D_80161430 = NULL;
    D_80124FC4 = 0;
    D_80124FCC = -1;
    D_80124FE0 = 0;
    D_80124FE2 = -1;
    if (D_8015B254 >= 0) {
        track_collision_wall(&D_801497C8, NULL, &D_8012E700[D_8015B254], view, 0, 1, 0, NULL);
        D_8017A638 = -1;
        D_8017A508 = NULL;
    }
    fog = 1;
    mode = -1;
    for (i = 0, poly = D_8015B268; i < D_801613B4; i++, poly++) {
        if (poly->flags & 0x8000) {
            continue;
        }
        if (!(poly->flags & (1 << view))) {
            continue;
        }
        if (poly->count == 0) {
            continue;
        }
        gDPPipeSync(D_801497C8++);
        if (poly->flags & 0x2000) {
            if (depth < 0) {
                gDPSetDepthSource(D_801497C8++, G_ZS_PRIM);
                if (depth != poly->z) {
                    gDPSetPrimDepth(D_801497C8++, poly->z, 0);
                }
                depth = poly->z;
            }
        } else if (depth >= 0) {
            depth = -1;
            gDPSetDepthSource(D_801497C8++, G_ZS_PIXEL);
        }
        if (poly->flags & 0x4000) {
            if (mode != 0) {
                mode = 0;
                gDPSetRenderMode(D_801497C8++, 0x00504240, 0);
                gDPSetCombine(D_801497C8++, 0xFFFFFF, 0xFFFDF6FB);
            }
            gDPSetPrimColor(D_801497C8++, 0, 0, poly->u.rect.col[0], poly->u.rect.col[1], poly->u.rect.col[2],
                            poly->u.rect.col[3]);
            gDPFillRectangle(D_801497C8++, (s32)poly->u.rect.x0, (s32)poly->u.rect.y0, (s32)poly->u.rect.x1,
                             (s32)poly->u.rect.y1);
            gDPPipeSync(D_801497C8++);
            continue;
        }
        for (j = 0; j < poly->count; j++) {
            src = &poly->u.v[j];
            if (!(poly->flags & 0x10)) {
                p[0] = src->pos[0] - v->pos[0] * 16.0f;
                p[1] = src->pos[1] - v->pos[1] * 16.0f;
                p[2] = src->pos[2] - v->pos[2] * 16.0f;
                if (p[0] <= -32768.0f || 32768.0f <= p[0] || p[1] <= -32768.0f || 32768.0f <= p[1] ||
                    p[2] <= -32768.0f || 32768.0f <= p[2]) {
                    goto skip;
                }
            } else {
                func_8009E820(src->pos, p, (f32 *)v);
            }
            D_80161434[j].v.ob[0] = p[0];
            D_80161434[j].v.ob[1] = p[1];
            D_80161434[j].v.ob[2] = p[2];
            memcpy(D_80161434[j].v.tc, src->tc, 8);
        }
        if ((poly->flags & 0x100) && fog) {
            fog = 0;
            mode = -1;
            gSPClearGeometryMode(D_801497C8++, G_FOG);
            gDPPipeSync(D_801497C8++);
            gDPSetCycleType(D_801497C8++, G_CYC_1CYCLE);
        } else if (!(poly->flags & 0x100) && !fog) {
            fog = 1;
            mode = -1;
            gSPSetGeometryMode(D_801497C8++, G_FOG);
            gDPPipeSync(D_801497C8++);
            gDPSetCycleType(D_801497C8++, G_CYC_2CYCLE);
        }
        if (fog) {
            gDPSetPrimColor(D_801497C8++, 0, 0, poly->u.rect.col[0], poly->u.rect.col[1], poly->u.rect.col[2],
                            poly->u.rect.col[3]);
        }
        if (poly->flags & 0x400) {
            tex = &D_80151AE8[poly->tex >> 10].tex[poly->tex & 0x3FF];
            if (tex != D_8017A508) {
                render_display_list(tex, NULL, NULL, &D_801497C8, D_80138670[poly->tex >> 10].pal);
                D_8017A508 = tex;
            }
            if (poly->flags & 0x200) {
                if (mode != 2) {
                    mode = 2;
                    if (!fog) {
                        gDPSetRenderMode(D_801497C8++, 0x00504A70, 0);
                        gDPSetCombine(D_801497C8++, 0x121824, 0xFF33FFFF);
                    } else {
                        gDPSetRenderMode(D_801497C8++, 0xC8104A50, 0);
                        gDPSetCombine(D_801497C8++, 0x1217FF, 0xFFFFFE38);
                    }
                }
            } else if (mode != 3) {
                mode = 3;
                if (!fog) {
                    gDPSetRenderMode(D_801497C8++, 0x00504240, 0);
                    gDPSetCombine(D_801497C8++, 0x121824, 0xFF33FFFF);
                } else {
                    gDPSetRenderMode(D_801497C8++, 0xC8104240, 0);
                    gDPSetCombine(D_801497C8++, 0x1217FF, 0xFFFFFE38);
                }
            }
        } else if (mode != 1) {
            mode = 1;
            if (!fog) {
                gDPSetRenderMode(D_801497C8++, 0x00504A50, 0);
                gDPSetCombine(D_801497C8++, 0xFFFFFF, 0xFFFE793C);
            } else {
                gDPSetRenderMode(D_801497C8++, 0xC8104A70, 0);
                gDPSetCombine(D_801497C8++, 0xFFFFFF, 0xFFFE7638);
            }
        }
        gSPVertex(D_801497C8++, D_80161434, poly->count, 0);
        D_80161434 += poly->count;
        if (poly->flags & 0x1000) {
            gSP2Triangles(D_801497C8++, 0, 2, 1, 0, 2, 0, 1, 0);
            if (poly->count >= 4) {
                gSP2Triangles(D_801497C8++, 2, 0, 3, 0, 0, 2, 3, 0);
            }
        } else if (D_80140A04) {
            if (poly->count == 3) {
                gSP1Triangle(D_801497C8++, 1, 2, 0, 0);
            } else {
                gSP2Triangles(D_801497C8++, 1, 0, 2, 0, 0, 3, 2, 0);
            }
        } else {
            if (poly->count == 3) {
                gSP1Triangle(D_801497C8++, 2, 0, 1, 0);
            } else {
                gSP2Triangles(D_801497C8++, 2, 0, 1, 0, 0, 2, 3, 0);
            }
        }
    skip:;
    }
    if (D_8017A638 != 1) {
        gDPSetTextureLUT(D_801497C8++, G_TT_RGBA16);
        D_8017A638 = 1;
    }
    if (depth >= 0) {
        gDPSetDepthSource(D_801497C8++, G_ZS_PIXEL);
    }
    if (!fog) {
        gSPSetGeometryMode(D_801497C8++, G_FOG);
        gDPPipeSync(D_801497C8++);
        gDPSetCycleType(D_801497C8++, G_CYC_2CYCLE);
    }
    if (D_80124FCC >= 0) {
        D_80124FC4 = 1;
        D_80124FE0 = 0;
        track_collision_wall(&D_801497C8, NULL, &D_8012E700[D_80124FCC], view, 0, 1, 0, NULL);
        D_8017A638 = -1;
        D_8017A508 = NULL;
    }
    if (D_80124FE2 >= 0) {
        D_80124FC4 = 0;
        D_80124FE0 = 1;
        track_collision_wall(&D_801497C8, NULL, &D_8012E700[D_80124FE2], view, 0, 1, 0, NULL);
        D_8017A638 = -1;
        D_8017A508 = NULL;
    }
}

void zz_caller(s32 n)
{
    s32 i;
    for (i = 0; i < n; i++) {
        func_8009F058(i, 0, 0, &D_80150B70[i]);
    }
}
void zz_caller2(s32 n)
{
    func_8009F058(n, 0, 0, &D_80150B70[n]);
}
f32 camera_update_c(f32 input) { return func_8009C3F8(0, input); }
f32 select_screen_update(f32 input) { return func_8009C3F8(1, input); }
