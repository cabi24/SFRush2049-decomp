/*
 * func_80086A50(mode): emit cycle type, render mode and combiner for render mode 0..4 (variants chosen
 * by bits 4/5 of D_8012E608 and by the previous mode D_8014A248), then record the mode.
 * Same 387 words as the locked src/blob/groups/func_80086A50/f.c, rewritten with SDK GBI macros
 * (wave 1, w1a). The macro form matters to the CALLERS: each macro's block-scoped `_g` is coloured by
 * uopt (a2, a3 in turn), so the callee's IPA register summary includes a2/a3 although the emitted
 * code only touches a0, a1, v0, v1, t6-t9. With the hand-pushed locked form the callers pick a2/a3
 * for values they keep across the call, where retail uses t0/t1.
 * gDPSetRenderModeRaw / gDPSetCombine take raw words; the named G_RM_* / G_CC_* modes are not yet
 * identified.
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
#define G_SETCOMBINE 0xFC
#define G_SETENVCOLOR 0xFB
#define G_SETPRIMDEPTH 0xEE
#define G_SETOTHERMODE_L 0xE2
#define G_SETOTHERMODE_H 0xE3
#define G_MDSFT_ALPHACOMPARE 0
#define G_MDSFT_ZSRCSEL 2
#define G_MDSFT_RENDERMODE 3
#define G_MDSFT_CYCLETYPE 20
#define G_CYC_1CYCLE (0 << G_MDSFT_CYCLETYPE)
#define G_CYC_2CYCLE (1 << G_MDSFT_CYCLETYPE)
#define G_CYC_COPY (2 << G_MDSFT_CYCLETYPE)
#define G_AC_NONE (0 << G_MDSFT_ALPHACOMPARE)
#define G_AC_THRESHOLD (1 << G_MDSFT_ALPHACOMPARE)
#define G_ZS_PIXEL (0 << G_MDSFT_ZSRCSEL)
#define G_ZS_PRIM (1 << G_MDSFT_ZSRCSEL)

#define gSPSetOtherMode(pkt, cmd, sft, len, data) \
{ \
    Gfx *_g = (Gfx *)(pkt); \
    _g->words.w0 = (_SHIFTL(cmd, 24, 8) | _SHIFTL(32 - (sft) - (len), 8, 8) | _SHIFTL((len) - 1, 0, 8)); \
    _g->words.w1 = (unsigned int)(data); \
}
#define gDPSetCycleType(pkt, type) gSPSetOtherMode(pkt, G_SETOTHERMODE_H, G_MDSFT_CYCLETYPE, 2, type)
#define gDPSetAlphaCompare(pkt, type) gSPSetOtherMode(pkt, G_SETOTHERMODE_L, G_MDSFT_ALPHACOMPARE, 2, type)
#define gDPSetDepthSource(pkt, src) gSPSetOtherMode(pkt, G_SETOTHERMODE_L, G_MDSFT_ZSRCSEL, 1, src)
#define gDPSetRenderModeRaw(pkt, m) gSPSetOtherMode(pkt, G_SETOTHERMODE_L, G_MDSFT_RENDERMODE, 29, m)
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
#define sDPRGBColor(pkt, cmd, r, g, b, a) \
    gDPSetColor(pkt, cmd, (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)))
#define gDPSetEnvColor(pkt, r, g, b, a) sDPRGBColor(pkt, G_SETENVCOLOR, r, g, b, a)

extern u32 D_8012E608;
extern Gfx *D_80149438;
extern s32 D_8014A248;

void func_80086A50(s32 mode)
{
    switch (mode) {
    case 0:
        if (D_8014A248 != 0) {
            gDPSetCycleType(D_80149438++, 0x200000);
        }
        gDPSetRenderModeRaw(D_80149438++, 0x0F0A4000);
        gDPSetCombine(D_80149438++, 0xFFFFFF, 0xFFFCF279);
        break;
    case 1:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            gDPSetCycleType(D_80149438++, 0);
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504A70);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x20) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504240);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x10) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504A70);
            gDPSetCombine(D_80149438++, 0xFFFFFF, 0xFFFCF279);
        }
        else {
            gDPSetRenderModeRaw(D_80149438++, 0x00504240);
            gDPSetCombine(D_80149438++, 0xFFFFFF, 0xFFFCF279);
        }
        break;
    case 2:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            gDPSetCycleType(D_80149438++, 0);
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504A70);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x20) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504240);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x10) {
            gDPSetRenderModeRaw(D_80149438++, 0x00504A70);
            gDPSetCombine(D_80149438++, 0x11FE23, 0xFFFFF3F9);
        }
        else {
            gDPSetRenderModeRaw(D_80149438++, 0x00504240);
            gDPSetCombine(D_80149438++, 0x11FE23, 0xFFFFF3F9);
        }
        break;
    case 3:
        if (D_8014A248 <= 0 || D_8014A248 >= 4) {
            gDPSetCycleType(D_80149438++, 0);
        }
        if ((D_8012E608 & 0x10) && (D_8012E608 & 0x20)) {
            gDPSetRenderModeRaw(D_80149438++, 0x00553078);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x20) {
            gDPSetRenderModeRaw(D_80149438++, 0x0F0A7008);
            gDPSetCombine(D_80149438++, 0x119623, 0xFF2FFFFF);
        }
        else if (D_8012E608 & 0x10) {
            gDPSetRenderModeRaw(D_80149438++, 0x00553078);
            gDPSetCombine(D_80149438++, 0x11FE23, 0xFFFFF3F9);
        }
        else {
            gDPSetRenderModeRaw(D_80149438++, 0x0F0A7008);
            gDPSetCombine(D_80149438++, 0x11FE23, 0xFFFFF3F9);
        }
        break;
    case 4:
        if (D_8014A248 < 4) {
            gDPSetCycleType(D_80149438++, 0x100000);
        }
        gDPSetRenderModeRaw(D_80149438++, 0x00504240);
        gDPSetCombine(D_80149438++, 0xFFABFF, 0xFFFC9238);
        gDPSetEnvColor(D_80149438++, 0, 0, 0, 0x55);
        break;
    }
    D_8014A248 = mode;
}
