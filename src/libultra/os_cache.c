/**
 * @file os_cache.c
 * @brief Cache management functions
 *
 * Decompiled from asm/us/8410.s, 88A0.s, 90E0.s, 9190.s
 * Contains cache invalidation, writeback, and memory zeroing functions
 *
 * Note: The cache instruction functions are written in assembly
 * and use the MIPS R4300i cache instruction directly.
 */

#include "types.h"

/* Cache line size is 16 bytes on R4300i */
#define CACHE_LINE_SIZE 16

/* I-cache and D-cache are both 8KB */
#define ICACHE_SIZE 0x2000
#define DCACHE_SIZE 0x2000

/**
 * Writeback data cache region
 * (0x80007CA0 - osWritebackDCache)
 *
 * Writes back (flushes) dirty cache lines to memory. If the size
 * is larger than the cache (8KB), writes back the entire cache.
 *
 * Uses cache instructions:
 * - 0x19 (Hit_Writeback_D) for partial writeback
 * - 0x01 (Index_Writeback_Inv_D) for full cache writeback
 *
 * @param addr Start address to writeback
 * @param size Number of bytes to writeback
 *
 * This is a handwritten assembly function.
 */
/* Assembly stub - actual implementation in asm */
#if 0
void osWritebackDCache(void *addr, u32 size) {
    u8 *start = (u8 *)addr;
    u8 *end;

    if (size <= 0) {
        return;
    }

    /* If size >= 8KB, writeback entire cache */
    if (size >= DCACHE_SIZE) {
        u8 *caddr = (u8 *)0x80000000;
        u8 *cend = caddr + DCACHE_SIZE - CACHE_LINE_SIZE;

        while (caddr < cend) {
            /* cache 0x01, 0(caddr) - Index_Writeback_Inv_D */
            __asm__("cache 0x01, 0(%0)" :: "r"(caddr));
            caddr += CACHE_LINE_SIZE;
        }
        return;
    }

    end = start + size;
    if (start >= end) {
        return;
    }

    /* Align start down to cache line */
    start = (u8 *)((u32)start & ~0xF);
    end -= CACHE_LINE_SIZE;

    /* Writeback cache lines */
    while (start < end) {
        /* cache 0x19, 0(start) - Hit_Writeback_D */
        __asm__("cache 0x19, 0(%0)" :: "r"(start));
        start += CACHE_LINE_SIZE;
    }
}
#endif

/**
 * Invalidate instruction cache
 * (0x80007810 - osInvalICache)
 *
 * Invalidates the entire 8KB instruction cache.
 * Uses cache instruction 0x01 (Index_Invalidate_I).
 *
 * This is a handwritten assembly function.
 */
/* Assembly stub - actual implementation in asm */
#if 0
void osInvalICache(void) {
    u8 *addr = (u8 *)0x80000000;
    u8 *end = addr + ICACHE_SIZE - CACHE_LINE_SIZE;

    while (addr < end) {
        /* cache 0x01, 0(addr) - Index_Invalidate_I */
        __asm__("cache 0x01, 0(%0)" :: "r"(addr));
        addr += CACHE_LINE_SIZE;
    }
}
#endif

/**
 * Invalidate data cache region
 * (0x800084E0 - osInvalDCache)
 *
 * Invalidates a range of the data cache. If the size is larger
 * than the cache (8KB), invalidates the entire cache.
 *
 * Uses cache instructions:
 * - 0x15 (Hit_Writeback_Inv_D) for partial invalidation
 * - 0x11 (Index_Writeback_Inv_D) for line invalidation
 * - 0x01 (Index_Invalidate_D) for full cache invalidation
 *
 * @param addr Start address to invalidate
 * @param size Number of bytes to invalidate
 *
 * This is a handwritten assembly function.
 */
/* Assembly stub - actual implementation in asm */
#if 0
void osInvalDCache(void *addr, u32 size) {
    u8 *start = (u8 *)addr;
    u8 *end;
    u32 offset;

    if (size <= 0) {
        return;
    }

    /* If size >= 8KB, invalidate entire cache */
    if (size >= DCACHE_SIZE) {
        u8 *caddr = (u8 *)0x80000000;
        u8 *cend = caddr + DCACHE_SIZE - CACHE_LINE_SIZE;

        while (caddr < cend) {
            /* cache 0x01, 0(caddr) */
            __asm__("cache 0x01, 0(%0)" :: "r"(caddr));
            caddr += CACHE_LINE_SIZE;
        }
        return;
    }

    end = start + size;
    if (start >= end) {
        return;
    }

    /* Handle unaligned start */
    offset = (u32)start & 0xF;
    if (offset != 0) {
        start -= offset;
        /* cache 0x15, 0(start) - Hit_Writeback_Inv_D */
        __asm__("cache 0x15, 0(%0)" :: "r"(start));
        if (start >= end - CACHE_LINE_SIZE) {
            return;
        }
        start += CACHE_LINE_SIZE;
    }

    end -= CACHE_LINE_SIZE;

    /* Handle unaligned end */
    offset = (u32)end & 0xF;
    if (offset != 0) {
        end -= offset;
        /* cache 0x15, 16(end) */
        __asm__("cache 0x15, 16(%0)" :: "r"(end));
        if (end < start) {
            return;
        }
    }

    /* Invalidate aligned cache lines */
    while (start < end) {
        /* cache 0x11, 0(start) - Index_Writeback_Inv_D */
        __asm__("cache 0x11, 0(%0)" :: "r"(start));
        start += CACHE_LINE_SIZE;
    }
}
#endif

/**
 * Zero-fill memory block
 * (0x80008590 - bzero)
 *
 * Optimized zero-fill that uses 32-byte unrolled loops for
 * large blocks, with special handling for alignment.
 *
 * @param ptr Start address to zero
 * @param size Number of bytes to zero
 */
void bzero(void *ptr, u32 size) {
    u8 *dst = (u8 *)ptr;
    u32 align;
    u32 block_size;
    u8 *end;

    /* For small buffers (<12 bytes), just do byte-by-byte */
    if (size < 12) {
        goto byte_copy;
    }

    /* Align to 4-byte boundary */
    align = (-(u32)dst) & 3;
    if (align != 0) {
        /* Use unaligned word store to handle prefix */
        /* swl zero, 0(dst) */
        *(u32 *)((u32)dst & ~3) &= ~(0xFFFFFFFF << ((4 - align) * 8));
        dst += align;
        size -= align;
    }

    /* Process 32-byte blocks */
    block_size = size & ~0x1F;
    if (block_size != 0) {
        size -= block_size;
        end = dst + block_size;

        while (dst != end) {
            ((u32 *)dst)[0] = 0;
            ((u32 *)dst)[1] = 0;
            ((u32 *)dst)[2] = 0;
            ((u32 *)dst)[3] = 0;
            ((u32 *)dst)[4] = 0;
            ((u32 *)dst)[5] = 0;
            ((u32 *)dst)[6] = 0;
            ((u32 *)dst)[7] = 0;
            dst += 32;
        }
    }

    /* Process 4-byte blocks */
    block_size = size & ~3;
    if (block_size != 0) {
        size -= block_size;
        end = dst + block_size;

        while (dst != end) {
            *(u32 *)dst = 0;
            dst += 4;
        }
    }

byte_copy:
    /* Process remaining bytes */
    if (size > 0) {
        end = dst + size;
        while (dst != end) {
            *dst++ = 0;
        }
    }
}
