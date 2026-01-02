/*
 * Function: memset
 * Address:  0x80002790
 * Category: libc
 * Status:   WIP
 *
 * Optimized memset - uses word stores when filling with zero
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

void *memset(void *s, int c, u32 n) {
    u8 *dst = s;
    u32 align;
    u32 *dstp;

    if (c != 0) {
        goto fill_nonzero;
    }

    /* Zero fill - can use word stores */
    align = (u32)dst & 3;

    /* Align to word boundary first */
    if (n != 0 && align > 0 && align != 4) {
        do {
            *dst = 0;
            dst++;
            align++;
            n--;
        } while (n != 0 && align > 0 && align != 4);
    }

    /* Word-aligned zero fill */
    dstp = (u32 *)dst;
    if (n >= 4) {
        do {
            *dstp = 0;
            dstp++;
            n -= 4;
        } while (n >= 4);
    }
    dst = (u8 *)dstp;

fill_nonzero:
    /* Byte fill for non-zero or remaining bytes */
    while (n-- > 0) {
        *dst = c;
        dst++;
    }

    return s;
}
