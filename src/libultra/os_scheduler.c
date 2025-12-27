/**
 * @file os_scheduler.c
 * @brief N64 OS Scheduler implementation
 *
 * Decompiled from asm/us/1050.s (func_80000450 - func_80001578)
 * The scheduler manages RSP/RDP tasks and VI retrace timing.
 */

#include "types.h"
#include "PR/os_thread.h"
#include "PR/os_message.h"

/* OS Event types for osSetEventMesg */
#define OS_EVENT_SW1        0   /* Software interrupt 1 */
#define OS_EVENT_SW2        1   /* Software interrupt 2 */
#define OS_EVENT_CART       2   /* Cartridge interrupt */
#define OS_EVENT_COUNTER    3   /* Counter interrupt (timer) */
#define OS_EVENT_SP         4   /* SP (RSP) interrupt */
#define OS_EVENT_SI         5   /* SI (Serial) interrupt */
#define OS_EVENT_AI         6   /* AI (Audio) interrupt */
#define OS_EVENT_VI         7   /* VI (Video) interrupt */
#define OS_EVENT_PI         8   /* PI (Parallel) interrupt */
#define OS_EVENT_DP         9   /* DP (RDP) interrupt */
#define OS_EVENT_PRENMI     14  /* Pre-NMI warning */

/* Scheduler message IDs */
#define SC_MSG_SWAP         0x29A   /* Swap buffer complete */
#define SC_MSG_RETRACE      0x29B   /* VI retrace */
#define SC_MSG_RSP_DONE     0x29C   /* RSP done */
#define SC_MSG_RDP_DONE     0x29D   /* RDP done */
#define SC_MSG_PRENMI       0x29E   /* Pre-NMI */

/* Task types */
#define SC_TASK_GFX         1   /* Graphics task */
#define SC_TASK_AUDIO       2   /* Audio task */

/* Task flags */
#define SC_TASK_RDP_BUSY    0x01    /* RDP is processing */
#define SC_TASK_RSP_BUSY    0x02    /* RSP is processing */
#define SC_TASK_YIELD_REQ   0x10    /* Yield requested */
#define SC_TASK_YIELDED     0x20    /* Task yielded */
#define SC_TASK_SWAP_FB     0x40    /* Swap framebuffer on complete */

/* External function declarations */
extern void osSetEventMesg(s32 event, OSMesgQueue *mq, OSMesg msg);
extern void osViSetMode(void *mode);
extern void osViBlack(s32 black);
extern u32 osGetCount(void);
extern void osSpTaskLoad(void *task);
extern void osSpTaskStartGo(void *task);
extern s32 osSpTaskYielded(void *task);
extern void osViSwapBuffer(void *framebuffer);
extern s32 osDpSetNextBuffer(void *buf, s32 size);
extern void osSpTaskYield(void);

/* External data - scheduler globals */
extern s16 __osScTaskCount;             /* Number of pending tasks */
extern void *__osScCurAudioTask;        /* Current audio task */
extern s32 __osScPendingSwap;           /* Pending framebuffer address */
extern s32 __osScSwapCount;             /* Retrace count at last swap */
extern s32 __osScTimeEnabled;           /* Time accumulation enabled */
extern f32 __osScTicksPerSecond;        /* Ticks to seconds conversion */
extern f32 __osScSecondsPerTick;        /* Seconds per tick */
extern void *__osViModeTable;           /* Video mode table */
extern s64 __osScFrameTime;             /* Frame time accumulator */
extern s32 __osScFrameCount;            /* Current frame count */
extern s32 __osScRetraceTimeLo;         /* Retrace time (low 32 bits) */
extern s32 __osScRetraceTimeHi;         /* Retrace time (high 32 bits) */
extern u32 __osScAudioStartTime;        /* Audio task start time */
extern f32 __osScElapsedTime;           /* Elapsed time since start */
extern f32 __osScDeltaTime;             /* Delta time this frame */
extern s32 __osScDeltaFrames;           /* Delta frames this update */
extern s32 __osScDeadlineCount;         /* Deadline frame count */
extern s32 __osScStartCount;            /* Start frame count */
extern s32 __osScLastCount;             /* Last frame count */

/* Forward declarations */
static void __scMain(void *sc);
static void __scSchedule(void *sc);
static void __scHandleRetrace(void *sc);
static void __scHandleRSP(void *sc);
static void __scHandleRDP(void *sc);
static s32 __scTaskComplete(void *sc, void *task);
static void __scAppendList(void *sc, void *msg);
static void __scExec(void *sc, void *rsp, void *rdp);
static void __scYield(void *sc);
static s32 __scScheduleCore(void *sc, void **rsp, void **rdp, s32 flags);

/**
 * OSScClient - Scheduler client structure
 *
 * Clients register with the scheduler to receive notifications
 * about retrace events and task completion.
 */
typedef struct OSScClient_s {
    struct OSScClient_s *next;      /* 0x00: Next client in list */
    OSMesgQueue *msgQueue;          /* 0x04: Queue for notifications */
} OSScClient;

/**
 * OSSched - Scheduler structure (reconstructed from offsets)
 *
 * The scheduler manages RSP/RDP task queues and coordinates
 * with the VI retrace for frame timing.
 *
 * Layout:
 * 0x00:  state (s16)
 * 0x20:  priority (s16)
 * 0x40:  cmdQueue (OSMesgQueue)
 * 0x58:  cmdMsgs[8] (OSMesg)
 * 0x78:  retQueue (OSMesgQueue)
 * 0x90:  retMsgs[8] (OSMesg)
 * 0xB0:  thread (OSThread)
 * 0x260: clientList
 * 0x264: rspTaskHead
 * 0x268: rspTaskTail
 * 0x26C: rdpTaskHead
 * 0x270: rdpTaskTail
 * 0x274: curRSPTask
 * 0x278: curRDPTask
 * 0x27C: retraceCount
 * 0x280: audioListPending
 */

/* OSSched field offsets */
#define SCHED_STATE             0x00
#define SCHED_PRIORITY          0x20
#define SCHED_CMD_QUEUE         0x40
#define SCHED_CMD_MSGS          0x58
#define SCHED_RET_QUEUE         0x78
#define SCHED_RET_MSGS          0x90
#define SCHED_THREAD            0xB0
#define SCHED_CLIENT_LIST       0x260
#define SCHED_RSP_TASK_HEAD     0x264
#define SCHED_RSP_TASK_TAIL     0x268
#define SCHED_RDP_TASK_HEAD     0x26C
#define SCHED_RDP_TASK_TAIL     0x270
#define SCHED_CUR_RSP_TASK      0x274
#define SCHED_CUR_RDP_TASK      0x278
#define SCHED_RETRACE_COUNT     0x27C
#define SCHED_AUDIO_PENDING     0x280

/* OSScTask field offsets */
#define TASK_NEXT               0x00
#define TASK_STATE              0x04
#define TASK_FLAGS              0x08
#define TASK_FRAMEBUFFER        0x0C
#define TASK_TYPE               0x10
#define TASK_MSG_QUEUE          0x50
#define TASK_MSG                0x54

/**
 * Create and initialize the scheduler
 * (func_80000450 - osCreateScheduler)
 *
 * @param sc Scheduler structure to initialize
 * @param stack Stack for scheduler thread
 * @param stackSize Size of stack
 * @param viMode VI mode index
 * @param numFields Number of fields (1 or 2)
 */
void osCreateScheduler(void *sc, void *stack, s32 stackSize, u8 viMode, u8 numFields) {
    u8 *scheduler = (u8 *)sc;

    /* Reset task count */
    __osScTaskCount = 0;

    /* Clear task pointers */
    *(void **)(scheduler + SCHED_CUR_RSP_TASK) = NULL;
    *(void **)(scheduler + SCHED_CUR_RDP_TASK) = NULL;
    *(void **)(scheduler + SCHED_CLIENT_LIST) = NULL;
    *(s32 *)(scheduler + SCHED_RETRACE_COUNT) = 0;
    *(void **)(scheduler + SCHED_RSP_TASK_HEAD) = NULL;
    *(void **)(scheduler + SCHED_RSP_TASK_TAIL) = NULL;
    *(void **)(scheduler + SCHED_RDP_TASK_HEAD) = NULL;
    *(void **)(scheduler + SCHED_RDP_TASK_TAIL) = NULL;

    /* Set state and priority */
    *(s16 *)(scheduler + SCHED_STATE) = 1;
    *(s16 *)(scheduler + SCHED_PRIORITY) = 4;

    /* Create command message queue (8 messages) */
    osCreateMesgQueue((OSMesgQueue *)(scheduler + SCHED_CMD_QUEUE),
                      (OSMesg *)(scheduler + SCHED_CMD_MSGS), 8);

    /* Create retrace message queue (8 messages) */
    osCreateMesgQueue((OSMesgQueue *)(scheduler + SCHED_RET_QUEUE),
                      (OSMesg *)(scheduler + SCHED_RET_MSGS), 8);

    /* Set event mask */
    osSetEventMesg(0xFE, NULL, NULL);

    /* Set VI mode */
    osViSetMode((u8 *)&__osViModeTable + (viMode * 0x50));

    /* Enable/disable black screen */
    osViBlack(1);

    /* Register for SP (RSP) events */
    osSetEventMesg(OS_EVENT_SP, (OSMesgQueue *)(scheduler + SCHED_CMD_QUEUE),
                   (OSMesg)SC_MSG_RETRACE);

    /* Register for DP (RDP) events */
    osSetEventMesg(OS_EVENT_DP, (OSMesgQueue *)(scheduler + SCHED_CMD_QUEUE),
                   (OSMesg)SC_MSG_RSP_DONE);

    /* Register for Pre-NMI events */
    osSetEventMesg(OS_EVENT_PRENMI, (OSMesgQueue *)(scheduler + SCHED_CMD_QUEUE),
                   (OSMesg)SC_MSG_RDP_DONE);

    /* Register for software interrupt 1 */
    osSetEventMesg(OS_EVENT_SW1, (OSMesgQueue *)(scheduler + SCHED_CMD_QUEUE),
                   (OSMesg)SC_MSG_PRENMI);

    /* Set up VI swap callback with numFields */
    /* osViSetSwapBuffer(scheduler + SCHED_CMD_QUEUE, SC_MSG_SWAP, numFields); */

    /* Create scheduler thread (priority 4, ID 4) */
    osCreateThread((OSThread *)(scheduler + SCHED_THREAD), 4, __scMain, sc,
                   stack, stackSize);

    /* Start scheduler thread */
    osStartThread((OSThread *)(scheduler + SCHED_THREAD));
}

/**
 * Add a client to receive scheduler notifications
 * (func_800005D4 - osScAddClient)
 *
 * @param sc Scheduler
 * @param client Client to add
 * @param msgQueue Message queue for notifications
 */
void osScAddClient(void *sc, OSScClient *client, OSMesgQueue *msgQueue) {
    u8 *scheduler = (u8 *)sc;
    s32 saved;

    saved = __osDisableInt();

    client->msgQueue = msgQueue;
    client->next = *(OSScClient **)(scheduler + SCHED_CLIENT_LIST);
    *(OSScClient **)(scheduler + SCHED_CLIENT_LIST) = client;

    __osRestoreInt(saved);
}

/**
 * Main scheduler loop
 * (func_8000063C - __scMain)
 */
static void __scMain(void *arg) {
    u8 *sc = (u8 *)arg;
    OSMesg msg;

    while (1) {
        /* Wait for message on command queue */
        if (osRecvMesg((OSMesgQueue *)(sc + SCHED_CMD_QUEUE), &msg,
                       OS_MESG_BLOCK) != -1) {
            s32 msgId = (s32)(uintptr_t)msg;

            /* Handle different message types */
            switch (msgId - SC_MSG_SWAP) {
                case 0:  /* SC_MSG_SWAP - Swap buffer complete */
                    /* osViSwapBuffer */
                    break;

                case 1:  /* SC_MSG_RETRACE - VI Retrace */
                    *(s64 *)&__osScRetraceTimeLo = osGetCount();
                    __scHandleRetrace(sc);
                    __scSchedule(sc);
                    break;

                case 2:  /* SC_MSG_RSP_DONE - RSP done */
                    __scHandleRSP(sc);
                    break;

                case 3:  /* SC_MSG_RDP_DONE - RDP done */
                    __osScFrameTime = osGetCount() - *(s64 *)&__osScRetraceTimeLo;
                    __scHandleRDP(sc);
                    break;

                case 4:  /* SC_MSG_PRENMI - Pre-NMI */
                    __scSchedule(sc);
                    break;

                default:
                    /* Unknown message - notify clients */
                    {
                        OSScClient *client = *(OSScClient **)(sc + SCHED_CLIENT_LIST);
                        while (client != NULL) {
                            osSendMesg(client->msgQueue,
                                      (OSMesg)(sc + SCHED_PRIORITY), OS_MESG_BLOCK);
                            client = client->next;
                        }
                    }
                    break;
            }
        }
    }
}

/**
 * Schedule pending tasks
 * (func_80000780 - __scSchedule)
 */
static void __scSchedule(void *arg) {
    u8 *sc = (u8 *)arg;
    OSMesg msg;
    void *rspTask = NULL;
    void *rdpTask = NULL;

    /* Process any pending return messages */
    while (osRecvMesg((OSMesgQueue *)(sc + SCHED_RET_QUEUE), &msg,
                      OS_MESG_NOBLOCK) != -1) {
        __scAppendList(sc, msg);
    }

    /* Check if we can yield current audio task */
    if (*(void **)(sc + SCHED_AUDIO_PENDING) != NULL &&
        *(void **)(sc + SCHED_RSP_TASK_HEAD) != NULL &&
        *(void **)(sc + SCHED_CUR_RSP_TASK) != NULL) {
        __scYield(sc);
    } else {
        /* Calculate scheduling flags */
        s32 flags = ((*(void **)(sc + SCHED_CUR_RSP_TASK) == NULL) << 1) |
                    (*(void **)(sc + SCHED_CUR_RDP_TASK) == NULL);

        if (__scScheduleCore(sc, &rspTask, &rdpTask, flags) != flags) {
            __scExec(sc, rspTask, rdpTask);
        }
    }
}

/**
 * Handle VI retrace interrupt
 * (func_80000880 - __scHandleRetrace)
 */
static void __scHandleRetrace(void *arg) {
    u8 *sc = (u8 *)arg;
    OSScClient *client;

    /* Increment retrace counter */
    (*(s32 *)(sc + SCHED_RETRACE_COUNT))++;

    /* Check for pending swap */
    if (__osScPendingSwap != 0 &&
        (u32)(*(s32 *)(sc + SCHED_RETRACE_COUNT) - __osScSwapCount) >= 2) {
        osViSwapBuffer((void *)(uintptr_t)__osScPendingSwap);
        /* Clear pending */
        __osScSwapCount = *(s32 *)(sc + SCHED_RETRACE_COUNT);
        __osScPendingSwap = 0;
    }

    /* Notify all clients of retrace */
    client = *(OSScClient **)(sc + SCHED_CLIENT_LIST);
    while (client != NULL) {
        osSendMesg(client->msgQueue, (OSMesg)sc, OS_MESG_NOBLOCK);
        client = client->next;
    }
}

/**
 * Handle RSP task completion
 * (func_80000950 - __scHandleRSP)
 *
 * Called when the RSP finishes processing a task.
 * Handles yield checks and reschedules pending tasks.
 */
static void __scHandleRSP(void *arg) {
    u8 *sc = (u8 *)arg;
    void *task;
    void *rspTask = NULL;
    void *rdpTask = NULL;
    s32 flags;

    if (*(void **)(sc + SCHED_CUR_RSP_TASK) == NULL) {
        return;
    }

    task = *(void **)(sc + SCHED_CUR_RSP_TASK);
    *(void **)(sc + SCHED_CUR_RSP_TASK) = NULL;

    /* Check if task yielded and has pending work */
    if ((*(s32 *)((u8 *)task + TASK_STATE) & SC_TASK_YIELD_REQ) &&
        osSpTaskYielded((u8 *)task + TASK_TYPE) != 0) {
        /* Mark as yielded */
        *(s32 *)((u8 *)task + TASK_STATE) |= SC_TASK_YIELDED;

        /* If graphics task, add back to RDP queue */
        if ((*(s32 *)((u8 *)task + TASK_FLAGS) & 7) == 3) {
            *(void **)task = *(void **)(sc + SCHED_RSP_TASK_TAIL);
            *(void **)(sc + SCHED_RSP_TASK_TAIL) = task;
            if (*(void **)(sc + SCHED_RDP_TASK_TAIL) == NULL) {
                *(void **)(sc + SCHED_RDP_TASK_TAIL) = task;
            }
        }
    } else {
        /* Clear RSP busy flag */
        *(s32 *)((u8 *)task + TASK_STATE) &= ~SC_TASK_RSP_BUSY;
        __scTaskComplete(sc, task);
    }

    /* Reschedule */
    flags = ((*(void **)(sc + SCHED_CUR_RSP_TASK) == NULL) << 1) |
            (*(void **)(sc + SCHED_CUR_RDP_TASK) == NULL);
    if (__scScheduleCore(sc, &rspTask, &rdpTask, flags) != flags) {
        __scExec(sc, rspTask, rdpTask);
    }
}

/**
 * Handle RDP task completion
 * (func_80000ACC - __scHandleRDP)
 *
 * Called when the RDP finishes processing a task.
 */
static void __scHandleRDP(void *arg) {
    u8 *sc = (u8 *)arg;
    void *task;
    void *rspTask = NULL;
    void *rdpTask = NULL;
    s32 flags;

    if (*(void **)(sc + SCHED_CUR_RDP_TASK) == NULL) {
        return;
    }

    /* Verify task type is graphics */
    if (*(s32 *)(*(u8 **)(sc + SCHED_CUR_RDP_TASK) + TASK_TYPE) != SC_TASK_GFX) {
        return;
    }

    task = *(void **)(sc + SCHED_CUR_RDP_TASK);
    *(void **)(sc + SCHED_CUR_RDP_TASK) = NULL;

    /* Clear RDP busy flag */
    *(s32 *)((u8 *)task + TASK_STATE) &= ~SC_TASK_RDP_BUSY;
    __scTaskComplete(sc, task);

    /* Reschedule */
    flags = ((*(void **)(sc + SCHED_CUR_RSP_TASK) == NULL) << 1) |
            (*(void **)(sc + SCHED_CUR_RDP_TASK) == NULL);
    if (__scScheduleCore(sc, &rspTask, &rdpTask, flags) != flags) {
        __scExec(sc, rspTask, rdpTask);
    }
}

/* External VI functions */
extern s32 osViGetCurrentFramebuffer(void);
extern void *osViGetFramebuffer(void);
extern void __osScViSync(void);  /* VI sync/update */

/**
 * Check if buffer swap is ready
 * (func_80000BA4 - __scCheckSwap)
 */
static s32 __scCheckSwap(void *sc, s32 flag) {
    u8 *scheduler = (u8 *)sc;
    s32 curFb, nextFb;

    if (flag != 0) {
        curFb = osViGetCurrentFramebuffer();
        nextFb = (s32)(uintptr_t)osViGetFramebuffer();

        if (nextFb != curFb) {
            return 0;
        }

        /* Check if swap is pending and not yet ready */
        if (__osScPendingSwap != 0 &&
            (u32)(*(s32 *)(scheduler + SCHED_RETRACE_COUNT) - __osScSwapCount) < 2) {
            return 0;
        }

        return flag;
    }
    return 0;
}

/**
 * Complete a task and notify client
 * (func_80000C54 - __scTaskComplete)
 */
static s32 __scTaskComplete(void *arg, void *task) {
    u8 *sc = (u8 *)arg;
    u8 *t = (u8 *)task;
    s32 flags;
    s32 result;

    /* Check if both RSP and RDP are done (flags in low 2 bits) */
    if (!(*(s32 *)(t + TASK_STATE) & (SC_TASK_RDP_BUSY | SC_TASK_RSP_BUSY))) {
        /* Send completion message to client */
        result = osSendMesg(*(OSMesgQueue **)(t + TASK_MSG_QUEUE),
                           *(OSMesg *)(t + TASK_MSG), OS_MESG_BLOCK);

        /* Handle graphics task completion */
        if (*(s32 *)(t + TASK_TYPE) == SC_TASK_GFX) {
            __osScTaskCount--;

            flags = *(s32 *)(t + TASK_FLAGS);
            /* Check for swap buffer flag and completed */
            if ((flags & SC_TASK_SWAP_FB) && (flags & SC_TASK_YIELDED)) {
                if ((u32)(*(s32 *)(sc + SCHED_RETRACE_COUNT) - __osScSwapCount) >= 2) {
                    __osScSwapCount = *(s32 *)(sc + SCHED_RETRACE_COUNT);
                    osViSwapBuffer(*(void **)(t + TASK_FRAMEBUFFER));
                    __osScViSync();
                } else {
                    __osScPendingSwap = *(s32 *)(t + TASK_FRAMEBUFFER);
                }
            }
        }
        return 1;
    }
    return 0;
}

/**
 * Append task to appropriate queue
 * (func_80000D6C - __scAppendList)
 */
static void __scAppendList(void *arg, void *msg) {
    u8 *sc = (u8 *)arg;
    u8 *task = (u8 *)msg;
    s32 taskType;

    taskType = *(s32 *)(task + TASK_TYPE);

    /* Task type 2 = audio, type 1 = graphics */
    if (taskType == SC_TASK_AUDIO) {
        /* Audio task - add to RSP queue */
        if (*(void **)(sc + SCHED_RDP_TASK_HEAD) != NULL) {
            *(void **)*(void **)(sc + SCHED_RDP_TASK_HEAD) = task;
        } else {
            *(void **)(sc + SCHED_RSP_TASK_HEAD) = task;
        }
        *(void **)(sc + SCHED_RDP_TASK_HEAD) = task;

        /* Mark audio pending */
        *(s32 *)(sc + SCHED_AUDIO_PENDING) = 1;
    } else {
        /* Graphics task - add to RDP queue */
        if (*(void **)(sc + SCHED_RDP_TASK_TAIL) != NULL) {
            *(void **)*(void **)(sc + SCHED_RDP_TASK_TAIL) = task;
        } else {
            *(void **)(sc + SCHED_RSP_TASK_TAIL) = task;
        }
        *(void **)(sc + SCHED_RDP_TASK_TAIL) = task;
    }

    /* Clear next pointer and set busy flags from task flags */
    *(void **)task = NULL;
    *(s32 *)(task + TASK_STATE) = *(s32 *)(task + TASK_FLAGS) &
                                  (SC_TASK_RDP_BUSY | SC_TASK_RSP_BUSY);
}

/* External SP/DP functions */
extern void osSpClearStatus(void);

/* OSScTask RDP buffer offset */
#define TASK_RDP_BUFFER     0x38

/**
 * Execute RSP/RDP tasks
 * (func_80000E20 - __scExec)
 */
static void __scExec(void *arg, void *rspTask, void *rdpTask) {
    u8 *sc = (u8 *)arg;
    u8 *rsp = (u8 *)rspTask;
    u8 *rdp = (u8 *)rdpTask;

    if (*(void **)(sc + SCHED_CUR_RSP_TASK) != NULL) {
        return;
    }

    if (rspTask != NULL) {
        /* Audio task */
        if (*(s32 *)(rsp + TASK_TYPE) == SC_TASK_AUDIO) {
            __osScAudioStartTime = osGetCount();
            __osScCurAudioTask = rspTask;
        } else if (!(*(s32 *)(rsp + TASK_STATE) & SC_TASK_YIELDED)) {
            /* Not yielded - record time */
            __osScFrameTime = __osScRetraceTimeLo;
        }

        /* Clear SP status and load task */
        osSpClearStatus();
        *(s32 *)(rsp + TASK_STATE) &= ~(SC_TASK_YIELD_REQ | SC_TASK_YIELDED);
        osSpTaskLoad(rsp + TASK_TYPE);
        osSpTaskStartGo(rsp + TASK_TYPE);

        *(void **)(sc + SCHED_CUR_RSP_TASK) = rspTask;
        if (rspTask == rdpTask) {
            *(void **)(sc + SCHED_CUR_RDP_TASK) = rdpTask;
        }
    }

    /* Set up RDP if different task */
    if (rdpTask != NULL && rdpTask != rspTask) {
        osDpSetNextBuffer(*(void **)(rdp + TASK_RDP_BUFFER), 0);
        *(void **)(sc + SCHED_CUR_RDP_TASK) = rdpTask;
    }
}

/**
 * Request current RSP task to yield
 * (func_80000F88 - __scYield)
 */
static void __scYield(void *arg) {
    u8 *sc = (u8 *)arg;
    u8 *task;

    /* Only yield graphics tasks */
    if (*(s32 *)(*(u8 **)(sc + SCHED_CUR_RSP_TASK) + TASK_TYPE) == SC_TASK_GFX) {
        task = *(u8 **)(sc + SCHED_CUR_RSP_TASK);
        /* Set yield request flag */
        *(s32 *)(task + TASK_STATE) |= SC_TASK_YIELD_REQ;
        osSpTaskYield();
    }
}

/**
 * Core scheduling algorithm
 * (func_80000FE8 - __scScheduleCore)
 *
 * Determines which tasks to run next based on queue state.
 *
 * @param sc Scheduler
 * @param rsp Output: RSP task to run
 * @param rdp Output: RDP task to run
 * @param flags Input flags (bit 1 = RSP free, bit 0 = RDP free)
 * @return Updated flags
 */
static s32 __scScheduleCore(void *sc, void **rsp, void **rdp, s32 flags) {
    u8 *scheduler = (u8 *)sc;
    void *rspHead, *rdpHead;

    rspHead = *(void **)(scheduler + SCHED_RSP_TASK_HEAD);
    rdpHead = *(void **)(scheduler + SCHED_RSP_TASK_TAIL);

    /* Check RSP queue (bit 1 = RSP available) */
    if ((flags & SC_TASK_RSP_BUSY) && rspHead != NULL) {
        *rsp = rspHead;
        *(void **)(scheduler + SCHED_RSP_TASK_HEAD) = *(void **)rspHead;
        if (*(void **)(scheduler + SCHED_RSP_TASK_HEAD) == NULL) {
            *(void **)(scheduler + SCHED_RDP_TASK_HEAD) = NULL;
        }
        flags &= ~SC_TASK_RSP_BUSY;

        /* Audio task clears pending flag */
        if (*(s32 *)((u8 *)rspHead + TASK_TYPE) == SC_TASK_AUDIO) {
            *(s32 *)(scheduler + SCHED_AUDIO_PENDING) = 0;
        }
    }

    /* Check RDP queue (bit 0 = RDP available) */
    if ((flags & SC_TASK_RDP_BUSY) && rdpHead != NULL) {
        *rdp = rdpHead;
        *(void **)(scheduler + SCHED_RSP_TASK_TAIL) = *(void **)rdpHead;
        if (*(void **)(scheduler + SCHED_RSP_TASK_TAIL) == NULL) {
            *(void **)(scheduler + SCHED_RDP_TASK_TAIL) = NULL;
        }
        flags &= ~SC_TASK_RDP_BUSY;
    }

    return flags;
}

/**
 * Reset time tracking
 * (func_80001350)
 */
void osScResetTime(void) {
    s32 saved;
    f32 temp;

    saved = __osDisableInt();

    __osScLastCount = __osScFrameCount;
    temp = 2.0f * __osScSecondsPerTick;
    __osScDeltaTime = temp;
    __osScElapsedTime = temp;

    __osRestoreInt(saved);
}

/**
 * Enable time accumulation
 * (func_800013C0)
 */
void osScEnableTime(void) {
    __osScTimeEnabled = 1;
}

/**
 * Disable time accumulation
 * (func_800013DC)
 */
void osScDisableTime(void) {
    __osScTimeEnabled = 0;
}

/**
 * Update frame timing
 * (func_800013F4)
 */
void osScUpdateTime(void) {
    __osScDeltaFrames = __osScFrameCount - __osScLastCount;
    __osScLastCount = __osScFrameCount;

    if (__osScTimeEnabled != 0) {
        __osScStartCount += __osScDeltaFrames;
        __osScDeadlineCount += __osScDeltaFrames;
    } else {
        __osScElapsedTime = (f32)(__osScFrameCount - __osScStartCount) *
                            __osScSecondsPerTick;
    }

    /* Clamp delta to max 5 frames */
    if (__osScDeltaFrames >= 6) {
        __osScDeltaFrames = 5;
    }

    __osScDeltaTime = (f32)__osScDeltaFrames * __osScSecondsPerTick;
}

/**
 * Set deadline timer
 * (func_800014F0)
 */
void osScSetDeadline(f32 time) {
    __osScStartCount = __osScFrameCount;
    __osScDeadlineCount = (s32)(time * __osScTicksPerSecond) + __osScFrameCount;
}

/**
 * Add time to deadline
 * (func_8000153C)
 */
void osScAddDeadline(f32 time) {
    __osScDeadlineCount += (s32)(time * __osScTicksPerSecond);
}

/**
 * Get time remaining until deadline
 * (func_80001578)
 */
f32 osScGetTimeRemaining(void) {
    return (f32)(__osScDeadlineCount - __osScFrameCount) * __osScSecondsPerTick;
}

/**
 * Check if deadline has passed
 * (func_800015BC)
 */
s32 osScDeadlinePassed(void) {
    return (__osScDeadlineCount - __osScFrameCount) < 1;
}

/**
 * Stub function
 * (func_800015DC)
 */
void osScStub(void) {
    /* Empty */
}
