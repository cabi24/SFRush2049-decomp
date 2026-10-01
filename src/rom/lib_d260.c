/* GENERATED ROM-aligned TU — segment 0xd260 (rom/lib_d260)
 * layout map f8e7304fe78f87eaf0b01132b0c5140f473ace785bdf961a8141e94d2f0ccccb; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osGetActiveQueue
 * Source:   cloud/work/static_C/osGetActiveQueue.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/osGetActiveQueue.c:osGetActiveQueue (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osGetActiveQueue(void) {
    return __osViModeInfo;
}

