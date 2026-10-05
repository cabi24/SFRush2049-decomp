/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
#include "rom_tu.h"
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
