/* GENERATED ROM-aligned TU — segment 0xa330 (rom/lib_a330)
 * layout map a35c7b19bb677a9029630ff9414a70c9be081a07d21c4a5bb277be656d9b0153; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_a330/osContStartQuery.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_a330/osContGetQuery.s")
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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_a330/osContGetReadData.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_a330/__osPackReadData.s")
