/* GENERATED ROM-aligned TU — segment 0x107a0 (rom/lib_107a0)
 * layout map 95484f236a87e8c9338a26bf5b05b74f3bad92c0711f303c4f8ccef655891a46; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — osDestroyThread
 * Source:   cloud/work/frontier/w16g/sources/osDestroyThread.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/osDestroyThread.c:osDestroyThread (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O1.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#define NULL ((void *)0)
#define OS_STATE_STOPPED 1
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 flags);
extern void dll_remove(OSThread **queue, OSThread *thread);
extern void __osDispatchThread(void);
extern OSThread *__osRunningThread;
extern OSThread *__osRunQueue;
void osDestroyThread(OSThread* t) {
    register u32 saveMask;
    register OSThread* pred;
    register OSThread* succ;

    saveMask = __osDisableInt();

    if (t == NULL) {
        t = __osRunningThread;
    } else if (t->state != OS_STATE_STOPPED) {
        dll_remove(t->queue, t);
    }

    if (__osRunQueue == t) {
        __osRunQueue = __osRunQueue->tlnext;
    } else {
        pred = __osRunQueue;
        while (pred->priority != -1) {
            succ = pred->tlnext;
            if (succ == t) {
                pred->tlnext = t->tlnext;
                break;
            }
            pred = succ;
        }
    }

    if (t == __osRunningThread) {
        __osDispatchThread();
    }

    __osRestoreInt(saveMask);
}

