/* GENERATED ROM-aligned TU — segment 0xe240 (rom/lib_e240)
 * layout map 574b1d74524f1b7c5783607ded27a53e2b3271af6246554b4bd8061c261a7c23; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-07-11 — __osSpSetStatus
 * Source:   src/libultra/os_sp.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:src/libultra/os_sp.c:__osSpSetStatus (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __osSpSetStatus(u32 status) {
    SP_STATUS_REG = status;
}

