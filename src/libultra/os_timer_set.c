/**
 * @file os_timer_set.c
 * @brief Timer initialization function
 *
 * Decompiled from asm/us/EFD0.s
 *
 * Timers are used for scheduling timed callbacks and delays.
 * They work by inserting into a sorted list and using the
 * CPU's compare register for the next expiration.
 */

#include "types.h"
#include "PR/os_time.h"

/* External data */
extern OSTimer *__osTimerList;  /* D_8002C3F0: First timer in list */

/* External functions */
extern OSTime __osInsertTimer(OSTimer *timer);          /* func_8000C308 */
extern void __osSetCompare(u32 hi, u32 lo);             /* func_8000C294 */

/**
 * Initialize and start a timer
 * (func_8000E3D0 - osSetTimer)
 *
 * Sets up a timer structure and inserts it into the timer list.
 * When the timer expires, a message is sent to the specified queue.
 *
 * @param timer Timer structure to initialize
 * @param countdown Initial countdown value (0 to use interval)
 * @param interval Timer interval for repeating (0 for one-shot)
 * @param mq Message queue for timer events (can be NULL)
 * @param msg Message to send on timer expiry (can be NULL)
 * @return 0 on success
 */
s32 osSetTimer(OSTimer *timer, OSTime countdown, OSTime interval,
               OSMesgQueue *mq, OSMesg msg) {
    OSTime result;
    u32 hi, lo;

    /* Clear next/prev pointers */
    timer->next = NULL;
    timer->prev = NULL;

    /* Store interval */
    timer->interval = interval;

    /* Store countdown, using interval if countdown is 0 */
    if (countdown != 0) {
        timer->value = countdown;
    } else {
        timer->value = interval;
    }

    /* Store message queue and message */
    timer->mq = mq;
    timer->msg = msg;

    /* Insert timer into sorted list */
    result = __osInsertTimer(timer);
    hi = (u32)(result >> 32);
    lo = (u32)result;

    /* If this is the first timer, set CPU compare register */
    if (__osTimerList == timer) {
        __osSetCompare(hi, lo);
    }

    return 0;
}
