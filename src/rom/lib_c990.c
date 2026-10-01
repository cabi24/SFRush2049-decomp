/* GENERATED ROM-aligned TU — segment 0xc990 (rom/lib_c990)
 * layout map 3d7225dbf25a7b232ed775c14d1337f15392518bd99b81ab2c8ecdb8fdd031aa; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_c990/osPfsChecker_full.s")
/* PROMOTED 2026-10-01 — osPfsReAllocate
 * Source:   cloud/work/static_C6/osPfsReAllocate.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C6/osPfsReAllocate.c:osPfsReAllocate (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
OSId osPfsReAllocate(OSThread* thread) {
    if (thread == NULL) {
        thread = __osRunningThread;
    }

    return thread->id;
}

