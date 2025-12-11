/**
 * @file os_scheduler.c
 * @brief N64 OS Scheduler implementation
 *
 * Decompiled from asm/us/1050.s (func_80000450 - func_80001578)
 * The scheduler manages RSP/RDP tasks and VI retrace timing.
 */

#include "types.h"

/* External function declarations */
extern void osCreateMesgQueue(void *mq, void *msg, s32 count);
extern void osSetEventMesg(s32 event, void *mq, s32 msg);
extern void osViSetMode(void *mode);
extern void osViBlack(s32 black);
extern void osCreateThread(void *thread, s32 id, void (*entry)(void *),
                           void *arg, void *stack, s32 priority);
extern void osStartThread(void *thread);
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
extern s32 osRecvMesg(void *mq, void **msg, s32 flags);
extern s32 osSendMesg(void *mq, void *msg, s32 flags);
extern u32 osGetCount(void);
extern void osSpTaskLoad(void *task);
extern void osSpTaskStartGo(void *task);
extern s32 osSpTaskYielded(void *task);
extern void osViSwapBuffer(void *framebuffer);
extern void osDpSetNextBuffer(void *buf, s32 size);

/* External data */
extern s16 D_8002EB70;
extern void *D_8002AFA0;
extern s32 D_8002AFA4;
extern s32 D_8002AFA8;
extern s32 D_8002AFB0;
extern f32 D_8002AFB4;
extern f32 D_8002AFB8;
extern void *D_8002B1B0;
extern s64 D_8002E8E0;
extern s32 D_8002EB64;
extern s32 D_8002EB80;
extern s32 D_8002EB84;
extern u32 D_8002EB78;
extern f32 D_8002EB90;
extern f32 D_8002EB94;
extern s32 D_8002EB98;
extern s32 D_8002EB9C;
extern s32 D_8002EBA0;
extern s32 D_8002EBA4;

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
 * OSScClient structure (reconstructed from offsets):
 * 0x00: next - pointer to next client
 * 0x04: msgQueue - message queue to notify
 */

/**
 * OSSched structure (reconstructed from m2c offsets):
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

typedef struct OSScClient_s {
    struct OSScClient_s *next;
    void *msgQueue;
} OSScClient;

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
    D_8002EB70 = 0;

    /* Clear task pointers */
    *(void **)(scheduler + 0x274) = NULL;  /* curRSPTask */
    *(void **)(scheduler + 0x278) = NULL;  /* curRDPTask */
    *(void **)(scheduler + 0x260) = NULL;  /* clientList */
    *(s32 *)(scheduler + 0x27C) = 0;       /* retraceCount */
    *(void **)(scheduler + 0x264) = NULL;  /* rspTaskHead */
    *(void **)(scheduler + 0x268) = NULL;  /* rspTaskTail */
    *(void **)(scheduler + 0x26C) = NULL;  /* rdpTaskHead */
    *(void **)(scheduler + 0x270) = NULL;  /* rdpTaskTail */

    /* Set state and priority */
    *(s16 *)(scheduler + 0x00) = 1;
    *(s16 *)(scheduler + 0x20) = 4;

    /* Create command message queue (8 messages) */
    osCreateMesgQueue(scheduler + 0x40, scheduler + 0x58, 8);

    /* Create retrace message queue (8 messages) */
    osCreateMesgQueue(scheduler + 0x78, scheduler + 0x90, 8);

    /* Set event mask */
    osSetEventMesg(0xFE, NULL, 0);

    /* Set VI mode */
    osViSetMode((u8 *)&D_8002B1B0 + (viMode * 0x50));

    /* Enable/disable black screen */
    osViBlack(1);

    /* Register for VI retrace events */
    osSetEventMesg(4, scheduler + 0x40, 0x29B);

    /* Register for SP events */
    osSetEventMesg(9, scheduler + 0x40, 0x29C);

    /* Register for DP events */
    osSetEventMesg(14, scheduler + 0x40, 0x29D);

    /* Register for Pre-NMI events */
    osSetEventMesg(0, scheduler + 0x40, 0x29E);

    /* Set up VI swap callback with numFields */
    /* osViSetSwapBuffer(scheduler + 0x40, 0x29A, numFields); */

    /* Create scheduler thread (priority 4, ID 4) */
    osCreateThread(scheduler + 0xB0, 4, __scMain, sc, stack, stackSize);

    /* Start scheduler thread */
    osStartThread(scheduler + 0xB0);
}

/**
 * Add a client to receive scheduler notifications
 * (func_800005D4 - osScAddClient)
 *
 * @param sc Scheduler
 * @param client Client to add
 * @param msgQueue Message queue for notifications
 */
void osScAddClient(void *sc, OSScClient *client, void *msgQueue) {
    u8 *scheduler = (u8 *)sc;
    s32 saved;

    saved = __osDisableInt();

    client->msgQueue = msgQueue;
    client->next = *(OSScClient **)(scheduler + 0x260);
    *(OSScClient **)(scheduler + 0x260) = client;

    __osRestoreInt(saved);
}

/**
 * Main scheduler loop
 * (func_8000063C - __scMain)
 */
static void __scMain(void *arg) {
    u8 *sc = (u8 *)arg;
    void *msg;

    while (1) {
        /* Wait for message on command queue */
        if (osRecvMesg(sc + 0x40, &msg, 1) != -1) {
            s32 msgId = (s32)(uintptr_t)msg;

            /* Handle different message types */
            switch (msgId - 0x29A) {
                case 0:  /* 0x29A - Swap buffer complete */
                    /* osViSwapBuffer */
                    break;

                case 1:  /* 0x29B - VI Retrace */
                    *(s64 *)&D_8002EB80 = osGetCount();
                    __scHandleRetrace(sc);
                    __scSchedule(sc);
                    break;

                case 2:  /* 0x29C - RSP done */
                    __scHandleRSP(sc);
                    break;

                case 3:  /* 0x29D - RDP done */
                    D_8002E8E0 = osGetCount() - *(s64 *)&D_8002EB80;
                    __scHandleRDP(sc);
                    break;

                case 4:  /* 0x29E - Pre-NMI */
                    __scSchedule(sc);
                    break;

                default:
                    /* Unknown message - notify clients */
                    {
                        OSScClient *client = *(OSScClient **)(sc + 0x260);
                        while (client != NULL) {
                            osSendMesg(client->msgQueue, (void *)(uintptr_t)(sc + 0x20), 1);
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
    void *msg;
    void *rspTask = NULL;
    void *rdpTask = NULL;

    /* Process any pending return messages */
    while (osRecvMesg(sc + 0x78, &msg, 0) != -1) {
        __scAppendList(sc, msg);
    }

    /* Check if we can yield current audio task */
    if (*(void **)(sc + 0x280) != NULL &&
        *(void **)(sc + 0x264) != NULL &&
        *(void **)(sc + 0x274) != NULL) {
        __scYield(sc);
    } else {
        /* Calculate scheduling flags */
        s32 flags = ((*(void **)(sc + 0x274) == NULL) << 1) |
                    (*(void **)(sc + 0x278) == NULL);

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
    (*(s32 *)(sc + 0x27C))++;

    /* Check for pending swap */
    if (D_8002AFA4 != 0 && (u32)(*(s32 *)(sc + 0x27C) - D_8002AFA8) >= 2) {
        osViSwapBuffer((void *)(uintptr_t)D_8002AFA4);
        /* Clear pending */
        D_8002AFA8 = *(s32 *)(sc + 0x27C);
        D_8002AFA4 = 0;
    }

    /* Notify all clients of retrace */
    client = *(OSScClient **)(sc + 0x260);
    while (client != NULL) {
        osSendMesg(client->msgQueue, sc, 0);
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

    if (*(void **)(sc + 0x274) == NULL) {
        return;
    }

    task = *(void **)(sc + 0x274);
    *(void **)(sc + 0x274) = NULL;

    /* Check if task yielded and has pending work */
    if ((*(s32 *)((u8 *)task + 4) & 0x10) && osSpTaskYielded(task + 0x10) != 0) {
        /* Mark as yielded */
        *(s32 *)((u8 *)task + 4) |= 0x20;

        /* If graphics task, add back to RDP queue */
        if ((*(s32 *)((u8 *)task + 8) & 7) == 3) {
            *(void **)task = *(void **)(sc + 0x268);
            *(void **)(sc + 0x268) = task;
            if (*(void **)(sc + 0x270) == NULL) {
                *(void **)(sc + 0x270) = task;
            }
        }
    } else {
        /* Clear RSP busy flag */
        *(s32 *)((u8 *)task + 4) &= ~2;
        __scTaskComplete(sc, task);
    }

    /* Reschedule */
    flags = ((*(void **)(sc + 0x274) == NULL) << 1) | (*(void **)(sc + 0x278) == NULL);
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

    if (*(void **)(sc + 0x278) == NULL) {
        return;
    }

    /* Verify task type is graphics (1) */
    if (*(s32 *)(*(u8 **)(sc + 0x278) + 0x10) != 1) {
        return;
    }

    task = *(void **)(sc + 0x278);
    *(void **)(sc + 0x278) = NULL;

    /* Clear RDP busy flag */
    *(s32 *)((u8 *)task + 4) &= ~1;
    __scTaskComplete(sc, task);

    /* Reschedule */
    flags = ((*(void **)(sc + 0x274) == NULL) << 1) | (*(void **)(sc + 0x278) == NULL);
    if (__scScheduleCore(sc, &rspTask, &rdpTask, flags) != flags) {
        __scExec(sc, rspTask, rdpTask);
    }
}

/* External VI functions */
extern s32 osViGetCurrentFramebuffer(void);
extern void *osViGetFramebuffer(void);
extern void func_80001D60(void);  /* VI sync/update */

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
        if (D_8002AFA4 != 0 && (u32)(*(s32 *)(scheduler + 0x27C) - D_8002AFA8) < 2) {
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
    if (!(*(s32 *)(t + 4) & 3)) {
        /* Send completion message to client */
        result = osSendMesg(*(void **)(t + 0x50), *(void **)(t + 0x54), 1);

        /* Handle graphics task completion */
        if (*(s32 *)(t + 0x10) == 1) {
            D_8002EB70--;

            flags = *(s32 *)(t + 8);
            /* Check for swap buffer flag (0x40) and completed (0x20) */
            if ((flags & 0x40) && (flags & 0x20)) {
                if ((u32)(*(s32 *)(sc + 0x27C) - D_8002AFA8) >= 2) {
                    D_8002AFA8 = *(s32 *)(sc + 0x27C);
                    osViSwapBuffer(*(void **)(t + 0xC));
                    func_80001D60();
                } else {
                    D_8002AFA4 = *(s32 *)(t + 0xC);
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

    taskType = *(s32 *)(task + 0x10);

    /* Task type 2 = audio, type 1 = graphics */
    if (taskType == 2) {
        /* Audio task - add to RSP queue */
        if (*(void **)(sc + 0x26C) != NULL) {
            *(void **)*(void **)(sc + 0x26C) = task;
        } else {
            *(void **)(sc + 0x264) = task;
        }
        *(void **)(sc + 0x26C) = task;

        /* Mark audio pending */
        *(s32 *)(sc + 0x280) = 1;
    } else {
        /* Graphics task - add to RDP queue */
        if (*(void **)(sc + 0x270) != NULL) {
            *(void **)*(void **)(sc + 0x270) = task;
        } else {
            *(void **)(sc + 0x268) = task;
        }
        *(void **)(sc + 0x270) = task;
    }

    /* Clear next pointer and set busy flags from task flags */
    *(void **)task = NULL;
    *(s32 *)(task + 4) = *(s32 *)(task + 8) & 3;
}

/* External SP/DP functions */
extern void osSpClearStatus(void);
extern void osSpTaskLoad(void *task);
extern void osSpTaskStartGo(void *task);
extern s32 osDpSetNextBuffer(void *buf, s32 size);

/**
 * Execute RSP/RDP tasks
 * (func_80000E20 - __scExec)
 */
static void __scExec(void *arg, void *rspTask, void *rdpTask) {
    u8 *sc = (u8 *)arg;
    u8 *rsp = (u8 *)rspTask;
    u8 *rdp = (u8 *)rdpTask;

    if (*(void **)(sc + 0x274) != NULL) {
        return;
    }

    if (rspTask != NULL) {
        /* Audio task (type 2) */
        if (*(s32 *)(rsp + 0x10) == 2) {
            D_8002EB78 = osGetCount();
            D_8002AFA0 = rspTask;
        } else if (!(*(s32 *)(rsp + 4) & 0x20)) {
            /* Not yielded - record time */
            D_8002E8E0 = D_8002EB80;
        }

        /* Clear SP status and load task */
        osSpClearStatus();
        *(s32 *)(rsp + 4) &= ~0x30;
        osSpTaskLoad(rsp + 0x10);
        osSpTaskStartGo(rsp + 0x10);

        *(void **)(sc + 0x274) = rspTask;
        if (rspTask == rdpTask) {
            *(void **)(sc + 0x278) = rdpTask;
        }
    }

    /* Set up RDP if different task */
    if (rdpTask != NULL && rdpTask != rspTask) {
        osDpSetNextBuffer(*(void **)(rdp + 0x38), 0);
        *(void **)(sc + 0x278) = rdpTask;
    }
}

/**
 * Request current RSP task to yield
 * (func_80000F88 - __scYield)
 */
static void __scYield(void *arg) {
    u8 *sc = (u8 *)arg;
    u8 *task;

    /* Only yield graphics tasks (type 1) */
    if (*(s32 *)(*(u8 **)(sc + 0x274) + 0x10) == 1) {
        task = *(u8 **)(sc + 0x274);
        /* Set yield request flag */
        *(s32 *)(task + 4) |= 0x10;
        osSpTaskYield();
    }
}

/* External yield function */
extern void osSpTaskYield(void);

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

    rspHead = *(void **)(scheduler + 0x264);
    rdpHead = *(void **)(scheduler + 0x268);

    /* Check RSP queue (bit 1 = RSP available) */
    if ((flags & 2) && rspHead != NULL) {
        *rsp = rspHead;
        *(void **)(scheduler + 0x264) = *(void **)rspHead;
        if (*(void **)(scheduler + 0x264) == NULL) {
            *(void **)(scheduler + 0x26C) = NULL;
        }
        flags &= ~2;

        /* Audio task clears pending flag */
        if (*(s32 *)((u8 *)rspHead + 0x10) == 2) {
            *(s32 *)(scheduler + 0x280) = 0;
        }
    }

    /* Check RDP queue (bit 0 = RDP available) */
    if ((flags & 1) && rdpHead != NULL) {
        *rdp = rdpHead;
        *(void **)(scheduler + 0x268) = *(void **)rdpHead;
        if (*(void **)(scheduler + 0x268) == NULL) {
            *(void **)(scheduler + 0x270) = NULL;
        }
        flags &= ~1;
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

    D_8002EBA4 = D_8002EB64;
    temp = 2.0f * D_8002AFB8;
    D_8002EB94 = temp;
    D_8002EB90 = temp;

    __osRestoreInt(saved);
}

/**
 * Enable time accumulation
 * (func_800013C0)
 */
void osScEnableTime(void) {
    D_8002AFB0 = 1;
}

/**
 * Disable time accumulation
 * (func_800013DC)
 */
void osScDisableTime(void) {
    D_8002AFB0 = 0;
}

/**
 * Update frame timing
 * (func_800013F4)
 */
void osScUpdateTime(void) {
    D_8002EB98 = D_8002EB64 - D_8002EBA4;
    D_8002EBA4 = D_8002EB64;

    if (D_8002AFB0 != 0) {
        D_8002EBA0 += D_8002EB98;
        D_8002EB9C += D_8002EB98;
    } else {
        D_8002EB90 = (f32)(D_8002EB64 - D_8002EBA0) * D_8002AFB8;
    }

    /* Clamp delta to max 5 frames */
    if (D_8002EB98 >= 6) {
        D_8002EB98 = 5;
    }

    D_8002EB94 = (f32)D_8002EB98 * D_8002AFB8;
}

/**
 * Set deadline timer
 * (func_800014F0)
 */
void osScSetDeadline(f32 time) {
    D_8002EBA0 = D_8002EB64;
    D_8002EB9C = (s32)(time * D_8002AFB4) + D_8002EB64;
}

/**
 * Add time to deadline
 * (func_8000153C)
 */
void osScAddDeadline(f32 time) {
    D_8002EB9C += (s32)(time * D_8002AFB4);
}

/**
 * Get time remaining until deadline
 * (func_80001578)
 */
f32 osScGetTimeRemaining(void) {
    return (f32)(D_8002EB9C - D_8002EB64) * D_8002AFB8;
}

/**
 * Check if deadline has passed
 * (func_800015BC)
 */
s32 osScDeadlinePassed(void) {
    return (D_8002EB9C - D_8002EB64) < 1;
}

/**
 * Stub function
 * (func_800015DC)
 */
void osScStub(void) {
    /* Empty */
}
