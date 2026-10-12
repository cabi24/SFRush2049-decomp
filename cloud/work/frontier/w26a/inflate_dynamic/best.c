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
#define NEEDBITS(n) do { while (k < (n)) { b |= (u32)(gInflateInPtr < gInflateInEnd ? (gInflateInPtr += 2, gInflateInPtr[-2] | (gInflateInPtr[-1] << 8)) : inflate_read_bits()) << k; k += 16; } } while (0)
#define DUMPBITS(n) do { b >>= (n); k -= (n); } while (0)
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
    NEEDBITS(5);
    nl = 257 + (b & 0x1f);
    DUMPBITS(5);
    NEEDBITS(5);
    nd = 1 + (b & 0x1f);
    DUMPBITS(5);
    NEEDBITS(4);
    nb = 4 + (b & 0xf);
    DUMPBITS(4);
    if (nl > 286 || nd > 30) return 1;
    for (j = 0; j < nb; j++) {
        NEEDBITS(3);
        ll[gInflateBorder[j]] = b & 7;
        DUMPBITS(3);
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
        NEEDBITS((u32)bl);
        j = (td = tl + (b & m))->b;
        DUMPBITS(j);
        j = td->v.n;
        if (j < 16)
            ll[i++] = l = j;
        else if (j == 16) {
            NEEDBITS(2);
            j = 3 + (b & 3);
            DUMPBITS(2);
            if ((u32)i + j > n) return 1;
            while (j--) ll[i++] = l;
        } else if (j == 17) {
            NEEDBITS(3);
            j = 3 + (b & 7);
            DUMPBITS(3);
            if ((u32)i + j > n) return 1;
            while (j--) ll[i++] = 0;
            l = 0;
        } else {
            NEEDBITS(7);
            j = 11 + (b & 0x7f);
            DUMPBITS(7);
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
