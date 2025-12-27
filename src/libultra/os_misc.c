/**
 * @file os_misc.c
 * @brief Miscellaneous low-level OS functions
 *
 * Decompiled from asm/us/D570.s, E340.s, EFC0.s, A800.s, D260.s
 * Contains various utility and hardware access functions
 */

#include "types.h"

/* Hardware register addresses */
#define DPC_STATUS_REG  (*(vu32 *)0xA410000C)

/* DPC Status bits */
#define DPC_STATUS_XBUS_DMEM    0x0001  /* XBUS DMEM DMA */
#define DPC_STATUS_FREEZE       0x0002  /* Freeze RDP */
#define DPC_STATUS_FLUSH        0x0004  /* Flush RDP */
#define DPC_STATUS_START_GCLK   0x0008  /* Start GCLK */
#define DPC_STATUS_TMEM_BUSY    0x0010  /* TMEM busy */
#define DPC_STATUS_PIPE_BUSY    0x0020  /* Pipe busy */
#define DPC_STATUS_CMD_BUSY     0x0040  /* Command busy */
#define DPC_STATUS_CBUF_READY   0x0080  /* Command buffer ready */
#define DPC_STATUS_DMA_BUSY     0x0100  /* DMA busy */
#define DPC_STATUS_END_VALID    0x0200  /* End register valid */
#define DPC_STATUS_START_VALID  0x0400  /* Start register valid */

/* External data */
extern void *__osActiveQueue;  /* Active thread queue pointer */

/**
 * Get CP0 Count register (hardware timer)
 * (func_8000C970 - osGetCount)
 *
 * Reads the MIPS R4300i Count register which increments
 * at half the CPU clock rate (~46.875 MHz on NTSC N64).
 * Used for timing and profiling.
 *
 * @return Current count value
 *
 * Note: This requires inline assembly or compiler intrinsic.
 * The function is 3 instructions: mfc0 $v0, $9; jr $ra; nop
 */
u32 osGetCount(void) {
    u32 count;
    /* GCC inline assembly for mfc0 */
    __asm__ volatile("mfc0 %0, $9" : "=r"(count));
    return count;
}

/**
 * Check if RDP DMA is busy
 * (func_8000D740 - osDpIsBusy / __osDpDeviceBusy)
 *
 * Checks if the RDP's DMA is currently transferring data.
 *
 * @return 1 if busy, 0 if not busy
 */
s32 osDpIsBusy(void) {
    u32 status;

    status = DPC_STATUS_REG;

    /* Check DMA_BUSY bit (0x100) */
    if (status & DPC_STATUS_DMA_BUSY) {
        return 1;
    }
    return 0;
}

/* sqrtf is implemented in src/libm/math.c */

/* External function for TLB lookup */
extern u32 __osTLBLookup(void *addr);

/**
 * Convert virtual address to physical
 * (func_8000D5C0 - osVirtualToPhysical)
 *
 * Converts a virtual address to a physical address by
 * checking which memory segment it's in:
 * - KSEG0 (0x80000000-0x9FFFFFFF): Cached, direct-mapped
 * - KSEG1 (0xA0000000-0xBFFFFFFF): Uncached, direct-mapped
 * - Other: Use TLB lookup
 *
 * @param addr Virtual address
 * @return Physical address (29-bit)
 */
u32 osVirtualToPhysical(void *addr) {
    u32 vaddr = (u32)addr;

    /* KSEG0: 0x80000000 - 0x9FFFFFFF (cached) */
    if (vaddr >= 0x80000000 && vaddr < 0xA0000000) {
        /* Mask off upper bits to get 29-bit physical address */
        return vaddr & 0x1FFFFFFF;
    }

    /* KSEG1: 0xA0000000 - 0xBFFFFFFF (uncached) */
    if (vaddr >= 0xA0000000 && vaddr < 0xC0000000) {
        /* Mask off upper bits to get 29-bit physical address */
        return vaddr & 0x1FFFFFFF;
    }

    /* KUSEG or KSSEG: Use TLB lookup */
    return __osTLBLookup(addr);
}

/**
 * Get active queue
 * (func_8000C660 - osGetActiveQueue)
 *
 * Returns the current active thread/event queue.
 *
 * @return Pointer to active queue
 */
void *osGetActiveQueue(void) {
    return __osActiveQueue;
}

/**
 * Convert physical address to virtual (KSEG0)
 * (func_80009C00 - osPhysicalToVirtual)
 *
 * Converts a 29-bit physical address to a cached KSEG0
 * virtual address by ORing with 0x80000000.
 *
 * @param physAddr Physical address (29-bit)
 * @return Virtual address in KSEG0 (cached)
 */
void *osPhysicalToVirtual(u32 physAddr) {
    return (void *)(physAddr | 0x80000000);
}
