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
#include "PR/os_message.h"

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

#define OS_NUM_EVENTS         15

/* Event table entry structure */
typedef struct __OSEventState {
    OSMesgQueue *queue;     /* Message queue pointer */
    OSMesg msg;             /* Message value to send */
} __OSEventState;

/* External data */
extern __OSEventState __osEventStateTab[OS_NUM_EVENTS];  /* Event table */
extern s32 __osViIntrCount;         /* VI initialized flag */
extern s32 __osPreNMI;              /* Pre-NMI registered flag */

/* External functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);

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
void osSetEventMesg(s32 event, OSMesgQueue *mq, OSMesg msg) {
    s32 saved;
    __OSEventState *entry;

    saved = __osDisableInt();

    /* Get entry in event table */
    entry = &__osEventStateTab[event];

    /* Store message queue and message */
    entry->queue = mq;
    entry->msg = msg;

    /* Special handling for OS_EVENT_PRENMI (event 14) */
    if (event == OS_EVENT_PRENMI) {
        /* Check if VI is initialized and pre-NMI not already registered */
        if (__osViIntrCount != 0 && __osPreNMI == 0) {
            /* Send immediate notification if VI is running */
            osSendMesg(mq, msg, OS_MESG_NOBLOCK);
        }
        /* Mark pre-NMI as registered */
        __osPreNMI = 1;
    }

    __osRestoreInt(saved);
}
