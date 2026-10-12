/* flags: -g0 -O2 -mips2 -G 0 -non_shared (compiler ido5.3s: IDO 5.3 cc -S, .loc stripped, as1 -O1) */
/* inflate_loop - lib_5610 (ido5.3s). gzip inflate.c inflate(): clear the bit buffer and decode
 * blocks until the last one; a failing block returns 1 (gzip returns the block's code), the
 * end of the stream returns 0. (gzip's window-flush and hufts statistics are not present.)
 * Draft: w25a inflate/inflate_loop.c, unchanged; standard -O2 differs only in the prologue
 * order, which the recipe's .loc-free as1 -O1 schedule reproduces. No shaping devices. */
#include "types.h"

extern u32 gInflateBitBuf, gInflateBitCount;
extern s32 inflate_block(s32 *last);

s32 inflate_loop(void) {
    s32 e;

    gInflateBitCount = 0;
    gInflateBitBuf = 0;
    do {
        if (inflate_block(&e) != 0) {
            return 1;
        }
    } while (!e);
    return 0;
}
