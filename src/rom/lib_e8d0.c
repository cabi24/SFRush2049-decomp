/* GENERATED ROM-aligned TU — segment 0xe8d0 (rom/lib_e8d0)
 * layout map 14cdd3d6833ac94d2f4f0846046858122b418220ea8efb0d728c182b631eb45a; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osPiStartDma
 * Source:   cloud/work/static_C9/osPiStartDma.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C9/osPiStartDma.c:osPiStartDma (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPiStartDma(s32 direction, u32 devAddr, void* dramAddr, u32 size) {
    register u32 stat;


    while ((stat = *(volatile u32*)0xA4600010) & 3);

    *(volatile u32*)0xA4600000 = osVirtualToPhysical(dramAddr);
    *(volatile u32*)0xA4600004 = (((u32)osRomBase | devAddr) & 0x1FFFFFFF);

    switch (direction) {
        case OS_READ:
            *(volatile u32*)0xA460000C = size - 1;
            break;
        case OS_WRITE:
            *(volatile u32*)0xA4600008 = size - 1;
            break;
        default:
            return -1;
    }
    return 0;
}

