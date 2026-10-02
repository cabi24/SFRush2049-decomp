/* GENERATED ROM-aligned TU — segment 0x8f80 (rom/lib_8e10)
 * layout map e43dc4b4a3c354d6f15ccf2a8b4a1109788a77c969329ffb0f06c532bb840e9e; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */

#include "rom_tu.h"



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

