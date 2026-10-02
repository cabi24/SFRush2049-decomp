/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
#include "scheduler_declarations.h"
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
