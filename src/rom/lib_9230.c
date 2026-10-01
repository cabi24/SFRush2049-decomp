/* GENERATED ROM-aligned TU — segment 0x9230 (rom/lib_9230)
 * layout map db0e580c5ebcb31e73af939792e5cb68899d06e5c02bac5a099593008014f6d2; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — __osPiRawStartDma
 * Source:   cloud/work/static_C8/__osPiRawStartDma.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C8/__osPiRawStartDma.c:__osPiRawStartDma (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osPiRawStartDma(OSIoMesg* mb, s32 priority, s32 direction, u32 devAddr, void* dramAddr, u32 size, OSMesgQueue* mq) {
    register s32 ret;
    if (!__osPiMgrState.flag) {
        return -1;
    }


    if (direction == OS_READ) {
        mb->hdr.type = OS_MESG_TYPE_DMAREAD;
    } else {
        mb->hdr.type = OS_MESG_TYPE_DMAWRITE;
    }

    mb->hdr.pri = priority;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = devAddr;
    mb->size = size;
    mb->piHandle = NULL;

    if (priority == OS_MESG_PRI_HIGH) {
        ret = osSendMesg((OSMesgQueue *)__osInsertTimer(), (OSMesg)mb, OS_MESG_NOBLOCK);
    } else {
        ret = osJamMesg((OSMesgQueue *)__osInsertTimer(), (OSMesg)mb, OS_MESG_NOBLOCK);
    }

    return ret;
}

