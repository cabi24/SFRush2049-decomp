/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
extern OSViMode *gViewportStruct, *gViewportDataPtr, gViewportBuffer[];
extern s16 gViewportLeftEdge, gViewportRightEdge, gViewportTopEdge, gViewportBottomEdge;
extern s16 gViewportScaleX, gViewportScaleY, gViewportXOverflow, gViewportYOverflow;
extern s16 gViewportScale, gViewportScaleYAlt, gViewportPendingFrames;
extern float gViewportFloatA, gViewportFloatB;
extern u32 osSetGlobalIntMask(u32);
extern void osWritebackDCache(void *, s32);
void display_update(void) {
    u32 saved;
    s32 left, top, right, bottom;
    u32 verticalScale;
    float xRatio, yRatio;
    if (gViewportStruct == NULL) return;
    left = gViewportLeftEdge + gViewportScaleX;
    right = gViewportRightEdge + gViewportScaleX + gViewportXOverflow;
    top = gViewportTopEdge + gViewportScaleY;
    bottom = gViewportBottomEdge + gViewportScaleY + gViewportYOverflow;
    xRatio = (float)(gViewportRightEdge - gViewportLeftEdge) / (float)(right - left);
    yRatio = (float)(gViewportBottomEdge - gViewportTopEdge) / (float)(bottom - top);
    saved = osSetGlobalIntMask(1);
    if (gViewportDataPtr == &gViewportBuffer[0]) {
        gViewportDataPtr = &gViewportBuffer[1];
    } else {
        gViewportDataPtr = &gViewportBuffer[0];
    }
    memcpy(gViewportDataPtr, gViewportStruct, sizeof(OSViMode));
    left = (s32)((float)left - (1.0f - (float)gViewportDataPtr->comRegs.xScale / (float)gViewportStruct->comRegs.xScale) * 5.0f);
    gViewportDataPtr->comRegs.hStart = ((u32)left << 16) | (u32)right;
    gViewportDataPtr->comRegs.xScale = (u32)((float)gViewportScale * xRatio * gViewportFloatA);
    gViewportDataPtr->fldRegs[0].vStart = ((top & 0x3FF) << 16) | (bottom & 0x3FF);
    gViewportDataPtr->fldRegs[0].origin = gViewportScale * 2;
    verticalScale = (u32)((float)gViewportScaleYAlt * yRatio * gViewportFloatB + 16.5f);
    if (gViewportStruct->fldRegs[0].origin != gViewportStruct->fldRegs[1].origin) {
        gViewportDataPtr->fldRegs[1].origin = gViewportScale * 4;
        gViewportDataPtr->comRegs.width = gViewportScale * 2;
        verticalScale >>= 1;
    } else {
        gViewportDataPtr->fldRegs[1].origin = gViewportScale * 2;
        gViewportDataPtr->comRegs.width = gViewportScale;
    }
    if (gViewportDataPtr->comRegs.ctrl & 0x40) {
        gViewportDataPtr->fldRegs[1].vStart = (((top + 2) & 0x3FF) << 16) | ((bottom + 2) & 0x3FF);
    } else {
        gViewportDataPtr->fldRegs[1].vStart = ((top & 0x3FF) << 16) | (bottom & 0x3FF);
    }
    verticalScale &= 0xFFF0;
    gViewportDataPtr->fldRegs[0].yScale = (gViewportDataPtr->fldRegs[0].yScale & 0xFFFF0000) | verticalScale;
    gViewportDataPtr->fldRegs[1].yScale = (gViewportDataPtr->fldRegs[1].yScale & 0xFFFF0000) | verticalScale;
    gViewportPendingFrames = 2;
    osWritebackDCache(gViewportDataPtr, sizeof(OSViMode));
    osSetGlobalIntMask(saved);
}
