/* flags: -g0 -O2 -mips2 -G 0 -non_shared (cc -S, as1 -O1: see RESULTS.md job 3) */
#include "rom_tu.h"
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
