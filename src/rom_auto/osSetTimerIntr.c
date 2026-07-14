/* Auto-decompiled by mips_to_c (autodecomp). */
#include "rom_auto.h"

void osSetTimerIntr(OSMesgQueue *arg0, void *arg1, s32 arg2) {
    u32 temp_a0;

    temp_a0 = __osDisableInt();
    __osViContext->msgq = arg0;
    __osViContext->msg = arg1;
    __osViContext->retraceCount = (u16) arg2;
    __osRestoreInt(temp_a0);
}
