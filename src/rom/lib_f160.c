/* GENERATED ROM-aligned TU — segment 0xf160 (rom/lib_f160)
 * layout map d685f3d6dab9373376d3ef3c9803ad25e7e8685c23458b05fb9c1a12d42f1036; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osSiInit
 * Source:   cloud/work/static_C/osSiInit.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/osSiInit.c:osSiInit (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osSiInit(void) {
    __osSiInitialized = 1;
    osCreateMesgQueue(&__osSiMesg, &__osSiMesgQueue, 1);
    osJamMesg(&__osSiMesg, NULL, 0);
}

/* PROMOTED 2026-10-01 — __osSiGetAccess
 * Source:   cloud/work/static_C2/__osSiGetAccess.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C2/__osSiGetAccess.c:__osSiGetAccess (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __osSiGetAccess(void) {
    OSMesg dummyMesg;
    if (!__osSiInitialized) {
        osSiInit();
    }
    osRecvMesg(&__osSiMesg, &dummyMesg, OS_MESG_BLOCK);
}

/* PROMOTED 2026-10-01 — __osSiRelAccess
 * Source:   cloud/work/static_C2/__osSiRelAccess.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C2/__osSiRelAccess.c:__osSiRelAccess (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __osSiRelAccess(void) {
    osJamMesg(&__osSiMesg, NULL, OS_MESG_NOBLOCK);
}

/* PROMOTED 2026-10-01 — osContStartReadData
 * Source:   cloud/work/static_C8/osContStartReadData.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C8/osContStartReadData.c:osContStartReadData (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osContStartReadData(OSMesgQueue* queue, int channel) {
    s32 ret = 0;
    OSMesg dummy;
    OSContStatus data;

    __osSiChannelMask = 250;

    __osContBuildRequest(channel, CONT_CMD_REQUEST_STATUS);

    ret = __osSiRawStartDma(OS_WRITE, &(*(OSPifRam *)__osPfsBuffer));
    osRecvMesg(queue, &dummy, OS_MESG_BLOCK);

    ret = __osSiRawStartDma(OS_READ, &(*(OSPifRam *)__osPfsBuffer));
    osRecvMesg(queue, &dummy, OS_MESG_BLOCK);

    __osContParseResponse(channel, &data);

    if (((data.status & CONT_CARD_ON) != 0) && ((data.status & CONT_CARD_PULL) != 0)) {
        return PFS_ERR_NEW_PACK;
    } else if ((data.errno != 0) || ((data.status & CONT_CARD_ON) == 0)) {
        return PFS_ERR_NOPACK;
    } else if ((data.status & CONT_ADDR_CRC_ER) != 0) {
        return PFS_ERR_CONTRFAIL;
    }

    return ret;
}

/* PROMOTED 2026-10-01 — __osContBuildRequest
 * Source:   cloud/work/static_C7/__osContBuildRequest.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C7/__osContBuildRequest.c:__osContBuildRequest (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __osContBuildRequest(int channel, u8 cmd) {
    u8* ptr;
    __OSContRequesFormatShort requestformat;
    int i;

    __osPfsRequestType = CONT_CMD_END;
    (*(OSPifRam *)__osPfsBuffer).pifstatus = CONT_CMD_READ_BUTTON;

    ptr = (u8*)&(*(OSPifRam *)__osPfsBuffer);

    requestformat.txsize = CONT_CMD_REQUEST_STATUS_TX;
    requestformat.rxsize = CONT_CMD_REQUEST_STATUS_RX;
    requestformat.cmd = cmd;
    requestformat.typeh = CONT_CMD_NOP;
    requestformat.typel = CONT_CMD_NOP;
    requestformat.status = CONT_CMD_NOP;

    for (i = 0; i < channel; i++) {
        *ptr++ = CONT_CMD_REQUEST_STATUS;
    }

    *(__OSContRequesFormatShort*)ptr = requestformat;
    ptr += sizeof(__OSContRequesFormatShort);
    *ptr = CONT_CMD_END;
}

/* PROMOTED 2026-10-01 — __osContParseResponse
 * Source:   cloud/work/static_C7/__osContParseResponse.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C7/__osContParseResponse.c:__osContParseResponse (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __osContParseResponse(int channel, OSContStatus* data) {
    u8* ptr = (u8*)&(*(OSPifRam *)__osPfsBuffer);
    __OSContRequesFormatShort requestformat;
    int i;

    for (i = 0; i < channel; i++) {
        ptr++;
    }

    requestformat = *(__OSContRequesFormatShort*)ptr;
    data->errno = CHNL_ERR(requestformat);

    if (data->errno != 0) {
        return;
    }

    data->type = (requestformat.typel << 8) | (requestformat.typeh);
    data->status = requestformat.status;
}

