/* SDK GBI subset (as w36e) */
typedef struct { u32 w0; u32 w1; } Gwords;
typedef union { Gwords words; long long int force_structure_alignment; } Gfx;
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define gSPTexture(pkt, s, t, level, tile, on) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(0xD7, 24, 8) | _SHIFTL(0, 16, 8) | _SHIFTL((level), 11, 3) | _SHIFTL((tile), 8, 3) | _SHIFTL((on), 1, 7)); \
    _g->words.w1 = (_SHIFTL((s), 16, 16) | _SHIFTL((t), 0, 16)); \
}
#define gDPNoParam(pkt, cmd) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(cmd, 24, 8); \
    _g->words.w1 = 0; \
}
#define gDPPipeSync(pkt) gDPNoParam(pkt, 0xE7)
#define gSPEndDisplayList(pkt) gDPNoParam(pkt, 0xDF)
#define gSPSetOtherMode(pkt, cmd, sft, len, data) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(cmd, 24, 8) | _SHIFTL(32 - (sft) - (len), 8, 8) | _SHIFTL((len) - 1, 0, 8)); \
    _g->words.w1 = (u32)(data); \
}
#define gDPSetRenderMode(pkt, c0, c1) gSPSetOtherMode(pkt, 0xE2, 3, 29, (c0) | (c1))
#define gDPSetCombine(pkt, muxs0, muxs1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(0xFC, 24, 8) | _SHIFTL(muxs0, 0, 24); \
    _g->words.w1 = (u32)(muxs1); \
}
#define gSPVertex(pkt, v, n, v0) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(0x01, 24, 8) | _SHIFTL((n), 12, 8) | _SHIFTL((v0) + (n), 1, 7); \
    _g->words.w1 = (u32)(v); \
}
#define __gsSP1Triangle_w1(v0, v1, v2) (_SHIFTL((v0) * 2, 16, 8) | _SHIFTL((v1) * 2, 8, 8) | _SHIFTL((v2) * 2, 0, 8))
#define gSP2Triangles(pkt, v00, v01, v02, flag0, v10, v11, v12, flag1) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(0x06, 24, 8) | __gsSP1Triangle_w1(v00, v01, v02)); \
    _g->words.w1 = __gsSP1Triangle_w1(v10, v11, v12); \
}
typedef struct { s16 ob[3]; u16 flag; s16 tc[2]; u8 cn[4]; } Vtx;
