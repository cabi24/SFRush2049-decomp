/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
#include "scheduler_declarations.h"
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
