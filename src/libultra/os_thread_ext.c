/**
 * @file os_thread_ext.c
 * @brief Extended thread management functions
 *
 * Decompiled from asm/us/8440.s
 */

#include "types.h"
#include "PR/os_thread.h"

/* External functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);

/**
 * Wait on thread queue
 * (func_80007840 - osThreadWait)
 *
 * Blocks the calling thread until signaled.
 *
 * @param queue Thread wait queue
 */
void osThreadWait(void *queue) {
    /* Thread wait - stub */
    (void)queue;
}

/**
 * Signal thread on queue
 * (func_8000794C - osThreadSignal)
 *
 * Wakes up a thread waiting on the queue.
 *
 * @param queue Thread wait queue
 */
void osThreadSignal(void *queue) {
    /* Thread signal - stub */
    (void)queue;
}

/**
 * Destroy a thread
 * (func_80007AB4 - osDestroyThread)
 *
 * Removes a thread from the system and frees its resources.
 *
 * @param thread Thread to destroy
 */
void osDestroyThread(OSThread *thread) {
    /* Thread destroy - stub */
    (void)thread;
}
