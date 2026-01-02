/*
 * Function: strchr
 * Address:  0x80007C00
 * Category: libc
 * Status:   WIP
 *
 * Find first occurrence of character c in string s.
 * Returns pointer to char or NULL if not found.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

char *strchr(const char *s, s32 c) {
    u8 ch = (u8)c;
    u8 cur = *s;

    /* Check if first char matches */
    if (ch == cur) {
        return (char *)s;
    }

    /* Search through string */
    while (cur != 0) {
        cur = *++s;
        if (ch == cur) {
            return (char *)s;
        }
    }

    return NULL;
}
