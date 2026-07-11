/* GENERATED ROM-aligned TU — segment 0xe250 (rom/lib_e250)
 * layout map dca9a642c868dbc1bb5a031766796d4fad107c99586a327e639525eec050647e; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-07-11 — __osSpSetPc
 * Source:   src/libultra/os_sp.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/libultra/os_sp.c:__osSpSetPc (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osSpSetPc(u32 pc) {
    u32 status;

    status = SP_STATUS_REG;

    /* Check if RSP is halted */
    if ((status & SP_STATUS_HALT) == 0) {
        return -1;
    }

    /* Set program counter */
    SP_PC_REG = pc;
    return 0;
}

