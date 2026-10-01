/* GENERATED ROM-aligned TU — segment 0xa330 (rom/lib_a330)
 * layout map a35c7b19bb677a9029630ff9414a70c9be081a07d21c4a5bb277be656d9b0153; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osContStartQuery
 * Source:   cloud/work/static_C3/osContStartQuery.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C3/osContStartQuery.c:osContStartQuery (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osContStartQuery(OSMesgQueue* mq) {
    s32 ret = 0;

    __osSiGetAccess();

    if (__osPfsRequestType != CONT_CMD_REQUEST_STATUS) {
        __osContRamReset(CONT_CMD_REQUEST_STATUS);
        ret = __osSiRawStartDma(OS_WRITE, __osSiDmaBuffer.ramarray);
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);
    }

    ret = __osSiRawStartDma(OS_READ, __osSiDmaBuffer.ramarray);
    __osPfsRequestType = CONT_CMD_REQUEST_STATUS;
    __osSiRelAccess();
    return ret;
}

/* PROMOTED 2026-10-01 — osContGetQuery
 * Source:   cloud/work/static_C2/osContGetQuery.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C2/osContGetQuery.c:osContGetQuery (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osContGetQuery(OSContStatus* data) {
    u8 pattern;
    __osContGetStatus(&pattern, data);
}

/* PROMOTED 2026-10-01 — osContStartReadData2
 * Source:   cloud/work/static_C/osContStartReadData2.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/osContStartReadData2.c:osContStartReadData2 (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osContStartReadData2(OSMesgQueue *arg0) {
    s32 temp_v0;
    s32 sp1C;

    __osSiGetAccess();
    if (__osPfsRequestType != 1) {
        __osPackReadData();
        __osSiRawStartDma(1, &__osSiDmaBuffer);
        osRecvMesg(arg0, NULL, 1);
    }
    temp_v0 = __osSiRawStartDma(0, &__osSiDmaBuffer);
    sp1C = temp_v0;
    __osPfsRequestType = 1;
    __osSiRelAccess();
    return temp_v0;
}

/* PROMOTED 2026-10-01 — osContGetReadData
 * Source:   cloud/work/static_C2/osContGetReadData.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C2/osContGetReadData.c:osContGetReadData (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osContGetReadData(OSContPad* data) {
    u8* ptr = (u8*)__osSiDmaBuffer.ramarray;
    __OSContReadFormat readformat;
    int i;

    for (i = 0; i < __osPfsRequestType2; i++, ptr += sizeof(__OSContReadFormat), data++) {
        readformat = *(__OSContReadFormat*)ptr;
        data->errno = CHNL_ERR(readformat);

        if (data->errno != 0) {
            continue;
        }

        data->button = readformat.button;
        data->stick_x = readformat.stick_x;
        data->stick_y = readformat.stick_y;
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_a330/__osPackReadData.s")
