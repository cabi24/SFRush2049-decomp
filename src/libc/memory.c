/**
 * @file memory.c
 * @brief Memory copy functions
 *
 * Decompiled from asm/us/DEB0.s
 * Contains optimized memory copy functions that handle overlapping regions
 */

#include "types.h"

/**
 * Copy memory with overlap handling (bcopy/memmove)
 * (0x8000D2B0 - bcopy)
 *
 * Copies n bytes from src to dst, handling overlapping regions correctly.
 * Uses optimized 32-byte and 16-byte block copies when properly aligned.
 *
 * Features:
 * - Handles forward and backward copy for overlapping regions
 * - Word-aligned fast path with 32-byte unrolled loops
 * - Byte-by-byte fallback for unaligned or small copies
 * - Alignment fixup for partially aligned addresses
 *
 * @param src Source address
 * @param dst Destination address
 * @param n Number of bytes to copy
 * @return Original dst pointer
 */
void *bcopy(const void *src, void *dst, u32 n) {
    const u8 *s = (const u8 *)src;
    u8 *d = (u8 *)dst;
    void *ret = dst;

    /* Handle trivial cases */
    if (n == 0) {
        return ret;
    }
    if (s == d) {
        return ret;
    }

    /* Check for overlap requiring backward copy */
    if (s < d && d < s + n) {
        /* Backward copy - start from end */
        s += n;
        d += n;

        /* Handle small copies byte-by-byte */
        if (n < 16) {
            goto backward_byte;
        }

        /* Check alignment */
        if (((u32)s & 3) != ((u32)d & 3)) {
            goto backward_byte;
        }

        /* Align to word boundary */
        switch ((u32)s & 3) {
        case 1:
            *--d = *--s;
            n--;
            /* fall through */
        case 2:
            d -= 2;
            s -= 2;
            *(u16 *)d = *(const u16 *)s;
            n -= 2;
            break;
        case 3:
            *--d = *--s;
            d -= 2;
            s -= 2;
            *(u16 *)d = *(const u16 *)s;
            n -= 3;
            break;
        }

        /* 32-byte backward block copy */
        while (n >= 32) {
            s -= 32;
            d -= 32;
            ((u32 *)d)[7] = ((const u32 *)s)[7];
            ((u32 *)d)[6] = ((const u32 *)s)[6];
            ((u32 *)d)[5] = ((const u32 *)s)[5];
            ((u32 *)d)[4] = ((const u32 *)s)[4];
            ((u32 *)d)[3] = ((const u32 *)s)[3];
            ((u32 *)d)[2] = ((const u32 *)s)[2];
            ((u32 *)d)[1] = ((const u32 *)s)[1];
            ((u32 *)d)[0] = ((const u32 *)s)[0];
            n -= 32;
        }

        /* 16-byte backward block copy */
        while (n >= 16) {
            s -= 16;
            d -= 16;
            ((u32 *)d)[3] = ((const u32 *)s)[3];
            ((u32 *)d)[2] = ((const u32 *)s)[2];
            ((u32 *)d)[1] = ((const u32 *)s)[1];
            ((u32 *)d)[0] = ((const u32 *)s)[0];
            n -= 16;
        }

        /* 4-byte backward copy */
        while (n >= 4) {
            s -= 4;
            d -= 4;
            *(u32 *)d = *(const u32 *)s;
            n -= 4;
        }

backward_byte:
        /* Byte-by-byte backward copy */
        while (n > 0) {
            *--d = *--s;
            n--;
        }

        return ret;
    }

    /* Forward copy */
    if (n < 16) {
        goto forward_byte;
    }

    /* Check alignment */
    if (((u32)s & 3) != ((u32)d & 3)) {
        goto forward_byte;
    }

    /* Align to word boundary */
    switch ((u32)s & 3) {
    case 1:
        *d++ = *s++;
        n--;
        /* fall through */
    case 2:
        *(u16 *)d = *(const u16 *)s;
        s += 2;
        d += 2;
        n -= 2;
        break;
    case 3:
        *d++ = *s++;
        *(u16 *)d = *(const u16 *)s;
        s += 2;
        d += 2;
        n -= 3;
        break;
    }

    /* 32-byte forward block copy */
    while (n >= 32) {
        ((u32 *)d)[0] = ((const u32 *)s)[0];
        ((u32 *)d)[1] = ((const u32 *)s)[1];
        ((u32 *)d)[2] = ((const u32 *)s)[2];
        ((u32 *)d)[3] = ((const u32 *)s)[3];
        ((u32 *)d)[4] = ((const u32 *)s)[4];
        ((u32 *)d)[5] = ((const u32 *)s)[5];
        ((u32 *)d)[6] = ((const u32 *)s)[6];
        ((u32 *)d)[7] = ((const u32 *)s)[7];
        s += 32;
        d += 32;
        n -= 32;
    }

    /* 16-byte forward block copy */
    while (n >= 16) {
        ((u32 *)d)[0] = ((const u32 *)s)[0];
        ((u32 *)d)[1] = ((const u32 *)s)[1];
        ((u32 *)d)[2] = ((const u32 *)s)[2];
        ((u32 *)d)[3] = ((const u32 *)s)[3];
        s += 16;
        d += 16;
        n -= 16;
    }

    /* 4-byte forward copy */
    while (n >= 4) {
        *(u32 *)d = *(const u32 *)s;
        s += 4;
        d += 4;
        n -= 4;
    }

forward_byte:
    /* Byte-by-byte forward copy */
    while (n > 0) {
        *d++ = *s++;
        n--;
    }

    return ret;
}
