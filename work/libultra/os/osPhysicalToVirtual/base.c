/*
 * Function: osPhysicalToVirtual
 * Address:  0x80009C00
 * Category: libultra/os
 * Status:   WIP
 *
 * Convert physical address to KSEG0 virtual address
 * KSEG0 is the cached, unmapped kernel segment at 0x80000000
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* KSEG0 base address */
#define KSEG0_BASE 0x80000000

void *osPhysicalToVirtual(u32 physAddr) {
    return (void *)(physAddr | KSEG0_BASE);
}
