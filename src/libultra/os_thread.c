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
extern OSThread **D_8002C3D8;     /* Active thread queue */
extern OSThread *D_8002C3DC;      /* Last created thread */
extern OSThread *D_8002C3E0;      /* Current running thread */
extern OSThread *D_8002C464;      /* System thread pointer */

/* Exception return address */
extern u32 D_8000D298;

/**
 * Set thread priority
 * (func_80006D40 - osSetThreadPri)
 *
 * Updates the priority of a thread and re-enqueues it
 * in the active thread queue.
 *
 * @param thread Thread to modify (via D_8002C464 indirection)
 */
void osSetThreadPri(OSThread *thread) {
    s32 saved;
    OSThread *target;
    OSThread *sys;

    saved = __osDisableInt();

    /* Get system thread pointer */
    sys = D_8002C464;
    target = thread;

    /* Set new priority */
    sys->priority = target;

    /* Mark as runnable */
    *(u16 *)sys = 1;

    /* Update queue position based on priority */
    sys = D_8002C464;
    sys->queue = sys->priority->id;

    __osRestoreInt(saved);
}

/**
 * Set interrupt mask
 * (func_80006DA0 - osSetIntMask)
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
        thread = D_8002C464;
        *(u16 *)thread = *(u16 *)thread | 0x20;
    } else {
        /* Disable interrupts - clear bit 5 of flags */
        thread = D_8002C464;
        *(u16 *)thread = *(u16 *)thread & ~0x20;
    }

    __osRestoreInt(saved);
}

/**
 * Create a new thread
 * (func_80006F30 - osCreateThread)
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

    /* Sign-extend arg for 64-bit register */
    thread->context.gp[4] = (s32)arg >> 31;   /* a0 high */
    thread->context.gp[5] = (s32)arg;          /* a0 low */

    /* Set up stack pointer (sp = stack - 16, for 16 byte alignment) */
    thread->context.gp[28] = (s32)stack >> 31;  /* sp high */
    thread->context.gp[29] = (s32)stack - 16;   /* sp low */

    /* Set return address to exception return point */
    thread->context.gp[30] = (u32)&D_8000D298 >> 31;  /* ra high */
    thread->context.gp[31] = (u32)&D_8000D298;        /* ra low */

    /* Set FP control register */
    thread->context.fpcsr = 0xFF03;

    /* Set initial status register value */
    thread->context.sr = 0x3F;  /* Enable all interrupts */

    /* Set cause register */
    thread->context.cause = 0x1000800;

    /* Clear flags */
    thread->fp = NULL;

    /* Set state to stopped */
    *(u16 *)&thread->state = OS_STATE_STOPPED;

    /* Clear running flag */
    *(u16 *)((u8 *)&thread->state + 2) = 0;

    saved = __osDisableInt();

    /* Link into thread list */
    thread->tlnext = D_8002C3DC;
    D_8002C3DC = thread;

    __osRestoreInt(saved);
}

/**
 * Start a thread
 * (func_80007080 - osStartThread)
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
        if (thread->queue == NULL || thread->queue == D_8002C3D8) {
            /* No queue or on active queue - just make runnable */
            *(u16 *)&thread->state = OS_STATE_RUNNABLE;
            __osEnqueueThread(D_8002C3D8, thread);
        } else {
            /* Thread was waiting on another queue */
            *(u16 *)&thread->state = OS_STATE_WAITING;
            __osEnqueueThread(thread->queue, thread);

            /* Pop from that queue and add to active */
            popped = __osPopThread(thread->queue);
            __osEnqueueThread(D_8002C3D8, popped);
        }
    } else if (state == OS_STATE_WAITING) {
        /* Thread was waiting - make runnable */
        *(u16 *)&thread->state = OS_STATE_RUNNABLE;
        __osEnqueueThread(D_8002C3D8, thread);
    }

    /* Check if we need to dispatch */
    if (D_8002C3E0 == NULL) {
        /* No current thread - dispatch now */
        __osDispatchThread();
    } else {
        /* Compare priorities */
        OSThread *active = *D_8002C3D8;
        if (D_8002C3E0->priority < active->priority) {
            /* New thread has higher priority - preempt */
            *(u16 *)&D_8002C3E0->state = OS_STATE_RUNNABLE;
            __osEnqueueThread(D_8002C3D8, D_8002C3E0);
        }
    }

    __osRestoreInt(saved);
}

/* Additional external functions */
extern void __osSwitchThread(s32 runnable);

/**
 * Yield the current thread
 * (func_8000BD90 - osYieldThread)
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
            *(u16 *)&D_8002C3E0->state = OS_STATE_STOPPED;
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
 * (func_8000BE50 - osGetThreadPri)
 *
 * Returns the priority of the specified thread.
 * If thread is NULL, returns current thread's priority.
 *
 * @param thread Thread to query, or NULL for current
 * @return Thread priority value
 */
s32 osGetThreadPri(OSThread *thread) {
    OSThread *t;

    t = thread;
    if (t == NULL) {
        t = D_8002C3E0;
    }
    return t->priority;
}

/**
 * Get thread ID
 * (func_8000C490 - osGetThreadId)
 *
 * Returns the ID of the specified thread.
 * If thread is NULL, returns current thread's ID.
 *
 * @param thread Thread to query, or NULL for current
 * @return Thread ID value
 */
s32 osGetThreadId(OSThread *thread) {
    OSThread *t;

    t = thread;
    if (t == NULL) {
        t = D_8002C3E0;
    }
    return t->id;
}
