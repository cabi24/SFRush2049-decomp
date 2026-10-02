/* GENERATED ROM-aligned TU — segment 0x1050 (rom/lib_1050_sdk)
 * layout map af9176b4b4f60ff529afaec19f5236cce01f4a00e4b6a10f392108db260a7b70; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */

#include "rom_tu.h"
#include "static_scheduler_context.h"


/* PROMOTED 2026-10-02 — osCreateScheduler
 * Source:   cloud/work/static_acceptance/SDK_candidates/osCreateScheduler.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_acceptance/SDK_candidates/osCreateScheduler.c:osCreateScheduler (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-02 — osScAddClient
 * Source:   cloud/work/static_acceptance/SDK_candidates/osScAddClient.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm
 * Evidence: lock:cloud/work/static_acceptance/SDK_candidates/osScAddClient.c:osScAddClient (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osScAddClient(OSSched *scheduler, OSScClient *client, OSMesgQueue *queue) {
    u32 mask;
    mask = osSetGlobalIntMask(1);
    client->msgQueue = queue;
    client->next = scheduler->clientList;
    scheduler->clientList = client;
    osSetGlobalIntMask(mask);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scMain.s")
/* PROMOTED 2026-10-02 — __scSchedule
 * Source:   cloud/work/static_acceptance/SDK_candidates/__scSchedule.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_acceptance/SDK_candidates/__scSchedule.c:__scSchedule (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scHandleRetrace.s")
/* PROMOTED 2026-10-02 — __scHandleRSP
 * Source:   cloud/work/static_acceptance/SDK_candidates/__scHandleRSP.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm
 * Evidence: lock:cloud/work/static_acceptance/SDK_candidates/__scHandleRSP.c:__scHandleRSP (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-02 — __scHandleRDP
 * Source:   cloud/work/static_acceptance/SDK_candidates/__scHandleRDP.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm
 * Evidence: lock:cloud/work/static_acceptance/SDK_candidates/__scHandleRDP.c:__scHandleRDP (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-02 — __scTaskReady
 * Source:   cloud/work/static_acceptance/SDK_candidates/__scTaskReady.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm
 * Evidence: lock:cloud/work/static_acceptance/SDK_candidates/__scTaskReady.c:__scTaskReady (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-02 — __scExecTask
 * Source:   cloud/work/static_acceptance/SDK_candidates/__scExecTask.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_acceptance/SDK_candidates/__scExecTask.c:__scExecTask (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scAppendList.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1050_sdk/__scExec.s")
/* PROMOTED 2026-10-02 — __scHandlePreNMI
 * Source:   cloud/work/static_acceptance/SDK_candidates/__scHandlePreNMI.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_acceptance/SDK_candidates/__scHandlePreNMI.c:__scHandlePreNMI (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __scHandlePreNMI(OSSched *scheduler)
{
    if (scheduler->curRSPTask->type == 1) {
        scheduler->curRSPTask->state |= 0x10;
        osDpWait();
    } else {
    }
}

/* PROMOTED 2026-10-02 — __scScheduleCore
 * Source:   cloud/work/static_acceptance/SDK_candidates/__scScheduleCore.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_acceptance/SDK_candidates/__scScheduleCore.c:__scScheduleCore (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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




