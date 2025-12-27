/**
 * @file os_phys.c
 * @brief Virtual to physical address conversion
 *
 * Decompiled from asm/us/E1C0.s
 */

#include "types.h"

/* External TLB lookup function */
extern u32 func_8000FCB0(void *addr);

/**
 * Convert virtual address to physical address
 * (func_8000D5C0 - osVirtualToPhysical)
 *
 * Converts a virtual address to its physical equivalent.
 * Handles KSEG0 (0x80000000-0x9FFFFFFF) and KSEG1 (0xA0000000-0xBFFFFFFF)
 * directly, falls back to TLB lookup for other addresses.
 *
 * @param addr Virtual address to convert
 * @return Physical address, or result from TLB lookup
 */
u32 osVirtualToPhysical(void *addr) {
    u32 vaddr = (u32)addr;

    /* Check KSEG0 range: 0x80000000 - 0x9FFFFFFF */
    if (vaddr >= 0x80000000 && vaddr < 0xA0000000) {
        return vaddr & 0x1FFFFFFF;
    }

    /* Check KSEG1 range: 0xA0000000 - 0xBFFFFFFF */
    if (vaddr >= 0xA0000000 && vaddr < 0xC0000000) {
        return vaddr & 0x1FFFFFFF;
    }

    /* Other addresses need TLB lookup */
    return func_8000FCB0(addr);
}
