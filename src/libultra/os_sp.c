/**
 * @file os_sp.c
 * @brief RSP (Signal Processor) control functions
 *
 * Decompiled from asm/us/E240.s, E250.s, E310.s
 * Contains functions for controlling the N64's RSP
 */

#include "types.h"

/* Hardware register addresses */
#define SP_STATUS_REG   (*(vu32 *)0xA4040010)
#define SP_PC_REG       (*(vu32 *)0xA4080000)

/* SP Status bits */
#define SP_STATUS_HALT          0x0001
#define SP_STATUS_BROKE         0x0002
#define SP_STATUS_DMA_BUSY      0x0004
#define SP_STATUS_DMA_FULL      0x0008
#define SP_STATUS_IO_FULL       0x0010
#define SP_STATUS_SSTEP         0x0020
#define SP_STATUS_INTR_BREAK    0x0040
#define SP_STATUS_SIG0          0x0080
#define SP_STATUS_SIG1          0x0100
#define SP_STATUS_SIG2          0x0200
#define SP_STATUS_SIG3          0x0400
#define SP_STATUS_SIG4          0x0800
#define SP_STATUS_SIG5          0x1000
#define SP_STATUS_SIG6          0x2000
#define SP_STATUS_SIG7          0x4000

/**
 * Set RSP status register
 * (func_8000D640 - __osSpSetStatus)
 *
 * Writes directly to the SP_STATUS_REG to set/clear status bits.
 *
 * @param status Value to write to SP_STATUS_REG
 */
void __osSpSetStatus(u32 status) {
    SP_STATUS_REG = status;
}

/**
 * Set RSP program counter
 * (func_8000D650 - osSpTaskLoad / __osSpSetPc)
 *
 * Sets the RSP program counter if the RSP is halted.
 *
 * @param pc New program counter value
 * @return 0 on success, -1 if RSP not halted
 */
s32 __osSpSetPc(u32 pc) {
    u32 status;

    status = SP_STATUS_REG;

    /* Check if RSP is halted */
    if ((status & SP_STATUS_HALT) == 0) {
        return -1;
    }

    /* Set program counter */
    SP_PC_REG = pc;
    return 0;
}

/**
 * Check if RSP device is busy
 * (func_8000D710 - __osSpDeviceBusy)
 *
 * Checks DMA_BUSY, DMA_FULL, and IO_FULL bits.
 *
 * @return 1 if busy, 0 if not busy
 */
s32 __osSpDeviceBusy(void) {
    u32 status;

    status = SP_STATUS_REG;

    /* Check bits 2-4 (DMA_BUSY, DMA_FULL, IO_FULL) */
    if (status & 0x1C) {
        return 1;
    }
    return 0;
}

/* SP Memory DMA registers */
#define SP_MEM_ADDR_REG   (*(vu32 *)0xA4040000)
#define SP_DRAM_ADDR_REG  (*(vu32 *)0xA4040004)
#define SP_RD_LEN_REG     (*(vu32 *)0xA4040008)
#define SP_WR_LEN_REG     (*(vu32 *)0xA404000C)

/* External functions */
extern u32 osVirtualToPhysical(void *addr);

/**
 * Transfer data between RSP DMEM/IMEM and RDRAM
 * (func_8000D680 - __osSpDma)
 *
 * Performs DMA transfer between RSP memory and main RAM.
 *
 * @param direction 0 = RDRAM->RSP (write to RSP), non-zero = RSP->RDRAM (read from RSP)
 * @param spMemAddr RSP memory address (DMEM/IMEM offset)
 * @param dramAddr RDRAM virtual address
 * @param length Transfer length in bytes
 * @return 0 on success, -1 if RSP busy
 */
s32 __osSpDma(s32 direction, void *spMemAddr, void *dramAddr, u32 length) {
    /* Check if SP is busy */
    if (__osSpDeviceBusy()) {
        return -1;
    }

    /* Set SP memory address */
    SP_MEM_ADDR_REG = (u32)spMemAddr;

    /* Set DRAM address (convert virtual to physical) */
    SP_DRAM_ADDR_REG = osVirtualToPhysical(dramAddr);

    /* Start DMA - direction determines which length register to use */
    if (direction == 0) {
        /* Write to RSP (RDRAM -> DMEM/IMEM) */
        SP_WR_LEN_REG = length - 1;
    } else {
        /* Read from RSP (DMEM/IMEM -> RDRAM) */
        SP_RD_LEN_REG = length - 1;
    }

    return 0;
}
