/**
 * @file os_event.c
 * @brief Event and interrupt handling
 *
 * Decompiled from asm/us/7A10.s
 * Contains functions for setting up event message handlers
 *
 * The N64 OS uses an event table that maps hardware interrupts
 * to message queues. When an interrupt occurs, the corresponding
 * message is sent to the registered queue.
 *
 * Event types (from libultra):
 *   0  = OS_EVENT_SW1        (Software interrupt 1)
 *   1  = OS_EVENT_SW2        (Software interrupt 2)
 *   2  = OS_EVENT_CART       (Cartridge interrupt)
 *   3  = OS_EVENT_COUNTER    (CPU counter)
 *   4  = OS_EVENT_SP         (RSP interrupt)
 *   5  = OS_EVENT_SI         (Serial interface)
 *   6  = OS_EVENT_AI         (Audio interface)
 *   7  = OS_EVENT_VI         (Video interface)
 *   8  = OS_EVENT_PI         (Peripheral interface)
 *   9  = OS_EVENT_DP         (RDP interrupt)
 *   10 = OS_EVENT_CPU_BREAK  (CPU break)
 *   11 = OS_EVENT_SP_BREAK   (RSP break)
 *   12 = OS_EVENT_FAULT      (CPU fault)
 *   13 = OS_EVENT_THREADSTATUS (Thread status change)
 *   14 = OS_EVENT_PRENMI     (Pre-NMI warning)
 */

#include "types.h"

/* Event IDs */
#define OS_EVENT_SW1          0
#define OS_EVENT_SW2          1
#define OS_EVENT_CART         2
#define OS_EVENT_COUNTER      3
#define OS_EVENT_SP           4
#define OS_EVENT_SI           5
#define OS_EVENT_AI           6
#define OS_EVENT_VI           7
#define OS_EVENT_PI           8
#define OS_EVENT_DP           9
#define OS_EVENT_CPU_BREAK    10
#define OS_EVENT_SP_BREAK     11
#define OS_EVENT_FAULT        12
#define OS_EVENT_THREADSTATUS 13
#define OS_EVENT_PRENMI       14

/* Event table entry structure */
typedef struct {
    void *mq;       /* Message queue pointer */
    s32 msg;        /* Message value to send */
} OSEventEntry;

/* External data */
extern OSEventEntry D_80036710[15];  /* Event table (15 events * 8 bytes) */
extern s32 D_8002C36C;               /* VI initialized flag */
extern s32 D_8002C350;               /* Pre-NMI registered flag */

/* External functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
extern s32 osSendMesg(void *mq, s32 msg, s32 flags);

/**
 * Set event message
 * (func_80006E10 - osSetEventMesg)
 *
 * Registers a message queue to receive messages when
 * a specific hardware event occurs.
 *
 * @param event Event type (OS_EVENT_*)
 * @param mq Message queue to receive event notifications
 * @param msg Message value to send when event occurs
 */
void osSetEventMesg(s32 event, void *mq, s32 msg) {
    s32 saved;
    OSEventEntry *entry;

    saved = __osDisableInt();

    /* Get entry in event table */
    entry = &D_80036710[event];

    /* Store message queue and message */
    entry->mq = mq;
    entry->msg = msg;

    /* Special handling for OS_EVENT_PRENMI (event 14) */
    if (event == OS_EVENT_PRENMI) {
        /* Check if VI is initialized and pre-NMI not already registered */
        if (D_8002C36C != 0 && D_8002C350 == 0) {
            /* Send immediate notification if VI is running */
            osSendMesg(mq, msg, 0);
        }
        /* Mark pre-NMI as registered */
        D_8002C350 = 1;
    }

    __osRestoreInt(saved);
}
