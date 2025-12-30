/**
 * @file os_yield.c
 * @brief Thread yield and priority functions
 *
 * Decompiled from asm/us/C990.s
 *
 * These functions handle voluntary thread preemption and
 * priority queries.
 */

#include "types.h"
#include "PR/os_thread.h"

/* External functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
extern void __osDispatchThread(void);                           /* func_8000CFC4 */
extern void __osDequeueThread(OSThread **queue, OSThread *t);   /* func_8000C050 */

/* External data - Current running thread (standard libultra name) */
extern OSThread *__osRunningThread;  /* 0x8002C3E0 */

/**
 * Yield/stop current thread
 * (func_8000BD90 - osStopThread)
 *
 * Causes a thread to stop executing. If thread is NULL, stops
 * the current running thread. The thread is removed from its
 * queue and marked as stopped.
 *
 * @param thread Thread to stop, or NULL for current
 */
void osStopThread(OSThread *thread) {
    s32 savedMask;
    u16 state;

    savedMask = __osDisableInt();

    if (thread == NULL) {
        state = OS_STATE_RUNNING;
    } else {
        state = thread->state;
    }

    switch (state) {
        case OS_STATE_RUNNABLE:
        case OS_STATE_WAITING:
            /* Thread in queue - mark as stopped and remove */
            thread->state = OS_STATE_STOPPED;
            __osDequeueThread(thread->queue, thread);
            break;

        case OS_STATE_RUNNING:
            /* Current thread - mark as stopped and dispatch */
            __osRunningThread->state = OS_STATE_STOPPED;
            __osDispatchThread();
            break;
    }

    __osRestoreInt(savedMask);
}

/**
 * Get thread priority
 * (func_8000BE50 - osGetThreadPri)
 *
 * Returns the priority of a thread.
 *
 * @param thread Thread to query, or NULL for current
 * @return Thread priority (higher = more important)
 */
OSPri osGetThreadPri(OSThread *thread) {
    if (thread == NULL) {
        thread = __osRunningThread;
    }
    return thread->priority;
}
