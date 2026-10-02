/* GENERATED ROM-aligned TU — segment 0x8190 (rom/lib_8040)
 * layout map fd8409c21b34dc7bbde95e98487864907c2fad0185f37de73dcc6e618d72f33e; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */

#include "rom_tu.h"



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
