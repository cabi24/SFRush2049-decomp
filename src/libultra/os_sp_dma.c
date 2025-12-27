/**
 * @file os_sp_dma.c
 * @brief SP (Signal Processor) DMA functions
 *
 * Decompiled from asm/us/E280.s
 *
 * The SP (RSP) is the N64's vector coprocessor used for
 * graphics and audio processing. These functions handle
 * DMA transfers between RDRAM and SP memory (IMEM/DMEM).
 */

#include "types.h"

/* SP DMA registers */
#define SP_MEM_ADDR_REG     (*(vu32 *)0xA4040000)
#define SP_DRAM_ADDR_REG    (*(vu32 *)0xA4040004)
#define SP_RD_LEN_REG       (*(vu32 *)0xA4040008)
#define SP_WR_LEN_REG       (*(vu32 *)0xA404000C)

/* SP direction constants */
#define SP_DMA_WRITE        0   /* RDRAM -> SP */
#define SP_DMA_READ         1   /* SP -> RDRAM */

/* External functions */
extern s32 __osSpDeviceBusy(void);  /* func_8000D710 */
extern u32 osVirtualToPhysical(void *addr);

/**
 * Start SP DMA transfer
 * (func_8000D680 - osSpRawStartDma)
 *
 * Initiates a DMA transfer between RDRAM and SP memory (IMEM/DMEM).
 * Used by the RSP task system for loading microcode and data.
 *
 * @param direction SP_DMA_WRITE (RDRAM -> SP) or SP_DMA_READ (SP -> RDRAM)
 * @param spAddr SP memory address (IMEM: 0x1000-0x1FFF, DMEM: 0x0000-0x0FFF)
 * @param dramAddr RDRAM address (virtual)
 * @param length Transfer length in bytes (minus 1 is written to register)
 * @return 0 on success, -1 if SP is busy
 */
s32 osSpRawStartDma(s32 direction, u32 spAddr, void *dramAddr, u32 length) {
    /* Check if SP is busy */
    if (__osSpDeviceBusy() != 0) {
        return -1;
    }

    /* Set SP memory address */
    SP_MEM_ADDR_REG = spAddr;

    /* Set RDRAM address (convert to physical) */
    SP_DRAM_ADDR_REG = osVirtualToPhysical(dramAddr);

    /* Start transfer based on direction */
    if (direction == SP_DMA_WRITE) {
        /* Write to SP (RDRAM -> SP) */
        SP_WR_LEN_REG = length - 1;
    } else {
        /* Read from SP (SP -> RDRAM) */
        SP_RD_LEN_REG = length - 1;
    }

    return 0;
}
