/*
 * Function: bcmp
 * Address:  0x8000F8D0
 * Category: boot
 * Status:   WIP
 *
 * Compare two memory regions byte-by-byte.
 * Returns 0 if equal, non-zero if different.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Compare two memory regions */
s32 bcmp(const void *s1, const void *s2, u32 n) {
    const u8 *p1 = (const u8 *)s1;
    const u8 *p2 = (const u8 *)s2;

    while (n > 0) {
        if (*p1 != *p2) {
            return 1;
        }
        p1++;
        p2++;
        n--;
    }
    return 0;
}
