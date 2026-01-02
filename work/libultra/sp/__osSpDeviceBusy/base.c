/*
 * Function: __osSpDeviceBusy
 * Address:  0x8000D710
 * Category: libultra/sp
 * Status:   WIP
 *
 * Check if RSP device is busy
 * Returns 1 if any of bits 0x1C set, 0 otherwise
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* SP Status Register */
#define SP_STATUS_REG (*(volatile u32 *)0xA4040010)

/* Busy status bits: halt, broke, DMA busy */
#define SP_STATUS_BUSY_MASK 0x1C

s32 __osSpDeviceBusy(void) {
    if (SP_STATUS_REG & SP_STATUS_BUSY_MASK) {
        return 1;
    }
    return 0;
}
