/**
 * @file os_yield.c
 * @brief Thread yield and priority functions
 *
 * Decompiled from asm/us/C990.s
 */

#include "types.h"
#include "PR/os_thread.h"

/* External functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
extern void func_8000CFC4(s32 runnable);  /* Switch thread */
extern void func_8000C050(OSThread **queue, OSThread *thread);  /* Remove from queue */

/* External data */
extern OSThread *D_8002C3E0;  /* Current running thread */

/**
 * Yield current thread
 * (func_8000BD90 - osYieldThread)
 *
 * Causes the current thread to yield the CPU.
 *
 * @param thread Thread to yield, or NULL for current
 */
void osYieldThread(OSThread *thread) {
    s32 saved;
    u16 state;

    saved = __osDisableInt();

    if (thread == NULL) {
        state = OS_STATE_RUNNING;
    } else {
        state = thread->state;
    }

    switch (state) {
        case OS_STATE_RUNNABLE:
        case OS_STATE_WAITING:
            /* Thread in queue - mark as stopped */
            thread->state = OS_STATE_STOPPED;
            func_8000C050(thread->queue, thread);
            break;

        case OS_STATE_RUNNING:
            /* Current thread - switch to idle */
            D_8002C3E0->state = OS_STATE_STOPPED;
            func_8000CFC4(0);
            break;
    }

    __osRestoreInt(saved);
}

/**
 * Get thread priority
 * (func_8000BE50 - osGetThreadPri)
 *
 * Returns the priority of a thread.
 *
 * @param thread Thread to query, or NULL for current
 * @return Thread priority
 */
s32 osGetThreadPri(OSThread *thread) {
    if (thread == NULL) {
        thread = D_8002C3E0;
    }
    return thread->priority;
}
