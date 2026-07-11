/* GENERATED ROM-aligned TU — segment 0xa5b0 (rom/lib_a5b0)
 * layout map 46591d37c9eebc136dc2d9d51a456a72b51d94d6b3f1a375c4da69fe2771b92f; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-07-11 — osDpGetCounters
 * Source:   src/libultra/os_dp_counters.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/libultra/os_dp_counters.c:osDpGetCounters (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osDpGetCounters(u32 *array) {
    *array++ = DPC_CLOCK_REG;
    *array++ = DPC_BUFBUSY_REG;
    *array++ = DPC_PIPEBUSY_REG;
    *array++ = DPC_TMEM_REG;
}

