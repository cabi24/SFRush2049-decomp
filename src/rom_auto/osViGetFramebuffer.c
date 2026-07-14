/* Auto-decompiled by mips_to_c (autodecomp). */
#include "rom_auto.h"

void *osViGetFramebuffer(void) {
    void *sp18;
    u32 temp_a0;

    temp_a0 = __osDisableInt();
    sp18 = __osViContext->framep;
    __osRestoreInt(temp_a0);
    return sp18;
}
