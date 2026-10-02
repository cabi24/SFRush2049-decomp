/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
#include "scheduler_declarations.h"
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
