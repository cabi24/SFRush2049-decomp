/* flags: -g0 -O2 -mips2 -G 0 -non_shared (compiler ido5.3s: IDO 5.3 cc -S, .loc stripped, as1 -O1) */
/* inflate_stored - lib_5610 (ido5.3s). gzip inflate.c inflate_stored(): drop to a byte
 * boundary, read LEN and NLEN (16 bits each), fail (1) unless NLEN is LEN's complement, then
 * copy LEN bytes from the bit stream to the output (gInflateOutPtr; Rush has no sliding-window
 * flush here: the whole output buffer is the window) and store the bit buffer back.
 * From static_C20/inflate_stored.c rewritten in gzip's form with inflate_block's macros:
 * `DUMPBITS(k & 7)` (retail computes the alignment into a temporary, not into n) and the
 * conditional-expression NEEDBITS (with a named `word` the `n--` temporary takes v0, retail
 * v1). No shaping devices. */
#include "types.h"

extern u32 gInflateBitBuf, gInflateBitCount;
extern u8 *gInflateInPtr, *gInflateInEnd, *gInflateOutPtr;
extern s32 inflate_read_bits(void);

#define NEEDBITS(n) do { while (k < (n)) { b |= (u32)(gInflateInPtr < gInflateInEnd ? (gInflateInPtr += 2, gInflateInPtr[-2] | (gInflateInPtr[-1] << 8)) : inflate_read_bits()) << k; k += 16; } } while (0)
#define DUMPBITS(n) do { b >>= (n); k -= (n); } while (0)

s32 inflate_stored(void) {
    u32 n;
    register u32 b = gInflateBitBuf, k = gInflateBitCount;

    DUMPBITS(k & 7);
    NEEDBITS(16);
    n = b & 0xffff;
    DUMPBITS(16);
    NEEDBITS(16);
    if (n != ((~b) & 0xffff)) return 1;
    DUMPBITS(16);
    while (n--) {
        NEEDBITS(8);
        *gInflateOutPtr++ = (u8)b;
        DUMPBITS(8);
    }
    gInflateBitBuf = b;
    gInflateBitCount = k;
    return 0;
}
