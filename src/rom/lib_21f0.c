/* GENERATED ROM-aligned TU — segment 0x21f0 (rom/lib_21f0)
 * layout map fe94ca7f42eb4854cf5b13eed87a4b511f52b70215182a56919a592137bfae27; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "static_debug_context.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21f0/display_update.s")
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

