/* GENERATED ROM-aligned TU — segment 0x7940 (rom/lib_7940)
 * layout map b0bc1fd0cfddbbfb907476d71e0def48cf8df1532d3bbcb1d35a79a3e58dbace; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osSetThreadPri
 * Source:   cloud/work/static_C4/osSetThreadPri.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C4/osSetThreadPri.c:osSetThreadPri (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osSetThreadPri(void *thread) {
    u32 temp_a0;

    temp_a0 = __osDisableInt();
    __osViContext->modep = (OSViMode *) thread;
    __osViContext->state = 1;
    __osViContext->control = __osViContext->modep->comRegs.ctrl;
    __osRestoreInt(temp_a0);
}

