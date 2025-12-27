/**
 * @file os_sp_dma.c
 * @brief SP (Signal Processor) DMA functions
 *
 * Decompiled from asm/us/E280.s
 */

#include "types.h"

/* SP DMA registers */
#define SP_MEM_ADDR_REG  (*(vu32 *)0xA4040000)
#define SP_DRAM_ADDR_REG (*(vu32 *)0xA4040004)
#define SP_RD_LEN_REG    (*(vu32 *)0xA4040008)
#define SP_WR_LEN_REG    (*(vu32 *)0xA404000C)

/* External functions */
extern s32 func_8000D710(void);  /* SP busy check */
extern u32 osVirtualToPhysical(void *addr);

/**
 * Start SP DMA transfer
 * (func_8000D680 - osSpRawStartDma)
 *
 * Initiates a DMA transfer between RDRAM and SP memory.
 *
 * @param direction 0 = write to SP (RDRAM -> SP), 1 = read from SP (SP -> RDRAM)
 * @param sp_addr SP memory address
 * @param dram_addr RDRAM address
 * @param length Transfer length in bytes
 * @return 0 on success, -1 if SP is busy
 */
s32 osSpRawStartDma(s32 direction, u32 sp_addr, void *dram_addr, u32 length) {
    /* Check if SP is busy */
    if (func_8000D710() != 0) {
        return -1;
    }

    /* Set SP memory address */
    SP_MEM_ADDR_REG = sp_addr;

    /* Set RDRAM address (physical) */
    SP_DRAM_ADDR_REG = osVirtualToPhysical(dram_addr);

    /* Start transfer */
    if (direction == 0) {
        /* Write to SP (RDRAM -> SP) */
        SP_WR_LEN_REG = length - 1;
    } else {
        /* Read from SP (SP -> RDRAM) */
        SP_RD_LEN_REG = length - 1;
    }

    return 0;
}
