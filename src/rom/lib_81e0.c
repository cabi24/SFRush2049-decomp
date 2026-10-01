/* GENERATED ROM-aligned TU — segment 0x81e0 (rom/lib_81e0)
 * layout map 8e70ddc130b05a84c93f5d95408f57a1d8eeb3eb18286495c541dcf6a1bcb1d1; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osJamMesg
 * Source:   cloud/work/static_C7/osJamMesg.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C7/osJamMesg.c:osJamMesg (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osJamMesg(OSMesgQueue* mq, OSMesg msg, s32 flags) {
    register u32 saveMask;
    register s32 last;


    saveMask = __osDisableInt();

    while (MQ_IS_FULL(mq)) {
        if (flags == OS_MESG_BLOCK) {
            __osRunningThread->state = OS_STATE_WAITING;
            __osCleanupThread(&mq->fullqueue);
        } else {
            __osRestoreInt(saveMask);
            return -1;
        }
    }

    last = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[last] = msg;
    mq->validCount++;

    if (mq->mtqueue->next != NULL) {
        osStartThread(__osPopThread(&mq->mtqueue));
    }

    __osRestoreInt(saveMask);
    return 0;
}

