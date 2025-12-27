/**
 * @file os_time.h
 * @brief N64 Timer and time management definitions
 *
 * Based on Nintendo SDK headers and reference decomp projects.
 */

#ifndef _OS_TIME_H_
#define _OS_TIME_H_

#include "types.h"
#include "PR/os_message.h"

/**
 * Time value type (64-bit counter ticks)
 */
typedef u64 OSTime;

/**
 * Interval Timer Structure
 *
 * Used for scheduling timed callbacks via message queues.
 */
typedef struct OSTimer {
    struct OSTimer *next;       /* Next timer in list */
    struct OSTimer *prev;       /* Previous timer in list */
    OSTime interval;            /* Timer interval (0 for one-shot) */
    OSTime value;               /* Time remaining before timer fires */
    OSMesgQueue *mq;            /* Message queue for timer events */
    OSMesg msg;                 /* Message to send on timer expiry */
} OSTimer;

/* Function prototypes */
OSTime osGetTime(void);
void osSetTime(OSTime time);
s32 osSetTimer(OSTimer *timer, OSTime countdown, OSTime interval,
               OSMesgQueue *mq, OSMesg msg);
s32 osStopTimer(OSTimer *timer);

#endif /* _OS_TIME_H_ */
