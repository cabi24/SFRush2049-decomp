/* GENERATED ROM-aligned TU — segment 0x83d0 (rom/lib_83d0)
 * layout map 8e202c518ba0cd27c4ebae19cfb36351f7dc0bdffe5abc6989eebbe37fcf6574; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osViGetFramebuffer
 * Source:   cloud/work/static_C/osViGetFramebuffer.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/osViGetFramebuffer.c:osViGetFramebuffer (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void *osViGetFramebuffer(void) {
    u32 temp_a0;
    void *sp18;

    temp_a0 = __osDisableInt();
    sp18 = __osViContext->framep;
    __osRestoreInt(temp_a0);
    return sp18;
}

