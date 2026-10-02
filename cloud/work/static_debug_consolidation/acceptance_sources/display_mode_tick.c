/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
void display_mode_tick(void) {
    if (gViewportPendingFrames == 0) return;
    if (--gViewportPendingFrames == 0) {
        if ((gViewportDataPtr->fldRegs[0].yScale & 0xFFFF) == (gViewportStruct->fldRegs[0].yScale & 0xFFFF)) {
            osSetIntMask(0);
        }
        osSetThreadPri(gViewportDataPtr);
        osViSetSpecialFeatures(0xAA);
    }
}
