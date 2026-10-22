/* GENERATED ROM-aligned TU — segment 0x10760 (rom/lib_10760)
 * layout map 9e2c65f3e1cf4c50cf2499113c3b008f660e524f91ebe0e810c8fb684c1efddc; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-21 — __osAiDeviceBusy
 * Source:   src/libultra/os_ai.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/libultra/os_ai.c:__osAiDeviceBusy (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osAiDeviceBusy(void) {
    register s32 status = AI_STATUS_REG;

    if (status & AI_STATUS_FIFO_FULL) {
        return 1;
    } else {
        return 0;
    }
}

