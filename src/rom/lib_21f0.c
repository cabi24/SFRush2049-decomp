/* GENERATED ROM-aligned TU — segment 0x21f0 (rom/lib_21f0)
 * layout map fe94ca7f42eb4854cf5b13eed87a4b511f52b70215182a56919a592137bfae27; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "static_debug_context.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21f0/display_update.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21f0/viewport_setup.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21f0/display_mode_tick.s")
/* PROMOTED 2026-10-02 — get_tv_offset
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/get_tv_offset.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/get_tv_offset.c:get_tv_offset (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 get_tv_offset(void) { s32 offset; if(osTvType==1) offset=0; else if(osTvType==0) offset=14; else offset=28; return offset; }

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21f0/apply_display_mode.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_21f0/update_viewport.s")
