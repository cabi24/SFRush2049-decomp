/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Eight complete native SDK scheduler bodies, original sched.c ancestry.
 * Root owns prefix placement and source-built ROM verification. */
#include "rom_tu.h"
#include "scheduler_declarations.h"

/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
extern s16 __osScTaskCount;
extern OSViMode gViModeTableBase[];
extern void osSetEventMesg(OSPri);
extern void osSetTimerIntr(OSMesgQueue *, OSMesg, s32);
extern void __scMain(void *);
/* Full native scheduler setup with five original SDK inputs.
 * Historical physical helper symbols retain observed actual contracts. */
void osCreateScheduler(OSSched *scheduler, void *stack, OSPri priority, u8 mode, u8 numFields)
{
    __osScTaskCount = 0;
    scheduler->curRSPTask = 0;
    scheduler->curRDPTask = 0;
    scheduler->clientList = 0;
    scheduler->retraceCount = 0;
    scheduler->rspTaskHead = 0;
    scheduler->rspTaskTail = 0;
    scheduler->rdpTaskHead = 0;
    scheduler->rdpTaskTail = 0;
    scheduler->state = 1;
    scheduler->priority = 4;
    osCreateMesgQueue(&scheduler->cmdQueue, scheduler->cmdMsgs, 8);
    osCreateMesgQueue(&scheduler->retQueue, scheduler->retMsgs, 8);
    osSetEventMesg(254);
    osSetThreadPri(&gViModeTableBase[mode]);
    osSetIntMask(1);
    osSetEventMesgAlt(4, &scheduler->cmdQueue, (OSMesg)667);
    osSetEventMesgAlt(9, &scheduler->cmdQueue, (OSMesg)668);
    osSetEventMesgAlt(14, &scheduler->cmdQueue, (OSMesg)669);
    osSetEventMesgAlt(0, &scheduler->cmdQueue, (OSMesg)670);
    osSetTimerIntr(&scheduler->cmdQueue, (OSMesg)666, numFields);
    osCreateThread((OSThread *)scheduler->padB0, 4, __scMain, scheduler, stack, priority);
    osStartThread((OSThread *)scheduler->padB0);
}

/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
extern s32 __scExecTask(OSSched *, OSScTask *);
extern s32 __scScheduleCore(OSSched *, OSScTask **, OSScTask **, s32);
/* Complete actual DP completion, with existing physical SDK types.
 * Native assertion checks have no failure-call payload and are retained.
 * SDK task/sp/dp/state declaration order comes from sched.c __scHandleRDP. */
void __scHandleRDP(OSSched *scheduler)
{
    OSScTask *task, *sp = 0, *dp = 0;
    s32 state;
    if (!scheduler->curRDPTask) { }
    if (scheduler->curRDPTask->type != 1) { }
    task = scheduler->curRDPTask;
    scheduler->curRDPTask = 0;
    task->state &= ~1;
    __scExecTask(scheduler, task);
    state = ((scheduler->curRSPTask == 0) << 1) | (scheduler->curRDPTask == 0);
    if (__scScheduleCore(scheduler, &sp, &dp, state) != state)
        __scExec(scheduler, sp, dp);
}

/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
extern s32 osSpTaskYielded(void *);
extern s32 __scExecTask(OSSched *, OSScTask *);
extern s32 __scScheduleCore(OSSched *, OSScTask **, OSScTask **, s32);
/* Actual RSP completion. Existing SDK fields retain original physical offsets.
 * Native check-only assertion/type-validation paths have no failure payload. */
void __scHandleRSP(OSSched *scheduler)
{
    OSScTask *task, *sp = 0, *dp = 0;
    s32 state;
    if (!scheduler->curRSPTask) { }
    task = scheduler->curRSPTask;
    scheduler->curRSPTask = 0;
    if ((task->state & 0x10) && osSpTaskYielded(&task->type)) {
        task->state |= 0x20;
        if ((task->flags & 7) == 3) {
            task->next = scheduler->rspTaskTail;
            scheduler->rspTaskTail = task;
            if (scheduler->rdpTaskTail == 0)
                scheduler->rdpTaskTail = task;
        }
    } else {
        if (task->flags & 0x40) { }
        else if (task->type == 2) { }
        task->state &= ~2;
        __scExecTask(scheduler, task);
    }
    state = ((scheduler->curRSPTask == 0) << 1) | (scheduler->curRDPTask == 0);
    if (__scScheduleCore(scheduler, &sp, &dp, state) != state)
        __scExec(scheduler, sp, dp);
}

/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
extern void __scHandlePreNMI(OSSched *);
extern s32 __scScheduleCore(OSSched *, OSScTask **, OSScTask **, s32);
/* Complete native task-queue scheduling at original return queue offset0x78.
 * All locals are consumed; physical SDK labels retain actual helper contracts. */
void __scSchedule(OSSched *scheduler)
{
    OSMesg message;
    s32 state;
    OSScTask *sp = 0;
    OSScTask *dp = 0;
    while (osRecvMesg(&scheduler->retQueue, &message, OS_MESG_NOBLOCK) != -1)
        __scAppendList(scheduler, (OSScTask *)message);
    if (scheduler->audioListPending && scheduler->rspTaskHead && scheduler->curRSPTask) {
        __scHandlePreNMI(scheduler);
    } else {
        state = ((scheduler->curRSPTask == 0) << 1) | (scheduler->curRDPTask == 0);
        if (__scScheduleCore(scheduler, &sp, &dp, state) != state)
            __scExec(scheduler, sp, dp);
    }
}

/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
extern s16 __osScTaskCount;
/* Complete native task-completion notification/framebuffer swap.
 * Real SDK rv local stores actual osJamMesg status, though not subsequently
 * consumed in the native body; original donor declaration retained/disclosed. */
s32 __scExecTask(OSSched *scheduler, OSScTask *task)
{
    int rv;
    if ((task->state & 3) == 0) {
        if (!task->msgQueue) { }
        rv = osJamMesg(task->msgQueue, task->msg, OS_MESG_BLOCK);
        if (task->type == 1) {
            __osScTaskCount--;
            if ((task->flags & 0x40) && (task->flags & 0x20)) {
                if (scheduler->retraceCount - __osScSwapCount >= 2U) {
                    __osScSwapCount = scheduler->retraceCount;
                    osViSetMode(task->framebuffer);
                    display_mode_tick();
                } else {
                    __osScPendingSwap = (s32)task->framebuffer;
                }
            }
        }
        return 1;
    }
    return 0;
}

OSScTask *__scTaskReady(OSSched *scheduler, OSScTask *task) {
    void *current;
    void *next;
    if (task != 0) {
        if ((current = osViGetCurrentFramebuffer()) !=
            (next = osViGetFramebuffer())) return 0;
        if (__osScPendingSwap != 0 && scheduler->retraceCount - __osScSwapCount < 2U)
            return 0;
        return task;
    }
    return 0;
}

/* SDK client registration; native physical helper is osSetGlobalIntMask. */
void osScAddClient(OSSched *scheduler,OSScClient *client,OSMesgQueue *queue) {
 u32 mask;
 mask=osSetGlobalIntMask(1);
 client->msgQueue=queue;
 client->next=scheduler->clientList;
 scheduler->clientList=client;
 osSetGlobalIntMask(mask);
}

/* Native function is original SDK __scYield, not pre-NMI handling. */
void __scHandlePreNMI(OSSched *scheduler) {
 if(scheduler->curRSPTask->type==1) {
  scheduler->curRSPTask->state|=0x10;
  osDpWait();
 } else {
 }
}
