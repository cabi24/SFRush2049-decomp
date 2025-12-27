/**
 * @file os_timer.c
 * @brief Timer management functions
 *
 * Decompiled from asm/us/7A10.s, 7AD0.s, 7FB0.s
 * Contains timer setup and time retrieval functions
 */

#include "types.h"
#include "PR/os_thread.h"
#include "PR/os_message.h"

/* External functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
/* osSendMesg declared in os_message.h */

/* Timer slot indices */
#define OS_TIMER_SYSTEM     0x0E    /* System timer slot (14) */

/* External data - Timer event tables */
extern OSMesgQueue *__osTimerMsgQueue[];    /* Timer message queue table */
extern s32 __osTimerActive;                 /* Timer hardware active flag */
extern s32 __osTimerInitialized;            /* Timer system initialized flag */
extern OSThread *__osTimerThread;           /* Timer service thread */

/**
 * Set up a timer event
 * (func_80006E10 - osSetTimer)
 *
 * Configures a timer slot with a message queue and callback value.
 * Timer slot 0xE (14) has special handling for system initialization.
 *
 * @param timer Timer slot index (0-15)
 * @param mq Message queue to receive timer events
 * @param msg Message value to send on timer expiry
 */
void osSetTimer(s32 timer, OSMesgQueue *mq, s32 msg) {
    s32 saved;
    OSMesgQueue **slot;

    saved = __osDisableInt();

    /* Calculate timer slot address (8 bytes per slot) */
    slot = &__osTimerMsgQueue[timer];

    /* Store message queue and message */
    slot[0] = mq;
    *(s32 *)&slot[1] = msg;

    /* Special handling for system timer slot */
    if (timer == OS_TIMER_SYSTEM) {
        if (__osTimerActive != 0 && __osTimerInitialized == 0) {
            /* Timer is active but not initialized - send initial message */
            osSendMesg(mq, (OSMesg)msg, OS_MESG_NOBLOCK);
        }
        /* Mark timer as initialized */
        __osTimerInitialized = 1;
    }

    __osRestoreInt(saved);
}

/**
 * Set timer interrupt parameters
 * (func_80006ED0 - osSetTimerIntr)
 *
 * Configures timer interrupt for a thread.
 *
 * @param thread Target thread
 * @param mq Message queue for timer events
 * @param value Interrupt value/interval
 */
void osSetTimerIntr(OSThread *thread, OSMesgQueue *mq, s16 value) {
    s32 saved;
    u8 *timerCtx;

    saved = __osDisableInt();

    timerCtx = (u8 *)__osTimerThread;

    /* Store thread pointer at offset 0 */
    *(void **)(timerCtx + 0) = thread;

    timerCtx = (u8 *)__osTimerThread;
    /* Store message queue at offset 4 */
    *(void **)(timerCtx + 4) = mq;

    timerCtx = (u8 *)__osTimerThread;
    /* Store interrupt value at offset 2 */
    *(s16 *)(timerCtx + 2) = value;

    __osRestoreInt(saved);
}

/* External time tracking variables */
extern u32 __osTimeHi;      /* Accumulated time (high 32 bits) */
extern u32 __osTimeLo;      /* Accumulated time (low 32 bits) */
extern u32 __osBaseCount;   /* Last CP0 Count value */

/* External functions for time retrieval */
extern u32 osGetCount(void);      /* Read CP0 Count register */

/**
 * Get current system time
 * (func_800073B0 - osGetTime)
 *
 * Returns the current system time as a 64-bit value.
 * Combines the accumulated time with elapsed cycles since last update.
 *
 * The time is computed as:
 *   accumulated_time + (current_count - base_count)
 *
 * @return 64-bit system time in CPU cycles
 */
u64 osGetTime(void) {
    s32 savedMask;
    u32 count;
    u32 elapsed;
    u32 lo, hi;
    u32 newLo;

    savedMask = __osDisableInt();

    /* Get current Count register */
    count = osGetCount();

    /* Calculate elapsed cycles since last update */
    elapsed = count - __osBaseCount;

    /* Read accumulated time */
    lo = __osTimeLo;
    hi = __osTimeHi;

    __osRestoreInt(savedMask);

    /* Add elapsed to low word, handle carry to high word */
    newLo = lo + elapsed;
    if (newLo < elapsed) {
        hi++;  /* Carry */
    }

    /* Return as 64-bit value (hi in upper 32 bits) */
    return ((u64)hi << 32) | newLo;
}
