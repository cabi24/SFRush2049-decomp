/*
 * Function: osDpIsBusy
 * Address:  0x8000D740
 * Category: libultra/dp
 * Status:   WIP
 *
 * Check if RDP (Reality Display Processor) is busy
 * Returns 1 if busy, 0 if idle
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* DPC (Display Processor Command) Status Register */
#define DPC_STATUS_REG (*(volatile u32 *)0xA410000C)

/* Status bit for command busy */
#define DPC_STATUS_CMDBSY 0x100

s32 osDpIsBusy(void) {
    if (DPC_STATUS_REG & DPC_STATUS_CMDBSY) {
        return 1;
    }
    return 0;
}
