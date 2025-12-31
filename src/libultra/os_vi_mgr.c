/**
 * @file os_vi_mgr.c
 * @brief Video Interface Manager
 *
 * Decompiled from asm/us/7630.s
 * Contains VI initialization and VI manager thread
 *
 * The VI manager handles vertical retrace interrupts and
 * coordinates framebuffer swapping with the display.
 */

#include "types.h"
#include "PR/os_thread.h"
#include "PR/os_message.h"

/* Event types */
#define OS_EVENT_VI      7
#define OS_EVENT_COUNTER 3
#define OS_EVENT_RETRACE 13
#define OS_EVENT_PRENMI  14

/* Forward declaration for VI mode structure */
struct OSViMode;

/* VI context structure (partial, for this file's use) */
typedef struct OSViContext {
    u16 state;              /* 0x00: State flags */
    u16 retraceCount;       /* 0x02: Retrace counter */
    void *buffer;           /* 0x04: Framebuffer address */
    struct OSViMode *mode;  /* 0x08: Current video mode */
    u32 features;           /* 0x0C: Feature flags */
    OSMesgQueue *mq;        /* 0x10: Message queue */
    OSMesg msg;             /* 0x14: Message value */
    /* ... more fields */
} OSViContext;

/* Event entry structure */
typedef struct {
    u16 type;               /* 0x00: Event type */
    u8 pad;                 /* 0x02: Padding */
    u8 unused;              /* 0x03: Unused */
    void *data;             /* 0x04: Event data */
} __OSEventMsg;

/* External data - VI manager globals */
extern s32 __osViInited;                /* VI initialized flag */
extern s32 __osViBlack;                 /* VI black screen flag */
extern OSThread __osViThread;           /* VI manager thread structure */
extern OSMesgQueue __osViMesgQueue;     /* VI event message queue */
extern OSMesg __osViMesgBuf[5];         /* VI event messages (5 slots) */
extern __OSEventMsg __osViEventMsg;     /* Retrace event message */
extern __OSEventMsg __osPreNMIEventMsg; /* Pre-NMI event message */
extern u16 __osViRetraceCount;          /* Retrace counter */
extern u32 __osTimeHi;                  /* Time high 32 bits */
extern u32 __osTimeLo;                  /* Time low 32 bits (extension) */
extern u32 __osBaseTime;                /* Last time sample */
extern u32 __osViRetraceTotal;          /* Total retrace count */

/* External functions */
extern void osSetEventMesg(s32 event, OSMesgQueue *mq, OSMesg msg);
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
extern void __osViInit(void);                   /* VI hardware init */
extern s32 osViGetCurrentMode(s32 arg);         /* Get current TV mode */
extern void osViSetMode(s32 a0, s32 a1);        /* Set video mode */
extern void __osViInterrupt(void);              /* Enable VI interrupt */
extern OSViContext *__osViGetCurrentContext(void); /* Get current VI context */
extern void __osViSwapContext(void);            /* Swap VI buffers */
extern u32 osGetCount(void);                    /* Read CP0 Count register */
extern void __osViHandlePreNMI(void);           /* Handle Pre-NMI */

/* Forward declaration */
static void vi_manager_thread(void *arg);

/**
 * Initialize Video Interface
 * (0x80006A30 - osViInit)
 *
 * Sets up the VI manager which handles vertical retrace
 * interrupts and coordinates with the display.
 *
 * @param mode Initial video mode
 */
void osViInit(s32 mode) {
    s32 saved;
    s32 prevMode;

    /* Only initialize once */
    if (__osViInited != 0) {
        return;
    }

    /* Initialize VI hardware */
    __osViInit();

    /* Clear black flag */
    __osViBlack = 0;

    /* Create message queue for VI events (5 slots) */
    osCreateMesgQueue(&__osViMesgQueue, __osViMesgBuf, 5);

    /* Set up retrace event message */
    __osViEventMsg.type = OS_EVENT_RETRACE;
    __osViEventMsg.pad = 0;
    __osViEventMsg.data = NULL;

    /* Set up pre-NMI event message */
    __osPreNMIEventMsg.type = OS_EVENT_PRENMI;
    __osPreNMIEventMsg.pad = 0;
    __osPreNMIEventMsg.data = NULL;

    /* Register for VI and Counter events */
    osSetEventMesg(OS_EVENT_VI, &__osViMesgQueue, (OSMesg)&__osViEventMsg);
    osSetEventMesg(OS_EVENT_COUNTER, &__osViMesgQueue, (OSMesg)&__osPreNMIEventMsg);

    /* Check and save current mode */
    prevMode = -1;
    if (osViGetCurrentMode(0) < mode) {
        osViSetMode(0, mode);
        prevMode = mode;
    }

    saved = __osDisableInt();

    /* Mark as initialized */
    __osViInited = 1;

    /* Create VI manager thread (id=0, priority from mode) */
    osCreateThread(&__osViThread, 0, vi_manager_thread, NULL,
                   &__osViMesgQueue, mode);

    /* Enable VI interrupt */
    __osViInterrupt();

    /* Start VI manager thread */
    osStartThread(&__osViThread);

    __osRestoreInt(saved);

    /* Restore previous mode if needed */
    if (prevMode != -1) {
        osViSetMode(0, prevMode);
    }
}

/**
 * VI Manager Thread
 * (0x80006BB0)
 *
 * Handles VI retrace and counter/timer events.
 * Runs as a high-priority thread to ensure timely
 * framebuffer swaps.
 *
 * @param arg Unused
 */
static void vi_manager_thread(void *arg) {
    __OSEventMsg *msg;
    OSViContext *viCtx;
    u16 retraceCount;
    u32 time;

    (void)arg;  /* Unused parameter */

    /* Get initial retrace count from VI context */
    viCtx = __osViGetCurrentContext();
    retraceCount = viCtx->retraceCount;
    if (retraceCount == 0) {
        retraceCount = 1;
    }
    __osViRetraceCount = retraceCount;

    for (;;) {
        /* Wait for event message */
        osRecvMesg(&__osViMesgQueue, (OSMesg *)&msg, OS_MESG_BLOCK);

        if (msg->type == OS_EVENT_RETRACE) {
            /* VI retrace interrupt */

            /* Swap buffers */
            __osViSwapContext();

            /* Decrement retrace counter */
            retraceCount = __osViRetraceCount - 1;
            __osViRetraceCount = retraceCount;

            if (retraceCount == 0) {
                /* Counter reached zero - notify waiting threads */
                viCtx = __osViGetCurrentContext();

                /* Send message if queue registered */
                if (viCtx->mq != NULL) {
                    osSendMesg(viCtx->mq, viCtx->msg, OS_MESG_NOBLOCK);
                }

                /* Reload counter */
                retraceCount = viCtx->retraceCount;
                __osViRetraceCount = retraceCount;
            }

            /* Update timing */
            __osViRetraceTotal++;
            time = osGetCount();

            /* Add elapsed time to accumulator */
            /* 64-bit addition: __osTimeHi:__osTimeLo += time - __osBaseTime */
            {
                u32 elapsed = time - __osBaseTime;
                u32 newLow = __osTimeLo + elapsed;
                u32 carry = (newLow < elapsed) ? 1 : 0;
                __osTimeHi += carry;
                __osTimeLo = newLow;
            }
            __osBaseTime = time;

        } else if (msg->type == OS_EVENT_PRENMI) {
            /* Pre-NMI warning */
            __osViHandlePreNMI();
        }
    }
}
