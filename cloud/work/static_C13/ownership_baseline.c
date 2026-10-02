/* GENERATED ROM-aligned TU — segment 0xe9a0 (rom/lib_e9a0)
 * layout map 3babb96e2ff8b36483524b7acb5405673e5c98d295075fdfac8ed2bdda2fb775; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* ROM_OWNED_RODATA_BEGIN osSpTaskLoad_full */
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_e9a0/osSpTaskLoad_full.table.s")
/* ROM_OWNED_RODATA_END osSpTaskLoad_full */


/* Canonical EPi hardware protocol; existing IO_READ/IO_WRITE macros suffice. */
#define PI_STATUS_IO_BUSY 2
#define PI_STATUS_DMA_BUSY 1
#define PI_DOMAIN1 0
#define K1_TO_PHYS(x) ((u32)(x)&0x1FFFFFFF)
#define PI_STATUS_REG 0xA4600010
#define PI_DRAM_ADDR_REG 0xA4600000
#define PI_CART_ADDR_REG 0xA4600004
#define PI_WR_LEN_REG 0xA460000C
#define PI_RD_LEN_REG 0xA4600008
#define PI_BSD_DOM1_LAT_REG 0xA4600014
#define PI_BSD_DOM1_PWD_REG 0xA4600018
#define PI_BSD_DOM1_PGS_REG 0xA460001C
#define PI_BSD_DOM1_RLS_REG 0xA4600020
#define PI_BSD_DOM2_LAT_REG 0xA4600024
#define PI_BSD_DOM2_PWD_REG 0xA4600028
#define PI_BSD_DOM2_PGS_REG 0xA460002C
#define PI_BSD_DOM2_RLS_REG 0xA4600030
#define WAIT_ON_IOBUSY(stat)                                                                \
    {                                                                                       \
        stat = IO_READ(PI_STATUS_REG);                                                      \
        while (stat & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY))                             \
            stat = IO_READ(PI_STATUS_REG);                                                  \
    } (void)0

#define UPDATE_REG(pihandle, reg, var) \
    if (cHandle->var != pihandle->var) \
        IO_WRITE(reg, pihandle->var)


#define EPI_SYNC(pihandle, stat, domain)                             \
                                                                     \
    WAIT_ON_IOBUSY(stat);                                            \
                                                                     \
    domain = pihandle->domain;                                       \
    if (__osPiDevList[domain]->type != pihandle->type)           \
    {                                                                \
        OSPiHandle *cHandle = __osPiDevList[domain];             \
        if (domain == PI_DOMAIN1)                                    \
        {                                                            \
            UPDATE_REG(pihandle, PI_BSD_DOM1_LAT_REG, latency);      \
            UPDATE_REG(pihandle, PI_BSD_DOM1_PGS_REG, pageSize);     \
            UPDATE_REG(pihandle, PI_BSD_DOM1_RLS_REG, relDuration);  \
            UPDATE_REG(pihandle, PI_BSD_DOM1_PWD_REG, pulse);        \
        }                                                            \
        else                                                         \
        {                                                            \
            UPDATE_REG(pihandle, PI_BSD_DOM2_LAT_REG, latency);      \
            UPDATE_REG(pihandle, PI_BSD_DOM2_PGS_REG, pageSize);     \
            UPDATE_REG(pihandle, PI_BSD_DOM2_RLS_REG, relDuration);  \
            UPDATE_REG(pihandle, PI_BSD_DOM2_PWD_REG, pulse);        \
        }                                                            \
        cHandle->type = pihandle->type;                              \
        cHandle->latency = pihandle->latency;                        \
        cHandle->pageSize = pihandle->pageSize;                      \
        cHandle->relDuration = pihandle->relDuration;                \
        cHandle->pulse = pihandle->pulse;                            \
    }(void)0

/* PROMOTED 2026-10-01 — osPiSetDeviceTiming
 * Source:   cloud/work/static_C12/osPiSetDeviceTiming.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C12/osPiSetDeviceTiming.c:osPiSetDeviceTiming (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPiSetDeviceTiming(OSPiHandle* pihandle, s32 direction, u32 devAddr, void* dramAddr, u32 size) {
    u32 stat;
    u32 domain;


    EPI_SYNC(pihandle, stat, domain);
    IO_WRITE(PI_DRAM_ADDR_REG, osVirtualToPhysical(dramAddr));
    IO_WRITE(PI_CART_ADDR_REG, K1_TO_PHYS(pihandle->baseAddress | devAddr));

    switch (direction) {
        case OS_READ:
            IO_WRITE(PI_WR_LEN_REG, size - 1);
            break;
        case OS_WRITE:
            IO_WRITE(PI_RD_LEN_REG, size - 1);
            break;
        default:
            return -1;
    }
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_e9a0/osSpTaskLoad_full.s")
/* PROMOTED 2026-07-15 — __osInsertTimer
 * Source:   src/rom_auto/__osInsertTimer.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/rom_auto/__osInsertTimer.c:__osInsertTimer (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osInsertTimer(void) {
    if (__osPiMgrState.flag == 0) {
        return 0;
    }
    return __osPiMgrState.unk8;
}
