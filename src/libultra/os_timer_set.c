/**
 * @file os_timer_set.c
 * @brief Timer initialization function
 *
 * Decompiled from asm/us/EFD0.s
 */

#include "types.h"

/* External data */
extern void *D_8002C3F0;  /* First timer in list */

/* External functions */
extern u64 func_8000C308(void *timer);  /* Insert timer into list */
extern void func_8000C294(u32 hi, u32 lo);  /* Set compare register */

/**
 * Initialize and start a timer
 * (func_8000E3D0 - osSetTimer)
 *
 * Sets up a timer structure and inserts it into the timer list.
 *
 * @param timer Timer structure to initialize
 * @param countdown Initial countdown value (unused if interval set)
 * @param interval Timer interval (0 for one-shot)
 * @param mq Message queue for timer events (can be NULL)
 * @param msg Message to send on timer expiry (can be NULL)
 * @return Always returns 0
 */
s32 osSetTimer(void *timer, u64 countdown, u64 interval, void *mq, void *msg) {
    u8 *t = (u8 *)timer;
    u32 hi, lo;

    /* Clear next/prev pointers */
    *(u32 *)(t + 0) = 0;
    *(u32 *)(t + 4) = 0;

    /* Store interval */
    *(u32 *)(t + 8) = (u32)(interval >> 32);
    *(u32 *)(t + 0xC) = (u32)interval;

    /* Store countdown or use interval if countdown is 0 */
    if (countdown != 0) {
        *(u32 *)(t + 0x10) = (u32)(countdown >> 32);
        *(u32 *)(t + 0x14) = (u32)countdown;
    } else {
        *(u32 *)(t + 0x10) = (u32)(interval >> 32);
        *(u32 *)(t + 0x14) = (u32)interval;
    }

    /* Store message queue and message */
    *(void **)(t + 0x18) = mq;
    *(void **)(t + 0x1C) = msg;

    /* Insert timer into list and get time value */
    {
        u64 result = func_8000C308(timer);
        hi = (u32)(result >> 32);
        lo = (u32)result;
    }

    /* If this is the first timer, set compare register */
    if (*(void **)D_8002C3F0 == timer) {
        func_8000C294(hi, lo);
    }

    return 0;
}
