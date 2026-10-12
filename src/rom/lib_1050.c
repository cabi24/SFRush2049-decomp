/* GENERATED ROM-aligned TU — segment 0x1f50 (rom/lib_1050)
 * layout map c8f0857aebf4ea1ed90dde16bb4db378a8281234d10bd7a742394c75635cc457; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */

#include "rom_tu.h"


#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viTickStart.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viEnableAccum.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viDisableAccum.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viUpdateTime.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viScheduleTick.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viAddTicks.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050/viGetTimeToDeadline.s")

/* PROMOTED 2026-10-01 — viDeadlinePassed
 * Source:   cloud/work/static_C/viDeadlinePassed.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul (w24s: segment flagset; re-locked
 *           against the 48-B target that absorbs the former 20-B "viStub" slot; was -g0 -O2)
 * Evidence: lock:cloud/work/static_C/viDeadlinePassed.c:viDeadlinePassed (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 viDeadlinePassed(void) {
    return (gViAccumTime - gViTickCounter) < 1;
}

