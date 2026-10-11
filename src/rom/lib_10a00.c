/* GENERATED ROM-aligned TU — segment 0x10a00 (rom/lib_10a00)
 * layout map ffd720a3499ccb577495d230497991b7a4ad65ff4dc1e030f3c4a7c041f14922; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — __osEPiRawWriteIo
 * Source:   cloud/work/frontier/w16g/sources/__osEPiRawWriteIo.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__osEPiRawWriteIo.c:__osEPiRawWriteIo (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#define IO_READ(addr) (*(volatile u32 *)(addr))
#define IO_WRITE(addr,value) (*(volatile u32 *)(addr)=(value))
#define PHYS_TO_K1(x) ((u32)(x)|0xA0000000)
#define PI_DOMAIN1 0
#define PI_STATUS_REG 0xA4600010
#define PI_STATUS_DMA_BUSY 1
#define PI_STATUS_IO_BUSY 2
#define PI_BSD_DOM1_LAT_REG 0xA4600014
#define PI_BSD_DOM1_PWD_REG 0xA4600018
#define PI_BSD_DOM1_PGS_REG 0xA460001C
#define PI_BSD_DOM1_RLS_REG 0xA4600020
#define PI_BSD_DOM2_LAT_REG 0xA4600024
#define PI_BSD_DOM2_PWD_REG 0xA4600028
#define PI_BSD_DOM2_PGS_REG 0xA460002C
#define PI_BSD_DOM2_RLS_REG 0xA4600030
#define WAIT_ON_IOBUSY(stat) { stat = IO_READ(PI_STATUS_REG); while (stat & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY)) stat = IO_READ(PI_STATUS_REG); } (void)0
#define UPDATE_REG(pihandle, reg, var) if (cHandle->var != pihandle->var) IO_WRITE(reg, pihandle->var)
#define EPI_SYNC(pihandle, stat, domain) WAIT_ON_IOBUSY(stat); domain = pihandle->domain; if (__osPiDevList[domain]->type != pihandle->type) { OSPiHandle *cHandle = __osPiDevList[domain]; if (domain == PI_DOMAIN1) { UPDATE_REG(pihandle, PI_BSD_DOM1_LAT_REG, latency); UPDATE_REG(pihandle, PI_BSD_DOM1_PGS_REG, pageSize); UPDATE_REG(pihandle, PI_BSD_DOM1_RLS_REG, relDuration); UPDATE_REG(pihandle, PI_BSD_DOM1_PWD_REG, pulse); } else { UPDATE_REG(pihandle, PI_BSD_DOM2_LAT_REG, latency); UPDATE_REG(pihandle, PI_BSD_DOM2_PGS_REG, pageSize); UPDATE_REG(pihandle, PI_BSD_DOM2_RLS_REG, relDuration); UPDATE_REG(pihandle, PI_BSD_DOM2_PWD_REG, pulse); } cHandle->type = pihandle->type; cHandle->latency = pihandle->latency; cHandle->pageSize = pihandle->pageSize; cHandle->relDuration = pihandle->relDuration; cHandle->pulse = pihandle->pulse; } (void)0
extern OSPiHandle *__osPiDevList[2];
s32 __osEPiRawWriteIo(OSPiHandle* pihandle, u32 devAddr, u32 data) {
    register u32 stat;
    register u32 domain;

    EPI_SYNC(pihandle, stat, domain);
    IO_WRITE(PHYS_TO_K1(pihandle->baseAddress | devAddr), data);

    return 0;
}

