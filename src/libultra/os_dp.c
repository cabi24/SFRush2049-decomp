/**
 * @file os_dp.c
 * @brief Display Processor (RDP) control functions
 *
 * Decompiled from asm/us/8700.s
 * Contains functions for controlling the N64's RDP
 */

#include "types.h"

/* RDP Status Register bits */
#define DPC_STATUS_XBUS_DMEM_DMA    0x0001
#define DPC_STATUS_FREEZE           0x0002
#define DPC_STATUS_FLUSH            0x0004
#define DPC_STATUS_START_GCLK       0x0008
#define DPC_STATUS_TMEM_BUSY        0x0010
#define DPC_STATUS_PIPE_BUSY        0x0020
#define DPC_STATUS_CMD_BUSY         0x0040
#define DPC_STATUS_CBUF_READY       0x0080
#define DPC_STATUS_DMA_BUSY         0x0100
#define DPC_STATUS_END_VALID        0x0200
#define DPC_STATUS_START_VALID      0x0400

/* Hardware register addresses */
#define DPC_START_REG   (*(vu32 *)0xA4100000)
#define DPC_END_REG     (*(vu32 *)0xA4100004)
#define DPC_STATUS_REG  (*(vu32 *)0xA410000C)

/* External functions */
extern u32 osVirtualToPhysical(void *addr);  /* Convert virtual to physical */
extern void __osSpSetStatus(u32 status);     /* SP status register write */

/* Forward declaration */
s32 __osDpDeviceBusy(void);

/**
 * Set RDP display list buffer
 * (func_80007B00 - osDpSetNextBuffer)
 *
 * Submits a display list to the RDP for processing.
 * Waits for any previous operations to complete first.
 *
 * @param cmdList Pointer to display list buffer
 * @param arg1 Unused
 * @param arg2 Unused
 * @param size Size of display list in bytes
 * @return 0 on success, -1 if DP is busy
 */
s32 osDpSetNextBuffer(void *cmdList, s32 arg1, s32 arg2, s32 size) {
    u32 start_addr;
    u32 end_addr;

    (void)arg1;  /* Unused */
    (void)arg2;  /* Unused */

    /* Check if DP is available */
    if (__osDpDeviceBusy() != 0) {
        return -1;
    }

    /* Clear xbus/DMA path (write 1 to bit 0) */
    DPC_STATUS_REG = 1;

    /* Wait for DMA to complete */
    while (DPC_STATUS_REG & DPC_STATUS_XBUS_DMEM_DMA) {
        /* Spin */
    }

    /* Convert virtual address to physical */
    start_addr = osVirtualToPhysical(cmdList);

    /* Set start register */
    DPC_START_REG = start_addr;

    /* Get physical address again (may have been modified) */
    start_addr = osVirtualToPhysical(cmdList);

    /* Set end register */
    end_addr = start_addr + size;
    DPC_END_REG = end_addr;

    return 0;
}

/**
 * Wait for RDP to be ready
 * (func_80007B80)
 *
 * Delays for a fixed number of cycles before checking DP status.
 */
void osDpWait(void) {
    __osSpSetStatus(0x400);  /* Delay ~1024 cycles */
}

/* Additional DPC counter registers */
#define DPC_CLOCK_REG   (*(vu32 *)0xA4100010)
#define DPC_BUFBUSY_REG (*(vu32 *)0xA4100014)
#define DPC_PIPEBUSY_REG (*(vu32 *)0xA4100018)
#define DPC_TMEM_REG    (*(vu32 *)0xA410001C)

/**
 * Get RDP performance counters
 * (func_800099B0 - osDpGetCounters)
 *
 * Reads the DPC performance counter registers and stores
 * them in the provided array.
 *
 * @param counters Array of 4 u32s to receive counter values:
 *                 [0] = DPC_CLOCK_REG (clock cycles)
 *                 [1] = DPC_BUFBUSY_REG (buffer busy cycles)
 *                 [2] = DPC_PIPEBUSY_REG (pipe busy cycles)
 *                 [3] = DPC_TMEM_REG (TMEM busy cycles)
 */
/* MATCHED: func_800099B0 */
void osDpGetCounters(u32 *counters) {
    counters[0] = DPC_CLOCK_REG;
    counters += 3;
    counters[-2] = DPC_BUFBUSY_REG;
    counters[-1] = DPC_PIPEBUSY_REG;
    counters[0] = DPC_TMEM_REG;
}

/**
 * Check if RDP DMA is busy
 * (func_8000D740 - __osDpDeviceBusy)
 *
 * Checks the DPC_STATUS_REG for the DMA_BUSY bit.
 *
 * @return 1 if RDP DMA is busy, 0 if not
 */
s32 __osDpDeviceBusy(void) {
    u32 status;

    status = DPC_STATUS_REG;

    /* Check DMA_BUSY bit (bit 8) */
    if (status & DPC_STATUS_DMA_BUSY) {
        return 1;
    }
    return 0;
}
