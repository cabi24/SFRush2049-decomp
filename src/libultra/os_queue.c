/**
 * @file os_queue.c
 * @brief Thread queue management
 *
 * Decompiled from asm/us/D580.s
 * Contains priority queue operations for thread scheduling
 *
 * Threads are stored in priority-ordered linked lists. The queue
 * head points to a dummy node, and threads are inserted based on
 * their priority value (higher priority = earlier in queue).
 */

#include "types.h"

/* Thread structure (partial - enough for queue operations) */
typedef struct OSThread {
    struct OSThread *next;      /* 0x00: Next thread in queue */
    s32 priority;               /* 0x04: Thread priority */
    struct OSThread **queue;    /* 0x08: Queue this thread belongs to */
    /* ... rest of thread context ... */
} OSThread;

/* External data */
extern OSThread *D_8002C3D8;    /* Active thread queue head */
extern OSThread *D_8002C3E0;    /* Currently running thread */

/**
 * Enqueue thread into priority queue
 * (func_8000D0C4 - __osEnqueueThread)
 *
 * Inserts a thread into a priority-ordered queue.
 * Threads with higher priority values come first.
 *
 * @param queue Pointer to queue head pointer
 * @param thread Thread to insert
 */
void __osEnqueueThread(OSThread **queue, OSThread *thread) {
    OSThread *pred;     /* Predecessor in queue */
    OSThread *curr;     /* Current position */
    s32 pri;            /* Thread priority */

    pri = thread->priority;
    pred = (OSThread *)queue;  /* Start at head (cast for linked list traversal) */
    curr = *queue;

    /* Find insertion point - walk until we find lower priority */
    while (curr->priority >= pri) {
        pred = curr;
        curr = curr->next;
    }

    /* Insert thread after pred */
    thread->next = pred->next;
    pred->next = thread;
    thread->queue = queue;
}

/**
 * Pop first thread from queue
 * (func_8000D10C - __osPopThread)
 *
 * Removes and returns the first (highest priority) thread
 * from the queue.
 *
 * @param queue Pointer to queue head pointer
 * @return Removed thread
 */
OSThread *__osPopThread(OSThread **queue) {
    OSThread *thread;

    thread = *queue;
    *queue = thread->next;

    return thread;
}

/**
 * Dispatch next thread (context switch)
 * (func_8000D11C - __osDispatchThread)
 *
 * Pops the highest priority thread from the active queue
 * and performs a context switch to it. This function does
 * not return - it restores the thread's saved register state
 * and jumps to its program counter.
 *
 * This is a handwritten assembly function that:
 * 1. Pops thread from D_8002C3D8 (active queue)
 * 2. Stores it in D_8002C3E0 (current thread)
 * 3. Sets thread state to RUNNING (4)
 * 4. Restores all 64-bit GPRs from thread context
 * 5. Restores CP0 Status and EPC registers
 * 6. Optionally restores FPU state if thread used FPU
 * 7. Executes ERET to return to thread's saved PC
 */
void __osDispatchThread(void);
/* Implementation in assembly - restores full thread context */
