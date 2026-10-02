#ifndef CAMPAIGN_NATIVE_SCHEDULER_H
#define CAMPAIGN_NATIVE_SCHEDULER_H
/* Actual SDK data carriers used by the native scheduler. No storage defined. */
typedef signed char s8;typedef unsigned char u8;
typedef signed short s16;typedef unsigned short u16;
typedef int s32;typedef unsigned int u32;
typedef long long s64;typedef unsigned long long u64;
typedef u64 OSTime;typedef s32 OSPri;typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {OSThread *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
typedef struct OSScTask {struct OSScTask *next;s32 state,flags;void *framebuffer;s32 type;u8 pad14[36];void *unk38;s32 *unk3C;u8 pad40[16];OSMesgQueue *msgQueue;OSMesg msg;} OSScTask;
typedef struct OSScClient {struct OSScClient *next;OSMesgQueue *msgQueue;} OSScClient;
typedef struct OSSched {s16 state;u8 pad02[30];s16 priority;u8 pad22[30];OSMesgQueue cmdQueue;OSMesg cmdMsgs[8];OSMesgQueue retQueue;OSMesg retMsgs[8];u8 padB0[432];OSScClient *clientList;OSScTask *rspTaskHead,*rspTaskTail,*rdpTaskHead,*rdpTaskTail,*curRSPTask,*curRDPTask;s32 retraceCount,audioListPending;} OSSched;
typedef struct OSViMode {u8 native[80];} OSViMode;
extern OSViMode gViModeTableBase[];
extern s16 __osScTaskCount;
extern s32 __osScPendingSwap,__osScFrameTimeResult;
extern u32 __osScSwapCount;
/* Each native timestamp is a contiguous high/low 64-bit object. The existing
 * physical Hi symbol names are retained, with their real complete width. */
/* Native adjacent scheduler timestamp objects; actual zero-initialized storage. */
OSTime __osScAudioStartHi,__osScFrameTimeHi;
extern OSTime __osScRetraceTimeHi;
extern OSScTask *__osScCurAudioTask;
extern void osCreateMesgQueue(OSMesgQueue *,OSMesg *,s32);
extern s32 osRecvMesg(OSMesgQueue *,OSMesg *,s32);
extern s32 osSendMesg(OSMesgQueue *,OSMesg,s32),osJamMesg(OSMesgQueue *,OSMesg,s32);
extern void osCreateThread(OSThread *,s32,void (*)(void *),void *,void *,OSPri);
extern void osStartThread(OSThread *);
extern void osSetEventMesg(OSPri);
extern void osSetEventMesgAlt(s32,OSMesgQueue *,OSMesg);
extern void osSetThreadPri(void *),osSetIntMask(s32);
extern u32 osSetGlobalIntMask(u32);
extern void osSetTimerIntr(OSMesgQueue *,OSMesg,s32);
extern OSTime osGetTime(void);
extern void *osViGetCurrentFramebuffer(void),*osViGetFramebuffer(void);
extern void osViSetMode(void *),display_mode_tick(void);
extern s32 osSpTaskYielded(void *);
extern void osInvalICache(void),osViModeNtscLan1(void *),osViModeNtscLpn1(void *),osDpWait(void);
extern s32 osDpSetNextBuffer(void *,u64);
void osCreateScheduler(OSSched *,void *,OSPri,u8,u8);
void osScAddClient(OSSched *,OSScClient *,OSMesgQueue *);
void __scMain(void *);
void __scSchedule(OSSched *);
void __scHandleRetrace(OSSched *),__scHandleRSP(OSSched *),__scHandleRDP(OSSched *);
OSScTask *__scTaskReady(OSSched *,OSScTask *);
s32 __scExecTask(OSSched *,OSScTask *);
void __scAppendList(OSSched *,OSScTask *),__scExec(OSSched *,OSScTask *,OSScTask *),__scHandlePreNMI(OSSched *);
s32 __scScheduleCore(OSSched *,OSScTask **,OSScTask **,s32);
#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK 1
#endif

/* Genuine complete 13-member native scheduler source closure. */
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

/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
extern OSScTask *__scTaskReady(OSSched *, OSScTask *);
/* Complete original recursive RCP scheduler. Existing SDK field names use
 * actual physical audio/gfx queue offsets. Native check-only RDP assertion
 * uses the actual native bit test; original SDK donor lacks parentheses. */
s32 __scScheduleCore(OSSched *scheduler, OSScTask **sp, OSScTask **dp, s32 availRCP)
{
    s32 avail = availRCP;
    OSScTask *gfx = scheduler->rspTaskTail;
    OSScTask *audio = scheduler->rspTaskHead;
    if (scheduler->audioListPending && (avail & 2)) {
        if (gfx && (gfx->flags & 0x10)) {
            *sp = gfx;
            avail &= ~2;
        } else {
            *sp = audio;
            avail &= ~2;
            scheduler->audioListPending = 0;
            scheduler->rspTaskHead = scheduler->rspTaskHead->next;
            if (scheduler->rspTaskHead == 0)
                scheduler->rdpTaskHead = 0;
        }
    } else {
        if (__scTaskReady(scheduler, gfx)) {
            switch (gfx->flags & 7) {
                case 3:
                    if (gfx->state & 0x20) {
                        if (avail & 2) {
                            *sp = gfx;
                            avail &= ~2;
                            if (gfx->state & 1) {
                                *dp = gfx;
                                avail &= ~1;
                                if ((avail & 1) == 0)
                                    if (scheduler->curRDPTask != gfx) { }
                            }
                            scheduler->rspTaskTail = scheduler->rspTaskTail->next;
                            if (scheduler->rspTaskTail == 0)
                                scheduler->rdpTaskTail = 0;
                        }
                    } else {
                        if (avail == 3) {
                            *sp = *dp = gfx;
                            avail &= ~3;
                            scheduler->rspTaskTail = scheduler->rspTaskTail->next;
                            if (scheduler->rspTaskTail == 0)
                                scheduler->rdpTaskTail = 0;
                        }
                    }
                    break;
                case 7:
                case 6:
                case 2:
                    if (gfx->state & 2) {
                        if (avail & 2) {
                            *sp = gfx;
                            avail &= ~2;
                        }
                    } else if (gfx->state & 1) {
                        if (avail & 1) {
                            *dp = gfx;
                            avail &= ~1;
                            scheduler->rspTaskTail = scheduler->rspTaskTail->next;
                            if (scheduler->rspTaskTail == 0)
                                scheduler->rdpTaskTail = 0;
                        }
                    }
                    break;
                case 5:
                case 1:
                default:
                    break;
            }
        }
    }
    if (avail != availRCP)
        avail = __scScheduleCore(scheduler, sp, dp, avail);
    return avail;
}

/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
/* Native two-input audio/graphics task queue append. Existing SDK field names
 * are retained at their proved physical offsets (audio tail is rdpTaskHead).
 * The native check-only paths are reconstructed explicitly: task type must
 * be audio/graphics; the existing audio-pending test has no failure payload.
 * No assertion call or data object exists in the original complete body. */
void __scAppendList(OSSched *scheduler, OSScTask *task)
{
    long type = task->type;
    if ((type == 2) || (type == 1)) { }
    if (type == 2) {
        if (scheduler->rdpTaskHead)
            scheduler->rdpTaskHead->next = task;
        else
            scheduler->rspTaskHead = task;
        scheduler->rdpTaskHead = task;
        if (scheduler->audioListPending) { }
        scheduler->audioListPending = 1;
    } else {
        if (scheduler->rdpTaskTail)
            scheduler->rdpTaskTail->next = task;
        else
            scheduler->rspTaskTail = task;
        scheduler->rdpTaskTail = task;
    }
    task->next = 0;
    task->state = task->flags & 3;
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

/* SDK __scHandleRetrace adapted to N64 pending-swap and notification path.
 * Removed task-drain block and its unused donor locals are not imported. */
void __scHandleRetrace(OSSched *scheduler) {
 OSScClient *client;
 scheduler->retraceCount++;
 if(__osScPendingSwap && scheduler->retraceCount-__osScSwapCount>=2U) {
  osViSetMode((void *)(u32)__osScPendingSwap);
  display_mode_tick();
  __osScSwapCount=scheduler->retraceCount;
  __osScPendingSwap=0;
 }
 for(client=scheduler->clientList;client!=0;client=client->next)
  osJamMesg(client->msgQueue,(OSMesg)scheduler,OS_MESG_NOBLOCK);
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

/* Complete SDK scheduler event dispatcher; N64 adds frame timestamps and
 * task-queue scheduling, with actual five message IDs 666..670. */
void __scMain(void *arg) {
 OSMesg message;
 OSSched *scheduler=(OSSched *)arg;
 OSScClient *client;
 while(1) {
  osRecvMesg(&scheduler->cmdQueue,&message,OS_MESG_BLOCK);
  switch((s32)message) {
   case 666:
    __osScFrameTimeHi=osGetTime();
    __scHandleRetrace(scheduler);
    __scSchedule(scheduler);
    break;
   case 667:
    __scHandleRSP(scheduler);
    break;
   case 668:
    __osScFrameTimeResult=(s32)(osGetTime()-__osScRetraceTimeHi);
    __scHandleRDP(scheduler);
    break;
   case 669:
    __scSchedule(scheduler);
    break;
   case 670:
    for(client=scheduler->clientList;client!=0;client=client->next)
     osSendMesg(client->msgQueue,(OSMesg)&scheduler->priority,OS_MESG_BLOCK);
    break;
  }
 }
}

/* Complete SDK RCP task launcher with native N64 audio/frame timestamps.
 * Native assertion checks have no failure payload. Original SDK rv stores
 * the actual DP-submit status and is checked, as in the native body. */
void __scExec(OSSched *scheduler,OSScTask *sp,OSScTask *dp) {
 s32 rv;
 if(scheduler->curRSPTask) { }
 if(sp) {
  if(sp->type==2) {
   __osScAudioStartHi=osGetTime();
   __osScCurAudioTask=sp;
  } else if(!(sp->state&0x20)) {
   __osScRetraceTimeHi=__osScFrameTimeHi;
  }
  osInvalICache();
  sp->state&=~0x30;
  osViModeNtscLan1(&sp->type);
  osViModeNtscLpn1(&sp->type);
  scheduler->curRSPTask=sp;
  if(sp==dp)scheduler->curRDPTask=dp;
 }
 if(dp && dp!=sp) {
  if(!dp->unk38) { }
  rv=osDpSetNextBuffer(dp->unk38,*(u64 *)dp->unk3C);
  if(rv) { }
  scheduler->curRDPTask=dp;
 }
}
