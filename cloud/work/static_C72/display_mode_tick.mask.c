/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
extern s16 gViewportPendingFrames;
extern OSViMode *gViewportStruct, *gViewportDataPtr;
void display_mode_tick(void) {
    if (gViewportPendingFrames == 0) return;
    if (--gViewportPendingFrames == 0) {
        if ((gViewportStruct->fldRegs[0].yScale & 0xFFFF) == (gViewportDataPtr->fldRegs[0].yScale & 0xFFFF)) {
            osSetIntMask(0);
        }
        osSetThreadPri(gViewportDataPtr);
        osViSetSpecialFeatures(0xAA);
    }
}
