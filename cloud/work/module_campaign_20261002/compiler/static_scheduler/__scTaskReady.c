#include "scheduler_native.h"
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
