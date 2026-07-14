/* GENERATED ROM-aligned TU — segment 0x7ad0 (rom/lib_7ad0)
 * layout map 3b0ed6cdad75b84892fa6b11e89b729dcd4f24c2bda1c9057615f972830f42aa; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-07-15 — osSetTimerIntr
 * Source:   src/rom_auto/osSetTimerIntr.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/rom_auto/osSetTimerIntr.c:osSetTimerIntr (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osSetTimerIntr(OSMesgQueue *arg0, void *arg1, s32 arg2) {
    u32 temp_a0;

    temp_a0 = __osDisableInt();
    __osViContext->msgq = arg0;
    __osViContext->msg = arg1;
    __osViContext->retraceCount = (u16) arg2;
    __osRestoreInt(temp_a0);
}

