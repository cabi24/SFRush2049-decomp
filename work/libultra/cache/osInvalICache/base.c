/*
 * Function: osInvalICache
 * Address:  0x80007810
 * Category: libultra/cache
 * Status:   WIP
 *
 * Invalidate entire I-cache (8KB)
 * Loops through cache lines using cache instruction
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Cache line size */
#define ICACHE_LINESIZE 16
#define ICACHE_SIZE     0x2000

void osInvalICache(void) {
    u32 addr = 0x80000000;
    u32 end = addr + ICACHE_SIZE - ICACHE_LINESIZE;

    do {
        __asm__ volatile("cache 0x01, 0(%0)" : : "r"(addr));
        addr += ICACHE_LINESIZE;
    } while (addr < end);
}
