/**
 * @file os_thread_pri.c
 * @brief Thread priority query function
 *
 * Decompiled from asm/us/83D0.s
 */

#include "types.h"
#include "PR/os_thread.h"

/* External functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);

/* External data - uses __osRunningThread from os_thread.h */

/**
 * Get current interrupt mask
 * (func_800077D0)
 *
 * Returns the interrupt mask from the current thread's context.
 *
 * @return Current interrupt mask value
 */
s32 __osGetIntMask(void) {
    s32 saved;
    s32 mask;

    saved = __osDisableInt();

    /* Get interrupt mask from thread priority field */
    mask = __osRunningThread->priority;

    __osRestoreInt(saved);

    return mask;
}
