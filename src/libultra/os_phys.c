/**
 * @file os_phys.c
 * @brief Virtual to physical address conversion
 *
 * Decompiled from asm/us/E1C0.s
 *
 * The N64 MIPS CPU uses a virtual memory model. KSEG0 and KSEG1
 * are direct-mapped segments that can be converted without TLB.
 */

#include "types.h"

/* External TLB lookup function */
extern u32 __osTlbLookup(void *addr);  /* func_8000FCB0 */

/**
 * Convert virtual address to physical address
 * (func_8000D5C0 - osVirtualToPhysical)
 *
 * Converts a virtual address to its physical equivalent.
 *
 * Memory segments:
 * - KSEG0 (0x80000000-0x9FFFFFFF): Cached, direct-mapped
 * - KSEG1 (0xA0000000-0xBFFFFFFF): Uncached, direct-mapped
 * - KUSEG/KSEG2/KSEG3: Require TLB translation
 *
 * @param addr Virtual address to convert
 * @return Physical address (29 bits)
 */
u32 osVirtualToPhysical(void *addr) {
    u32 vaddr = (u32)addr;

    /* KSEG0 range: 0x80000000 - 0x9FFFFFFF (cached) */
    if (vaddr >= 0x80000000 && vaddr < 0xA0000000) {
        return vaddr & 0x1FFFFFFF;
    }

    /* KSEG1 range: 0xA0000000 - 0xBFFFFFFF (uncached) */
    if (vaddr >= 0xA0000000 && vaddr < 0xC0000000) {
        return vaddr & 0x1FFFFFFF;
    }

    /* Other addresses (KUSEG, KSEG2, KSEG3) need TLB lookup */
    return __osTlbLookup(addr);
}

/**
 * Convert physical address to virtual address
 * (osPhysicalToVirtual)
 *
 * Converts a physical address to a cached virtual address in KSEG0.
 *
 * @param physAddr Physical address
 * @return Virtual address in KSEG0
 */
void *osPhysicalToVirtual(u32 physAddr) {
    return (void *)(physAddr | 0x80000000);
}
