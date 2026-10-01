/* GENERATED ROM-aligned TU — segment 0x8e10 (rom/lib_8e10)
 * layout map 786dc4a0d32a138c634e46bb162149d1f56f97ae353a3cda4339e9d88e027ea7; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8e10/osCreatePiManager.s")
/* PROMOTED 2026-10-01 — osCreateViManager
 * Source:   cloud/work/static_C8/osCreateViManager.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C8/osCreateViManager.c:osCreateViManager (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osCreateViManager(OSThread* t, OSPri pri) {
    register u32 saveMask;


    saveMask = __osDisableInt();

    if (t == NULL) {
        t = __osRunningThread;
    }

    if (t->priority != pri) {
        t->priority = pri;

        if (t != __osRunningThread && t->state != OS_STATE_STOPPED) {
            dll_remove(t->queue, t);
            __osEnqueueThread(t->queue, t);
        }

        if (__osRunningThread->priority < ((OSThread *)__osActiveQueue)->priority) {
            __osRunningThread->state = OS_STATE_RUNNABLE;
            __osCleanupThread((OSThread **) &__osActiveQueue);
        }
    }

    __osRestoreInt(saveMask);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8e10/osInvalICache_full.s")
