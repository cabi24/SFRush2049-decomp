/* flags: -g0 -O1 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
void osViSetMode(void *mode) {
    u32 temp_a0;

    temp_a0 = __osDisableInt();
    __osViContext->framep = mode;
    __osViContext->state |= 0x10;
    __osRestoreInt(temp_a0);
}
