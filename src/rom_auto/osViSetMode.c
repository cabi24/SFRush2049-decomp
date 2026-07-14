/* Auto-decompiled by mips_to_c (autodecomp). */
#include "rom_auto.h"

void osViSetMode(void *mode) {
    u32 temp_a0;

    temp_a0 = __osDisableInt();
    __osViContext->framep = mode;
    __osViContext->state |= 0x10;
    __osRestoreInt(temp_a0);
}
