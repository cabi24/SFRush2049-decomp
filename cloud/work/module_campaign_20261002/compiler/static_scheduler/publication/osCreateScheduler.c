/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
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
