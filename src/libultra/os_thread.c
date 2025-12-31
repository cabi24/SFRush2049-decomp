/**
 * @file os_thread.c
 * @brief Thread management functions
 *
 * Decompiled from asm/us/7B30.s, 7C80.s, 7940.s
 * Contains thread creation, starting, and priority management
 */

#include "types.h"
#include "PR/os_thread.h"
#include "PR/os_message.h"

/* External functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
extern void __osEnqueueThread(OSThread **queue, OSThread *thread);
extern OSThread *__osPopThread(OSThread **queue);
extern void __osDispatchThread(void);

/* External thread queue pointers */
extern OSThread **__osActiveQueue;    /* Active thread queue */
extern OSThread *__osThreadList;      /* Thread list head (last created) */
extern OSThread *__osRunningThread;   /* Currently running thread */
extern OSThread *__osViThread;        /* VI manager thread pointer */

/* Exception return address */
extern u32 __osExceptionReturn;

/**
 * Set thread priority
 * (0x80006D40 - osSetThreadPri)
 *
 * Updates the priority of a thread and re-enqueues it
 * in the active thread queue.
 *
 * @param thread Thread to modify
 * @param priority New priority value
 * @return Previous priority value
 */
OSPri osSetThreadPri(OSThread *thread, OSPri priority) {
    s32 saved;
    OSPri old_priority;

    saved = __osDisableInt();

    /* Get old priority */
    old_priority = thread->priority;

    /* Set new priority */
    thread->priority = priority;

    /* Mark as runnable */
    thread->state = OS_STATE_RUNNABLE;

    /* Re-enqueue to update queue position */
    __osEnqueueThread(thread->queue, thread);

    __osRestoreInt(saved);

    return old_priority;
}

/**
 * Set interrupt mask
 * (0x80006DA0 - osSetIntMask)
 *
 * Enables or disables interrupts globally.
 *
 * @param mask 0 = disable, non-zero = enable
 */
void osSetIntMask(s32 mask) {
    s32 saved;
    OSThread *thread;

    saved = __osDisableInt();

    if ((u8)mask != 0) {
        /* Enable interrupts - set bit 5 of flags */
        thread = __osViThread;
        *(u16 *)thread = *(u16 *)thread | 0x20;
    } else {
        /* Disable interrupts - clear bit 5 of flags */
        thread = __osViThread;
        *(u16 *)thread = *(u16 *)thread & ~0x20;
    }

    __osRestoreInt(saved);
}

/**
 * Create a new thread
 * (0x80006F30 - osCreateThread)
 *
 * Initializes a thread structure with entry point, stack, and priority.
 *
 * @param thread Pointer to thread structure to initialize
 * @param id Thread ID (0-7 typically)
 * @param entry Thread entry function
 * @param arg Argument passed to entry function
 * @param stack Top of thread's stack
 * @param priority Thread priority (higher = more priority)
 */
void osCreateThread(OSThread *thread, s32 id, void (*entry)(void *),
                    void *arg, void *stack, s32 priority) {
    s32 saved;

    /* Store thread ID */
    thread->id = id;

    /* Store priority */
    thread->priority = priority;

    /* Clear next pointer */
    thread->next = NULL;

    /* Clear queue pointer */
    thread->queue = NULL;

    /* Store entry point */
    thread->context.pc = (u32)entry;

    /* Set argument register a0 */
    thread->context.a0 = (u64)(u32)arg;

    /* Set up stack pointer (sp = stack - 16, for 16 byte alignment) */
    thread->context.sp = (u64)((u32)stack - 16);

    /* Set return address to exception return point */
    thread->context.ra = (u64)(u32)&__osExceptionReturn;

    /* Set FP control register */
    thread->context.fpcsr = 0xFF03;

    /* Set initial status register value */
    thread->context.sr = 0x3F;  /* Enable all interrupts */

    /* Set cause register */
    thread->context.cause = 0x1000800;

    /* Clear FP used flag */
    thread->fp = 0;

    /* Set state to stopped */
    *(u16 *)&thread->state = OS_STATE_STOPPED;

    /* Clear running flag */
    *(u16 *)((u8 *)&thread->state + 2) = 0;

    saved = __osDisableInt();

    /* Link into thread list */
    thread->tlnext = __osThreadList;
    __osThreadList = thread;

    __osRestoreInt(saved);
}

/**
 * Start a thread
 * (0x80007080 - osStartThread)
 *
 * Transitions a thread from stopped to runnable state and
 * enqueues it for scheduling.
 *
 * @param thread Thread to start
 */
void osStartThread(OSThread *thread) {
    s32 saved;
    u16 state;
    OSThread *popped;

    saved = __osDisableInt();

    state = *(u16 *)&thread->state;

    if (state == OS_STATE_STOPPED) {
        /* Thread was just created */
        if (thread->queue == NULL || thread->queue == __osActiveQueue) {
            /* No queue or on active queue - just make runnable */
            *(u16 *)&thread->state = OS_STATE_RUNNABLE;
            __osEnqueueThread(__osActiveQueue, thread);
        } else {
            /* Thread was waiting on another queue */
            *(u16 *)&thread->state = OS_STATE_WAITING;
            __osEnqueueThread(thread->queue, thread);

            /* Pop from that queue and add to active */
            popped = __osPopThread(thread->queue);
            __osEnqueueThread(__osActiveQueue, popped);
        }
    } else if (state == OS_STATE_WAITING) {
        /* Thread was waiting - make runnable */
        *(u16 *)&thread->state = OS_STATE_RUNNABLE;
        __osEnqueueThread(__osActiveQueue, thread);
    }

    /* Check if we need to dispatch */
    if (__osRunningThread == NULL) {
        /* No current thread - dispatch now */
        __osDispatchThread();
    } else {
        /* Compare priorities */
        OSThread *active = *__osActiveQueue;
        if (__osRunningThread->priority < active->priority) {
            /* New thread has higher priority - preempt */
            *(u16 *)&__osRunningThread->state = OS_STATE_RUNNABLE;
            __osEnqueueThread(__osActiveQueue, __osRunningThread);
        }
    }

    __osRestoreInt(saved);
}

/* Additional external functions */
extern void __osSwitchThread(s32 runnable);

/**
 * Yield the current thread
 * (0x8000BD90 - osYieldThread)
 *
 * Causes the current thread to voluntarily give up the CPU.
 * If thread is NULL, yields current thread to idle.
 * Otherwise handles thread state transitions.
 *
 * @param thread Thread to yield, or NULL for current
 */
void osYieldThread(OSThread *thread) {
    s32 saved;
    u16 state;

    saved = __osDisableInt();

    if (thread == NULL) {
        state = OS_STATE_RUNNING;  /* 4 */
    } else {
        state = *(u16 *)&thread->state;
    }

    switch (state) {
        case OS_STATE_RUNNING:  /* 4 */
            /* Current thread - just switch to idle */
            *(u16 *)&__osRunningThread->state = OS_STATE_STOPPED;
            __osSwitchThread(0);
            break;

        case OS_STATE_RUNNABLE:  /* 2 */
        case OS_STATE_WAITING:   /* 8 */
            /* Thread in queue - mark as stopped and re-enqueue */
            *(u16 *)&thread->state = OS_STATE_STOPPED;
            __osEnqueueThread(thread->queue, thread);
            break;
    }

    __osRestoreInt(saved);
}

/**
 * Get thread priority
 * (0x8000BE50 - osGetThreadPri)
 *
 * Returns the priority of the specified thread.
 * If thread is NULL, returns current thread's priority.
 *
 * @param thread Thread to query, or NULL for current
 * @return Thread priority value
 */
OSPri osGetThreadPri(OSThread *thread) {
    OSThread *t;

    t = thread;
    if (t == NULL) {
        t = __osRunningThread;
    }
    return t->priority;
}

/**
 * Get thread ID
 * (0x8000C490 - osGetThreadId)
 *
 * Returns the ID of the specified thread.
 * If thread is NULL, returns current thread's ID.
 *
 * @param thread Thread to query, or NULL for current
 * @return Thread ID value
 */
OSId osGetThreadId(OSThread *thread) {
    OSThread *t;

    t = thread;
    if (t == NULL) {
        t = __osRunningThread;
    }
    return t->id;
}
