/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
#include "rom_tu.h"
#include "static_scheduler_context.h"
/* Complete actual scheduler retrace. Locals retain original SDK declarations
 * from sched.c215-223; this game's task-drain path is absent in native code.
 * rspTask/i/state and initialized sp/dp remain genuinely unused donor locals,
 * disclosed as a source hypothesis rather than invented register pressure. */
void __scHandleRetrace(OSSched *scheduler)
{
    OSScTask *rspTask;
    OSScClient *client;
    s32 i;
    s32 state;
    OSScTask *sp = 0;
    OSScTask *dp = 0;
    scheduler->retraceCount++;
    if (__osScPendingSwap && scheduler->retraceCount - __osScSwapCount >= 2U) {
        osViSetMode((void *)(u32)__osScPendingSwap);
        display_mode_tick();
        __osScSwapCount = scheduler->retraceCount;
        __osScPendingSwap = 0;
    }
    for (client = scheduler->clientList; client != 0; client = client->next) {
        osJamMesg(client->msgQueue, (OSMesg)scheduler, OS_MESG_NOBLOCK);
    }
}
