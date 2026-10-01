/* GENERATED ROM-aligned TU — segment 0x8700 (rom/lib_8700)
 * layout map b2a06a4be17f56ba1988d227bd563ddac1bbf960bb1278c82005b0becd2afa28; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osDpSetNextBuffer
 * Source:   cloud/work/static_C3/osDpSetNextBuffer.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C3/osDpSetNextBuffer.c:osDpSetNextBuffer (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osDpSetNextBuffer(void* bufPtr, u64 size) {
    register u32 stat;

#ifdef _DEBUG
    if ((u32)bufPtr & 0x7) {
        __osError(ERR_OSDPSETNEXTBUFFER_ADDR, 1, bufPtr);
        return -1;
    }
    if (size & 0x7) {
        __osError(ERR_OSDPSETNEXTBUFFER_SIZE, 1, size);
        return -1;
    }
#endif

    if (osDpIsBusy()) {
        return -1;
    }

    (*(vu32 *)0xA410000CU) = 1;

    while (TRUE) {
        stat = (*(vu32 *)0xA410000CU);
        if ((stat & 1) == 0) {
            break;
        }
    }

    (*(vu32 *)0xA4100000U) = osVirtualToPhysical(bufPtr);
    (*(vu32 *)0xA4100004U) = osVirtualToPhysical(bufPtr) + size;
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8700/osDpWait.s")
