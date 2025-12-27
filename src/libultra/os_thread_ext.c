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
 * Thread wait function
 * (func_80007840)
 */
void func_80007840(void *queue) {
    /* Thread wait - stub */
    (void)queue;
}

/**
 * Thread signal function
 * (func_8000794C)
 */
void func_8000794C(void *queue) {
    /* Thread signal - stub */
    (void)queue;
}

/**
 * Thread destroy function
 * (func_80007AB4)
 */
void func_80007AB4(OSThread *thread) {
    /* Thread destroy - stub */
    (void)thread;
}
