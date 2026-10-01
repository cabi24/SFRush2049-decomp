/* GENERATED ROM-aligned TU — segment 0x8330 (rom/lib_8330)
 * layout map 90c062168a4e29eeb44d9521ab322e206373d920a766a0ba15c0fbd3ceaf252d; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osSpTaskYielded
 * Source:   cloud/work/static_C2/osSpTaskYielded.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C2/osSpTaskYielded.c:osSpTaskYielded (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
OSYieldResult osSpTaskYielded(OSTask* tp) {
    u32 status;
    OSYieldResult result;

    status = bzero_alt();
    result = (status & SP_STATUS_YIELDED) ? OS_TASK_YIELDED : 0;

    if (status & SP_STATUS_YIELD) {
        tp->t.flags |= result;
        tp->t.flags &= ~(OS_TASK_DP_WAIT);
    }

    return result;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8330/osViGetCurrentFramebuffer.s")
