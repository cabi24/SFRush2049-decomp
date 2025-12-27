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

/* External data */
extern OSMesgQueue *D_80036710[];   /* Timer message queue table (8 entries) */
extern s32 D_8002C36C;              /* Timer active flag */
extern s32 D_8002C350;              /* Timer initialized flag */
extern OSThread *D_8002C464;        /* System thread pointer */

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
    slot = &D_80036710[timer];

    /* Store message queue and message */
    slot[0] = mq;
    *(s32 *)&slot[1] = msg;

    /* Special handling for timer slot 14 (system timer) */
    if (timer == 0xE) {
        if (D_8002C36C != 0 && D_8002C350 == 0) {
            /* Timer is active but not initialized - send initial message */
            osSendMesg(mq, (OSMesg)msg, 0);
        }
        /* Mark timer as initialized */
        D_8002C350 = 1;
    }

    __osRestoreInt(saved);
}

/**
 * Set timer interrupt parameters
 * (func_80006ED0 - osSetTimerIntr)
 *
 * Configures timer interrupt for a thread.
 *
 * @param thread Target thread (via D_8002C464)
 * @param mq Message queue
 * @param value Interrupt value
 */
void osSetTimerIntr(OSThread *thread, OSMesgQueue *mq, s16 value) {
    s32 saved;
    u8 *sys;

    saved = __osDisableInt();

    sys = (u8 *)D_8002C464;

    /* Store thread pointer at offset 0 */
    *(void **)(sys + 0) = thread;

    sys = (u8 *)D_8002C464;
    /* Store message queue at offset 4 */
    *(void **)(sys + 4) = mq;

    sys = (u8 *)D_8002C464;
    /* Store interrupt value at offset 2 */
    *(s16 *)(sys + 2) = value;

    __osRestoreInt(saved);
}

/* External time tracking variables */
extern u32 D_80037C50;   /* Time high word */
extern u32 D_80037C54;   /* Time low word */
extern u32 D_80037C58;   /* Last count value */

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
 *   accumulated_time + (current_count - last_count)
 *
 * @return 64-bit system time (high word in return, low word in $v1)
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
    elapsed = count - D_80037C58;

    /* Read accumulated time */
    lo = D_80037C54;
    hi = D_80037C50;

    __osRestoreInt(savedMask);

    /* Add elapsed to low word, handle carry to high word */
    newLo = lo + elapsed;
    if (newLo < elapsed) {
        hi++;  /* Carry */
    }

    /* Return as 64-bit value (hi in upper 32 bits) */
    return ((u64)hi << 32) | newLo;
}
