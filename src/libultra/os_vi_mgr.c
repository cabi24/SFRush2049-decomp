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

/* Event types */
#define OS_EVENT_VI      7
#define OS_EVENT_COUNTER 3
#define OS_EVENT_RETRACE 13
#define OS_EVENT_PRENMI  14

/* VI manager state structure */
typedef struct {
    s32 initialized;        /* 0x00: Initialization flag */
    void *thread;           /* 0x04: VI manager thread */
    void *queue;            /* 0x08: Message queue for VI events */
    void *currQueue;        /* 0x0C: Current retrace queue */
    void *retraceQueue;     /* 0x10: Retrace message queue */
    s32 retraceMsg;         /* 0x14: Retrace message value */
    s32 unused;             /* 0x18: Unused */
} OSViMgr;

/* Event entry structure */
typedef struct {
    u16 type;               /* 0x00: Event type */
    u8 pad;                 /* 0x02: Padding */
    u8 unused;              /* 0x03: Unused */
    void *data;             /* 0x04: Event data */
} OSEventMsg;

/* External data */
extern OSViMgr D_8002C330;              /* VI manager state */
extern s32 D_8002C34C;                  /* VI black flag */
extern u8 D_800354F0[];                 /* VI manager thread structure */
extern u8 D_800366A0[24];               /* VI event message queue */
extern u8 D_800366B8[20];               /* VI event messages (5 slots) */
extern OSEventMsg D_800366D0;           /* Retrace event message */
extern OSEventMsg D_800366E8;           /* Pre-NMI event message */
extern u16 D_80036700;                  /* Retrace counter */
extern u32 D_80037C50;                  /* Time high */
extern u32 D_80037C54;                  /* Time low (extension) */
extern u32 D_80037C58;                  /* Last time */
extern u32 D_80037C5C;                  /* Retrace count */

/* External functions */
extern void osCreateMesgQueue(void *mq, void *msgBuf, s32 count);
extern void osSetEventMesg(s32 event, void *mq, s32 msg);
extern void osCreateThread(void *thread, s32 id, void (*entry)(void *),
                          void *arg, void *stack, s32 pri);
extern void osStartThread(void *thread);
extern s32 osRecvMesg(void *mq, void **msg, s32 flags);
extern s32 osSendMesg(void *mq, s32 msg, s32 flags);
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
extern void func_8000C090(void);        /* VI hardware init */
extern s32 func_8000C490(s32 arg);      /* Get current TV mode */
extern void func_80008380(s32 a0, s32 a1);  /* Set video mode */
extern void func_8000C540(void);        /* Enable VI interrupt */
extern void *func_8000C660(void);       /* Get current VI context */
extern void func_8000C670(void);        /* Swap VI buffers */
extern u32 osGetCount(void);            /* Read CP0 Count register */
extern void func_8000C11C(void);        /* Handle Pre-NMI */

/* Forward declaration */
static void vi_manager_thread(void *arg);

/**
 * Initialize Video Interface
 * (func_80006A30 - osViInit)
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
    if (D_8002C330.initialized != 0) {
        return;
    }

    /* Initialize VI hardware */
    func_8000C090();

    /* Clear black flag */
    D_8002C34C = 0;

    /* Create message queue for VI events (5 slots) */
    osCreateMesgQueue(D_800366A0, D_800366B8, 5);

    /* Set up retrace event message */
    D_800366D0.type = OS_EVENT_RETRACE;
    D_800366D0.pad = 0;
    D_800366D0.data = NULL;

    /* Set up pre-NMI event message */
    D_800366E8.type = OS_EVENT_PRENMI;
    D_800366E8.pad = 0;
    D_800366E8.data = NULL;

    /* Register for VI and Counter events */
    osSetEventMesg(OS_EVENT_VI, D_800366A0, (s32)&D_800366D0);
    osSetEventMesg(OS_EVENT_COUNTER, D_800366A0, (s32)&D_800366E8);

    /* Check and save current mode */
    prevMode = -1;
    if (func_8000C490(0) < mode) {
        func_80008380(0, mode);
        prevMode = mode;
    }

    saved = __osDisableInt();

    /* Initialize VI manager state */
    D_8002C330.initialized = 1;
    D_8002C330.thread = D_800354F0;
    D_8002C330.queue = D_800366A0;
    D_8002C330.currQueue = D_800366A0;
    D_8002C330.retraceQueue = NULL;
    D_8002C330.retraceMsg = 0;
    D_8002C330.unused = 0;

    /* Create VI manager thread (id=0, priority from mode) */
    osCreateThread(D_800354F0, 0, vi_manager_thread, &D_8002C330,
                   D_800366A0, mode);

    /* Enable VI interrupt */
    func_8000C540();

    /* Start VI manager thread */
    osStartThread(D_800354F0);

    __osRestoreInt(saved);

    /* Restore previous mode if needed */
    if (prevMode != -1) {
        func_80008380(0, prevMode);
    }
}

/**
 * VI Manager Thread
 * (func_80006BB0)
 *
 * Handles VI retrace and counter/timer events.
 * Runs as a high-priority thread to ensure timely
 * framebuffer swaps.
 *
 * @param arg Pointer to VI manager state
 */
static void vi_manager_thread(void *arg) {
    OSViMgr *mgr = (OSViMgr *)arg;
    OSEventMsg *msg;
    void *viCtx;
    u16 retraceCount;
    u32 time;

    /* Get initial retrace count from VI context */
    viCtx = func_8000C660();
    retraceCount = *(u16 *)((u8 *)viCtx + 2);
    if (retraceCount == 0) {
        retraceCount = 1;
    }
    D_80036700 = retraceCount;

    for (;;) {
        /* Wait for event message */
        osRecvMesg(mgr->currQueue, (void **)&msg, 1);

        if (msg->type == OS_EVENT_RETRACE) {
            /* VI retrace interrupt */

            /* Swap buffers */
            func_8000C670();

            /* Decrement retrace counter */
            retraceCount = D_80036700 - 1;
            D_80036700 = retraceCount;

            if (retraceCount == 0) {
                /* Counter reached zero - notify waiting threads */
                viCtx = func_8000C660();

                /* Send message if queue registered */
                if (*(void **)((u8 *)viCtx + 0x10) != NULL) {
                    osSendMesg(*(void **)((u8 *)viCtx + 0x10),
                              *(s32 *)((u8 *)viCtx + 0x14), 0);
                }

                /* Reload counter */
                retraceCount = *(u16 *)((u8 *)viCtx + 2);
                D_80036700 = retraceCount;
            }

            /* Update timing */
            D_80037C5C++;
            time = osGetCount();

            /* Add elapsed time to accumulator */
            /* 64-bit addition: D_80037C50:D_80037C54 += time - D_80037C58 */
            {
                u32 elapsed = time - D_80037C58;
                u32 newLow = D_80037C54 + elapsed;
                u32 carry = (newLow < elapsed) ? 1 : 0;
                D_80037C50 += carry;
                D_80037C54 = newLow;
            }
            D_80037C58 = time;

        } else if (msg->type == OS_EVENT_PRENMI) {
            /* Pre-NMI warning */
            func_8000C11C();
        }
    }
}
