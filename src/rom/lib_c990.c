/* GENERATED ROM-aligned TU — segment 0xc990 (rom/lib_c990)
 * layout map 3d7225dbf25a7b232ed775c14d1337f15392518bd99b81ab2c8ecdb8fdd031aa; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osPfsChecker_full
 * Source:   cloud/work/static_C11/osPfsChecker_full.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C11/osPfsChecker_full.c:osPfsChecker_full (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osPfsChecker_full(OSThread* t) {
    register u32 saveMask = __osDisableInt();
    register u16 state;

    state = (t == NULL) ? OS_STATE_RUNNING: t->state;

    switch (state) {
        case OS_STATE_RUNNING:
            __osRunningThread->state = OS_STATE_STOPPED;
            __osCleanupThread(NULL);
            break;
        case OS_STATE_RUNNABLE:
        case OS_STATE_WAITING:
            t->state = OS_STATE_STOPPED;
            dll_remove(t->queue, t);
            break;
    }

    __osRestoreInt(saveMask);
}

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

