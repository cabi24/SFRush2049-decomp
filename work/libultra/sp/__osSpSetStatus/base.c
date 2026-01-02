/*
 * Function: __osSpSetStatus
 * Address:  0x8000D640
 * Category: libultra/sp
 * Status:   WIP
 *
 * Set RSP status register
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

#define SP_STATUS_REG (*(volatile u32 *)0xA4040010)

/* Write value to SP status register */
void __osSpSetStatus(u32 status) {
    SP_STATUS_REG = status;
}
