/* flags: -g0 -O2 -mips2 -G 0 -non_shared (compiler ido5.3s: IDO 5.3 cc -S, .loc stripped, as1 -O1 -g) */
/* inflate_free_window - lib_5610 (ido5.3s). The label is historical: this is gzip inflate.c
 * inflate_codes(tl, td, bl, bd), decoding literal/length and distance codes with the tables
 * until end-of-block (code 15): literals go straight to the output (gInflateOutPtr, the whole
 * output buffer is the window); a match copies from behind with memcpy when the distance is
 * more than 8 bytes back, byte by byte otherwise (overlapping copy); invalid code (99) -> 1.
 * w26a reconstruction from gzip's text (no earlier draft).
 * Unused local (disclosed, wave-12 rule): `w`, gzip's "current window position", declared where
 * gzip declares it and never used (Rush writes through gInflateOutPtr). It is the exact frame
 * residual: retail frame 0x60, 0x58 without it; one 4-byte slot (+4 alignment) that moves
 * ml/md from 0x40/0x44 to retail's 0x44/0x48. */
#include "types.h"
typedef struct huft { u8 e; u8 b; union { u16 n; struct huft *t; } v; } huft;
extern u32 gInflateBitBuf, gInflateBitCount;
extern u8 *gInflateInPtr, *gInflateInEnd, *gInflateOutPtr;
extern s32 inflate_read_bits(void);
extern u16 gInflateMaskBits[];
extern void *memcpy(void *, const void *, u32);
#define NEEDBITS(n) do { while (k < (n)) { b |= (u32)(gInflateInPtr < gInflateInEnd ? (gInflateInPtr += 2, gInflateInPtr[-2] | (gInflateInPtr[-1] << 8)) : inflate_read_bits()) << k; k += 16; } } while (0)
#define DUMPBITS(n) do { b >>= (n); k -= (n); } while (0)
s32 inflate_free_window(huft *tl, huft *td, s32 bl, s32 bd) {
    register u32 e;
    u32 n;
    s32 d;
    u32 w;  /* gzip: current window position (unused here, see header) */
    huft *t;
    u32 ml, md;
    register u32 b;
    register u32 k;

    b = gInflateBitBuf;
    k = gInflateBitCount;
    ml = gInflateMaskBits[bl];
    md = gInflateMaskBits[bd];
    for (;;) {
        NEEDBITS((u32)bl);
        if ((e = (t = tl + (b & ml))->e) > 16)
            do {
                if (e == 99) return 1;
                DUMPBITS(t->b);
                e -= 16;
                NEEDBITS(e);
            } while ((e = (t = t->v.t + (b & gInflateMaskBits[e]))->e) > 16);
        DUMPBITS(t->b);
        if (e == 16) {
            *gInflateOutPtr++ = (u8)t->v.n;
        } else {
            if (e == 15) break;
            NEEDBITS(e);
            n = t->v.n + (b & gInflateMaskBits[e]);
            DUMPBITS(e);
            NEEDBITS((u32)bd);
            if ((e = (t = td + (b & md))->e) > 16)
                do {
                    if (e == 99) return 1;
                    DUMPBITS(t->b);
                    e -= 16;
                    NEEDBITS(e);
                } while ((e = (t = t->v.t + (b & gInflateMaskBits[e]))->e) > 16);
            DUMPBITS(t->b);
            NEEDBITS(e);
            d = -t->v.n - (b & gInflateMaskBits[e]);
            DUMPBITS(e);
            if (d < -8) {
                memcpy(gInflateOutPtr, gInflateOutPtr + d, n);
                gInflateOutPtr += n;
            } else {
                while (n--) {
                    *gInflateOutPtr = gInflateOutPtr[d];
                    gInflateOutPtr++;
                }
            }
        }
    }
    gInflateBitBuf = b;
    gInflateBitCount = k;
    return 0;
}
