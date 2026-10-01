/* GENERATED ROM-aligned TU — segment 0xefd0 (rom/lib_efd0)
 * layout map 2a7f9866255757178a232bef3926568dc2e7e6952bf1b193104a6395c263892a; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osSetTimer
 * Source:   cloud/work/static_C3/osSetTimer.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C3/osSetTimer.c:osSetTimer (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
int osSetTimer(OSTimer* t, OSTime countdown, OSTime interval, OSMesgQueue* mq, OSMesg msg) {
    OSTime time;


    t->next = NULL;
    t->prev = NULL;
    t->interval = interval;
    t->value = (countdown != 0) ? countdown : interval;
    t->mq = mq;
    t->msg = msg;

    time = dll_insert(t);
    if (((OSTimer *)__osTimerList)->next == t) {
        dll_reschedule(time);
    }

    return 0;
}

