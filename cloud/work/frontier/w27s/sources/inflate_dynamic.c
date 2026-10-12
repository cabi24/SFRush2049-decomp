/* flags: -g0 -O2 -mips2 -G 0 -non_shared (compiler ido5.3s: IDO 5.3 cc -S, .loc stripped, as1 -O1) */
/* inflate_dynamic - lib_5610 (ido5.3s). gzip inflate.c inflate_dynamic(): read the dynamic-block
 * header (HLIT nl, HDIST nd, HCLEN nb), the code-length code lengths in gInflateBorder order,
 * build the code-length table (huft_build into tl), decode the nl + nd literal/length and distance
 * code lengths (repeat codes 16/17/18), build the literal/length and distance tables with the
 * base/extra tables (gInflateCplens/Cplext/Cpdist/Cpdext; the fixed-table bit widths come from
 * gInflateHuftPointerLit/Dist) and inflate the block with inflate_free_window (gzip inflate_codes).
 * gDisplayListSize is the huft_alloc arena cursor (reset to 0 on every exit, 0x4F0 after the
 * length-code pass). From w26a's best.c (344/484 register-equal).
 * w27s: NEEDBITS/DUMPBITS are gzip's ORIGINAL brace macros `{ ... }`, not the TU's do-while(0)
 * form (named *_BR here only because the TU defines NEEDBITS/DUMPBITS for the other slots). The
 * 16 `do {} while (0)` statements cost uopt 2 blocks each (154 blocks, callee-saved cost
 * blocks/4 = 38.5); with gzip's braces it is 122 blocks, cost 30.5, and the nl + nd expression
 * web (tot 33) gets s8 as in retail (n itself stays a memory variable at 0x84, reloaded for the
 * loop test), and td is no longer reloaded. No shaping devices, unused locals or pad arrays.
 * The end pointer &gInflateBorder[19] of the zero-fill loop is retail's split symbol
 * gInflateCplens: two reloc_addrs entries (rom 0x6BDC/0x6BE0, gInflateBorder + 0x4C). */
#include "types.h"
typedef struct huft { u8 e; u8 b; union { u16 n; struct huft *t; } v; } huft;
extern u32 gInflateBitBuf, gInflateBitCount;
extern u8 *gInflateInPtr, *gInflateInEnd;
extern s32 inflate_read_bits(void);
extern u16 gInflateCplens[], gInflateCplext[], gInflateCpdist[], gInflateCpdext[];
extern unsigned int gDisplayListSize;
extern s32 huft_alloc(s32);
extern s32 huft_build(u32 *, u32, u32, u16 *, u16 *, huft **, s32 *);
extern s32 inflate_free_window(huft *, huft *, s32, s32);
extern u32 gInflateBorder[];
extern u16 gInflateMaskBits[];
extern s32 gInflateHuftPointerLit, gInflateHuftPointerDist;
#define NEEDBITS_BR(n) { while (k < (n)) { b |= (u32)(gInflateInPtr < gInflateInEnd ? (gInflateInPtr += 2, gInflateInPtr[-2] | (gInflateInPtr[-1] << 8)) : inflate_read_bits()) << k; k += 16; } }
#define DUMPBITS_BR(n) { b >>= (n); k -= (n); }
s32 inflate_dynamic(void) {
    s32 i;
    u32 j;
    u32 l;
    u32 m;
    u32 n;
    huft *tl;
    huft *td;
    s32 bl;
    s32 bd;
    u32 nb;
    u32 nl;
    u32 nd;
    u32 *ll;
    register u32 b;
    register u32 k;

    ll = (u32 *)huft_alloc((286 + 30) * sizeof(u32));
    b = gInflateBitBuf;
    k = gInflateBitCount;
    NEEDBITS_BR(5);
    nl = 257 + (b & 0x1f);
    DUMPBITS_BR(5);
    NEEDBITS_BR(5);
    nd = 1 + (b & 0x1f);
    DUMPBITS_BR(5);
    NEEDBITS_BR(4);
    nb = 4 + (b & 0xf);
    DUMPBITS_BR(4);
    if (nl > 286 || nd > 30) return 1;
    for (j = 0; j < nb; j++) {
        NEEDBITS_BR(3);
        ll[gInflateBorder[j]] = b & 7;
        DUMPBITS_BR(3);
    }
    for (; j < 19; j++) ll[gInflateBorder[j]] = 0;
    bl = 7;
    if ((i = huft_build(ll, 19, 19, NULL, NULL, &tl, &bl)) != 0) {
        gDisplayListSize = 0;
        return i;
    }
    n = nl + nd;
    m = gInflateMaskBits[bl];
    i = l = 0;
    while ((u32)i < n) {
        NEEDBITS_BR((u32)bl);
        j = (td = tl + (b & m))->b;
        DUMPBITS_BR(j);
        j = td->v.n;
        if (j < 16)
            ll[i++] = l = j;
        else if (j == 16) {
            NEEDBITS_BR(2);
            j = 3 + (b & 3);
            DUMPBITS_BR(2);
            if ((u32)i + j > n) return 1;
            while (j--) ll[i++] = l;
        } else if (j == 17) {
            NEEDBITS_BR(3);
            j = 3 + (b & 7);
            DUMPBITS_BR(3);
            if ((u32)i + j > n) return 1;
            while (j--) ll[i++] = 0;
            l = 0;
        } else {
            NEEDBITS_BR(7);
            j = 11 + (b & 0x7f);
            DUMPBITS_BR(7);
            if ((u32)i + j > n) return 1;
            while (j--) ll[i++] = 0;
            l = 0;
        }
    }
    gDisplayListSize = 0x4F0;
    gInflateBitBuf = b;
    gInflateBitCount = k;
    bl = gInflateHuftPointerLit;
    if ((i = huft_build(ll, nl, 257, gInflateCplens, gInflateCplext, &tl, &bl)) != 0) {
        gDisplayListSize = 0;
        return i;
    }
    bd = gInflateHuftPointerDist;
    if ((i = huft_build(ll + nl, nd, 0, gInflateCpdist, gInflateCpdext, &td, &bd)) != 0) {
        gDisplayListSize = 0;
        return i;
    }
    if ((i = inflate_free_window(tl, td, bl, bd)) != 0) return i < 0 ? i : 1;
    gDisplayListSize = 0;
    return 0;
}
