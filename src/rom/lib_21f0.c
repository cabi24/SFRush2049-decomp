/* GENERATED ROM-aligned TU — segment 0x21f0 (rom/lib_21f0)
 * layout map fe94ca7f42eb4854cf5b13eed87a4b511f52b70215182a56919a592137bfae27; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "static_debug_context.h"

/* PROMOTED 2026-10-02 — display_update
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/display_update.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/display_update.c:display_update (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-02 — viewport_setup
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/viewport_setup.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/viewport_setup.c:viewport_setup (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-02 — display_mode_tick
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/display_mode_tick.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/display_mode_tick.c:display_mode_tick (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-02 — get_tv_offset
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/get_tv_offset.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/get_tv_offset.c:get_tv_offset (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 get_tv_offset(void) { s32 offset; if(osTvType==1) offset=0; else if(osTvType==0) offset=14; else offset=28; return offset; }

/* PROMOTED 2026-10-02 — apply_display_mode
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/apply_display_mode.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/apply_display_mode.c:apply_display_mode (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void apply_display_mode(void) { osSetThreadPri(gViewportStruct); }

/* PROMOTED 2026-10-02 — get_viewport_pos
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/get_viewport_pos.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/get_viewport_pos.c:get_viewport_pos (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void get_viewport_pos(register s32 *x, register s32 *y) {
    *x = ((s16 (*)[4])gViewportOffsetX)[gViewportX][0];
    *y = ((s16 (*)[4])gViewportOffsetY)[gViewportX][0];
}

/* PROMOTED 2026-10-02 — get_viewport_offset
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/get_viewport_offset.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/get_viewport_offset.c:get_viewport_offset (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void get_viewport_offset(register s32 *x, register s32 *y) {
    *x = gViewportScaleX - ((s16 (*)[4])gViewportOffsetX)[gViewportX][0];
    *y = gViewportScaleY - ((s16 (*)[4])gViewportOffsetY)[gViewportX][0];
}

/* PROMOTED 2026-10-02 — update_viewport
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/update_viewport.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/update_viewport.c:update_viewport (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void update_viewport(s32 x, s32 y) {
    s32 left, top, right, bottom;
    if (gViewportDataPtr == NULL) return;
    left = gViewportLeftEdge + x;
    top = gViewportTopEdge + y;
    right = gViewportRightEdge + x + gViewportXOverflow;
    bottom = gViewportBottomEdge + y + gViewportYOverflow;
    if (left > gViewportBoundsTable[gViewportX][0] && left < gViewportBoundsTable[gViewportX][1] && right > gViewportBoundsTable[gViewportX][2] && right < gViewportBoundsTable[gViewportX][3]) {
        gViewportScaleX = ((s16 (*)[4])gViewportOffsetX)[gViewportX][0] + x;
    }
    if (top > gViewportBoundsTable[gViewportX][4] && top < gViewportBoundsTable[gViewportX][5] && bottom > gViewportBoundsTable[gViewportX][6] && bottom < gViewportBoundsTable[gViewportX][7]) {
        gViewportScaleY = ((s16 (*)[4])gViewportOffsetY)[gViewportX][0] + y;
    }
    display_update();
}

