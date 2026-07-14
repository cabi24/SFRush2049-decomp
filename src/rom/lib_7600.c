/* GENERATED ROM-aligned TU — segment 0x7600 (rom/lib_7600)
 * layout map dadeb1437c625ee4a3fa6a890930dee6bced3d2e607b76262a3328452aa4a03b; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-07-14 — osCreateMesgQueue
 * Source:   src/libultra/os_message.c (in-repo, locked)
 * Flags:    -g0 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:src/libultra/os_message.c:osCreateMesgQueue (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count) {
    mq->mtqueue = (OSThread *)&__osEmptyMesgQueue;
    mq->fullqueue = (OSThread *)&__osEmptyMesgQueue;
    mq->validCount = 0;
    mq->first = 0;
    mq->msgCount = count;
    mq->msg = msg;
}

