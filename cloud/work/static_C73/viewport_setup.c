/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
extern float gScaleTicksPerSecond, gScaleSecondsPerTick, gViewportFloatC, gViewportFloatD;
extern s16 gViewportX, gViewportY, gViewportLeftEdge, gViewportRightEdge, gViewportTopEdge, gViewportBottomEdge;
extern s16 gViewportXOverflow, gViewportYOverflow, gViewportScaleX, gViewportScaleY, gViewportScale, gViewportScaleYAlt;
extern s16 gViewportOffsetXExtra[], gViewportOffsetYExtra[], gViewportOffsetX[], gViewportOffsetY[];
extern OSViMode *gViewportStruct, *gViewportDataPtr, gViModeTableBase[];
extern void viewport_scale(float, float), display_update(void);
void viewport_setup(s32 tv, s32 mode, s32 width, s32 height) {
    if (tv == 0) {
        gScaleTicksPerSecond = 50.0f;
        gScaleSecondsPerTick = gViewportFloatC;
    } else {
        gScaleTicksPerSecond = 60.0f;
        gScaleSecondsPerTick = gViewportFloatD;
    }
    gViewportX = tv;
    gViewportY = mode;
    gViewportStruct = &gViModeTableBase[mode];
    gViewportLeftEdge = (gViewportStruct->comRegs.hStart >> 16) & 0x3FF;
    gViewportRightEdge = gViewportStruct->comRegs.hStart & 0x3FF;
    gViewportTopEdge = (gViewportStruct->fldRegs[0].vStart >> 16) & 0x3FF;
    gViewportBottomEdge = gViewportStruct->fldRegs[0].vStart & 0x3FF;
    if (gViewportDataPtr == NULL) {
        gViewportXOverflow = ((s16 (*)[4])gViewportOffsetXExtra)[gViewportX][0];
        gViewportYOverflow = ((s16 (*)[4])gViewportOffsetYExtra)[gViewportX][0];
        gViewportScaleX = ((s16 (*)[4])gViewportOffsetX)[gViewportX][0];
        gViewportScaleY = ((s16 (*)[4])gViewportOffsetY)[gViewportX][0];
    } else {
        viewport_scale((float)width / (float)gViewportScale, (float)height / (float)gViewportScaleYAlt);
    }
    gViewportScale = width;
    gViewportScaleYAlt = height;
    display_update();
}
