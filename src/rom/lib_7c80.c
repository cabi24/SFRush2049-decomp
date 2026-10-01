/* GENERATED ROM-aligned TU — segment 0x7c80 (rom/lib_7c80)
 * layout map a452e3f8ed80e93cc04393c69a0b02f777d956fac41af97e742e46319d590aaf; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osStartThread
 * Source:   cloud/work/static_C4/osStartThread.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C4/osStartThread.c:osStartThread (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osStartThread(OSThread* t) {
    register u32 saveMask = __osDisableInt();

    switch (t->state) {
        case OS_STATE_WAITING:
            t->state = OS_STATE_RUNNABLE;
            __osEnqueueThread(&__osActiveQueue, t);
            break;
        case OS_STATE_STOPPED:
            if (t->queue == NULL || t->queue == &__osActiveQueue) {
                t->state = OS_STATE_RUNNABLE;
                __osEnqueueThread(&__osActiveQueue, t);
            } else {
                t->state = OS_STATE_WAITING;
                __osEnqueueThread(t->queue, t);
                __osEnqueueThread(&__osActiveQueue, __osPopThread(t->queue));
            }
            break;
#ifdef _DEBUG
        default:
            __osError(ERR_OSSTARTTHREAD, 0);
            __osRestoreInt(saveMask);
            return;
#endif
    }

    if (__osRunningThread == NULL) {
        __osDispatchThread();
    } else if (__osRunningThread->priority < __osActiveQueue->priority) {
        __osRunningThread->state = OS_STATE_RUNNABLE;
        __osCleanupThread(&__osActiveQueue);
    }

    __osRestoreInt(saveMask);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_7c80/osSetGlobalIntMask.s")
/* PROMOTED 2026-10-01 — osRecvMesg
 * Source:   cloud/work/static_C4/osRecvMesg.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C4/osRecvMesg.c:osRecvMesg (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osRecvMesg(OSMesgQueue* mq, OSMesg* msg, s32 flags) {
    register u32 saveMask;

#ifdef _DEBUG
    if ((flags != OS_MESG_NOBLOCK) && (flags != OS_MESG_BLOCK)) {
        __osError(ERR_OSRECVMESG, 1, flags);
        return -1;
    }
#endif

    saveMask = __osDisableInt();

    while (MQ_IS_EMPTY(mq)) {
        if (flags == OS_MESG_NOBLOCK) {
            __osRestoreInt(saveMask);
            return -1;
        } else {
            __osRunningThread->state = OS_STATE_WAITING;
            __osCleanupThread(&mq->mtqueue);
        }
    }

    if (msg != NULL) {
        *msg = mq->msg[mq->first];
    }

    mq->first = (mq->first + 1) % mq->msgCount;
    mq->validCount--;

    if (mq->fullqueue->next != NULL) {
        osStartThread(__osPopThread(&mq->fullqueue));
    }

    __osRestoreInt(saveMask);
    return 0;
}

