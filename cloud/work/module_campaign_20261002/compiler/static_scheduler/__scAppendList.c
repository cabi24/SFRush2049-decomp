#include "scheduler_native.h"
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
