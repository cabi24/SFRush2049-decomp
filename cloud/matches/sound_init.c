/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * sound_init (0x800A4934, 400 bytes): begin a double-buffered display list.
 * Historical name; this is N64 graphics-state initialization, not audio.
 * No arcade ancestor: N64-specific RDP/RSP state-cache and GBI setup.
 *
 * Matches in the real -O3 group described by recipe.json. Keep this public
 * no-argument entry and the existing gfx_modes roots; func_80086A50 remains
 * internal so its genuine register summary preserves the height address.
 * The accepted object_render.c owns D_8012E688 as s64 in that same group.
 * Do not replace its definition with an extern-only unit or invent a second
 * object owner here. No accepted source file is modified by this packet.
 *
 * The four SDK-style macros each have their own block-local Gfx pointer.
 * Their source lifetimes reproduce the retail allocator without fake locals,
 * keepers, assembly, volatile steering, or unrelated callers.
 */
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef signed short s16;
typedef signed long long s64;
typedef struct { u32 w0, w1; } Gwords;
typedef union {
    Gwords words;
    s64 force_structure_alignment;
} Gfx;

/* SDK GBI forms, using the same self-contained types as gfx_modes. */
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((0x01 << (w)) - 1)) << (s)))
#define G_SETBLENDCOLOR 0xF9
#define G_RDPPIPESYNC 0xE7
#define G_SETOTHERMODE_H 0xE3
#define G_GEOMETRYMODE 0xD9
#define G_MDSFT_TEXTPERSP 19
#define G_TP_NONE (0 << G_MDSFT_TEXTPERSP)

#define gDPSetColor(pkt, c, d) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(c, 24, 8); \
    _g->words.w1 = (unsigned int)(d); \
}
#define gDPSetBlendColor(pkt, r, g, b, a) \
    gDPSetColor(pkt, G_SETBLENDCOLOR, \
        _SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | \
        _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8))
#define gDPPipeSync(pkt) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_RDPPIPESYNC, 24, 8); \
    _g->words.w1 = 0; \
}
#define gSPSetOtherMode(pkt, cmd, sft, len, data) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(cmd, 24, 8) | \
        _SHIFTL(32 - (sft) - (len), 8, 8) | _SHIFTL((len) - 1, 0, 8)); \
    _g->words.w1 = (unsigned int)(data); \
}
#define gDPSetTexturePersp(pkt, type) \
    gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_TEXTPERSP, 1, type)
#define gSPGeometryMode(pkt, c, s) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = _SHIFTL(G_GEOMETRYMODE, 24, 8) | _SHIFTL(~(u32)(c), 0, 24); \
    _g->words.w1 = (u32)(s); \
}
#define gSPClearGeometryMode(pkt, c) gSPGeometryMode(pkt, c, 0)

extern Gfx *D_80149438;
extern Gfx D_80124FE8[2][2400];
extern u32 D_8012E608;
extern s32 D_8012E60C, D_8012E668, D_8012E610;
extern s32 D_8002AFC0, D_8002AFC4, D_8012E674;
extern s16 D_8012E67A;
extern u8 D_8011EACF;
extern void *D_8012E684;
extern s32 D_8012E6C0, D_8014A248, D_8012E680, D_8012E6D0;
extern s64 D_8012E688;
extern void func_80086A50(s32);
extern void func_800878E0(u32);

void sound_init(void)
{
    if (D_80149438 == 0) {
        D_8012E608 = 0;
        D_8012E60C = 0;
        D_8012E668 = 0;
        D_8012E610 = D_8002AFC0 - 1;
        D_8012E674 = D_8002AFC4 - 1;
        D_8012E67A = 0;
        D_8011EACF = 255;
        D_8012E684 = 0;
        D_8012E688 = 0;
        if (++D_8012E6C0 >= 2) D_8012E6C0 = 0;
        D_80149438 = D_80124FE8[D_8012E6C0];
        D_8014A248 = -1;
        D_8012E680 = -1;
        D_8012E6D0 = 0;
        gDPSetBlendColor(D_80149438++, 0, 0, 0, 16);
        gDPPipeSync(D_80149438++);
        gDPSetTexturePersp(D_80149438++, G_TP_NONE);
        gSPClearGeometryMode(D_80149438++, -1);
        func_80086A50(1);
        if (D_8002AFC4 >= 221) func_800878E0(0x8000);
    }
}
