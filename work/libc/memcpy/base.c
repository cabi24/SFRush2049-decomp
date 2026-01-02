/*
 * Function: memcpy
 * Address:  0x80007C68
 * Category: libc
 * Status:   WIP
 *
 * Copy n bytes from src to dst, return dst.
 * Simple byte-by-byte copy loop.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

void *memcpy(void *dst, const void *src, u32 n) {
    u8 *d = (u8 *)dst;
    const u8 *s = (const u8 *)src;

    while (n != 0) {
        *d++ = *s++;
        n--;
    }

    return dst;
}
