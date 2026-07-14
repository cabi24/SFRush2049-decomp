/* GENERATED ROM-aligned TU — segment 0x8040 (rom/lib_8040)
 * layout map cb779642bb3a2fda1dfc519e82dc8d1e7dd0e52567c634e5d852e95d32107bc9; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8040/osSendMesg.s")
/* PROMOTED 2026-07-15 — osViSetMode
 * Source:   src/rom_auto/osViSetMode.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/rom_auto/osViSetMode.c:osViSetMode (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osViSetMode(void *mode) {
    u32 temp_a0;

    temp_a0 = __osDisableInt();
    __osViContext->framep = mode;
    __osViContext->state |= 0x10;
    __osRestoreInt(temp_a0);
}

