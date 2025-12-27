/**
 * @file os_int.c
 * @brief Interrupt management functions
 *
 * Decompiled from asm/us/D0B0.s
 * Contains low-level interrupt enable/disable functions
 *
 * These are handwritten assembly functions that directly manipulate
 * the MIPS R4300i CP0 Status register ($12).
 */

#include "types.h"
#include "PR/os_thread.h"

/* External data */
extern u32 __osGlobalIntMask;      /* Global interrupt mask storage */
extern OSThread *__osRunningThread; /* Currently executing thread */

/**
 * Disable interrupts
 * (func_8000C4B0 - __osDisableInt)
 *
 * Disables all CPU interrupts by clearing the IE bit (bit 0) in
 * the CP0 Status register. Returns the previous IE state so it
 * can be restored later.
 *
 * If the interrupt mask has changed since the last check, updates
 * the current thread's status register accordingly.
 *
 * @return Previous interrupt enable state (0 or 1)
 *
 * Assembly implementation:
 * - Reads current interrupt mask from __osGlobalIntMask
 * - mfc0 $t0, $12 (read Status register)
 * - andi $t1, $t0, 0xFFFE (clear IE bit)
 * - mtc0 $t1, $12 (write Status register)
 * - Returns original IE bit
 */
/* This is implemented in assembly - stub for reference */
#if 0
s32 __osDisableInt(void) {
    u32 prev_mask;
    u32 status;
    u32 new_mask;
    OSThread *thread;

    /* Read current mask */
    prev_mask = __osGlobalIntMask & 0xFF00;

    /* Read and modify Status register */
    status = __mfc0(12);  /* Get Status register */
    __mtc0(12, status & ~1);  /* Clear IE bit */

    /* Check if mask changed */
    new_mask = __osGlobalIntMask & 0xFF00;
    if (new_mask != prev_mask) {
        /* Update thread's status register */
        thread = __osRunningThread;
        u32 thread_sr = thread->context.sr;
        thread_sr = (thread_sr & 0xFFFF00FF) | (new_mask & prev_mask);
        thread_sr &= ~1;  /* Keep IE cleared */
        __mtc0(12, thread_sr);
    }

    return status & 1;  /* Return previous IE state */
}
#endif

/**
 * Restore interrupts
 * (func_8000C520 - __osRestoreInt)
 *
 * Restores the interrupt enable state by OR-ing the provided value
 * into the CP0 Status register.
 *
 * @param mask Previous interrupt state (from __osDisableInt)
 *
 * Assembly implementation:
 * - mfc0 $t0, $12 (read Status register)
 * - or $t0, $t0, $a0 (OR in the mask)
 * - mtc0 $t0, $12 (write Status register)
 */
/* This is implemented in assembly - stub for reference */
#if 0
void __osRestoreInt(s32 mask) {
    u32 status;

    status = __mfc0(12);  /* Get Status register */
    status |= mask;        /* Restore IE bit if it was set */
    __mtc0(12, status);    /* Write Status register */
}
#endif
