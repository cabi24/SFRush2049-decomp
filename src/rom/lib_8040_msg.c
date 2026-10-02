/* GENERATED ROM-aligned TU — segment 0x8040 (rom/lib_8040_msg)
 * layout map fd8409c21b34dc7bbde95e98487864907c2fad0185f37de73dcc6e618d72f33e; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */

#include "rom_tu.h"


/* PROMOTED 2026-10-02 — osSendMesg
 * Source:   module_campaign_20261002 current-header SDK publication
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: independent canonical complete native body
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osSendMesg(OSMesgQueue* mq, OSMesg msg, s32 flag) {
    register u32 saveMask;


    saveMask = __osDisableInt();

    while (mq->validCount >= mq->msgCount) {
        if (flag == OS_MESG_BLOCK) {
            __osRunningThread->state = OS_STATE_WAITING;
            __osCleanupThread(&mq->fullqueue);
        } else {
            __osRestoreInt(saveMask);
            return -1;
        }
    }

    mq->first = (mq->first + mq->msgCount - 1) % mq->msgCount;
    mq->msg[mq->first] = msg;
    mq->validCount++;

    if (mq->mtqueue->next != NULL) {
        osStartThread(__osPopThread(&mq->mtqueue));
    }

    __osRestoreInt(saveMask);
    return 0;
}



