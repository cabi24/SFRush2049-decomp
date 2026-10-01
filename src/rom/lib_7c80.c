/* GENERATED ROM-aligned TU — segment 0x7c80 (rom/lib_7c80)
 * layout map a452e3f8ed80e93cc04393c69a0b02f777d956fac41af97e742e46319d590aaf; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_7c80/osStartThread.s")
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

