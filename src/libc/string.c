/**
 * @file string.c
 * @brief Standard C library string functions
 *
 * Decompiled from asm/us/3330.s, asm/us/3390.s, asm/us/8800.s
 */

#include "types.h"

/**
 * Search for a byte in a memory block
 * @param ptr Pointer to memory block
 * @param value Value to search for
 * @param num Number of bytes to search
 * @return Pointer to first occurrence, or NULL if not found
 *
 * MATCHING: 0x80002730 (asm/us/3330.s)
 * - ptr stored on stack (volatile pattern)
 * - pre-decrement in delay slot pattern
 */
void *memchr(void *ptr, s32 value, u32 num) {
    /* Volatile to force stack storage like original assembly */
    void * volatile sp_ptr = ptr;
    u32 remaining;

    remaining = num;
    if (remaining == 0) {
        return NULL;
    }
    num--;

    do {
        if (*(u8 *)sp_ptr == (u8)value) {
            return (void *)sp_ptr;
        }
        sp_ptr = (u8 *)sp_ptr + 1;
        remaining = num;
        num--;
    } while (remaining != 0);

    return NULL;
}

/**
 * Fill memory with a constant byte
 * @param ptr Pointer to memory block
 * @param value Value to set (converted to u8)
 * @param num Number of bytes to set
 * @return ptr
 *
 * MATCHING: 0x80002790 (asm/us/3390.s)
 * - ptr stored on stack at sp+12
 * - align stored on stack at sp+8
 * - aligned_ptr stored on stack at sp+4
 * - Fast path for value==0 with word writes
 */
void *memset(void *ptr, s32 value, u32 num) {
    /* Stack variables to match assembly */
    void * volatile sp_ptr = ptr;       /* sp+12 */
    u32 volatile align;                  /* sp+8 */
    void * volatile aligned_ptr;         /* sp+4 */
    u32 remaining;

    if (value != 0) {
        goto byte_loop;
    }

    /* value == 0: Fast zero-fill path */
    align = (u32)sp_ptr & 3;

    /* Align to 4-byte boundary */
    if (num != 0) {
        if (align > 0 && align != 4) {
            do {
                *(u8 *)sp_ptr = 0;
                sp_ptr = (u8 *)sp_ptr + 1;
                align++;
                num--;
                if (num == 0) break;
            } while (align > 0 && align != 4);
        }
    }

    /* Word-aligned zero writes */
    aligned_ptr = sp_ptr;
    if (num >= 4) {
        do {
            *(u32 *)aligned_ptr = 0;
            aligned_ptr = (u32 *)aligned_ptr + 1;
            num -= 4;
        } while (num >= 4);
    }
    sp_ptr = aligned_ptr;

byte_loop:
    /* Byte-by-byte writes */
    remaining = (num > 0) ? 1 : 0;
    num--;
    if (remaining == 0) {
        goto done;
    }
    do {
        *(u8 *)sp_ptr = (u8)value;
        sp_ptr = (u8 *)sp_ptr + 1;
        remaining = (num > 0) ? 1 : 0;
        num--;
    } while (remaining != 0);

done:
    return ptr;
}

/**
 * Find first occurrence of character in string
 * @param str String to search
 * @param c Character to find
 * @return Pointer to character, or NULL if not found
 *
 * MATCHING: 0x80007C00 (asm/us/8800.s), IDO -O2
 * Source: ultralib src/libc/string.c:strchr @ e24c8367 (score 0 via conveyor)
 */
char *strchr(const char *s, int c) {
    const char ch = c;

    while (*s != ch) {
        if (*s == 0) {
            return NULL;
        }
        s++;
    }
    return (char *)s;
}

/**
 * Get string length
 * @param str Null-terminated string
 * @return Length of string (not including null terminator)
 *
 * MATCHING: 0x80007C40 (asm/us/8800.s)
 * - Uses branch-likely for loop optimization
 * - Loads next byte in delay slot
 */
u32 strlen(const char *str) {
    const char *p;

    p = str;
    if (*(u8 *)p != 0) {
        do {
            p++;
        } while (*(u8 *)p != 0);
    }
    return (u32)(p - str);
}

/**
 * Copy memory
 * @param s1 Destination buffer
 * @param s2 Source buffer
 * @param n Number of bytes to copy
 * @return s1
 *
 * MATCHING: 0x80007C68 (asm/us/8800.s), IDO -O2
 * Source: ultralib src/libc/string.c:memcpy @ e24c8367 (score 0 via conveyor)
 */
void *memcpy(void *s1, const void *s2, u32 n) {
    char *su1 = (char *)s1;
    const char *su2 = (const char *)s2;

    while (n > 0) {
        *su1 = *su2;
        su1++;
        su2++;
        n--;
    }
    return (void *)s1;
}

/**
 * Optimized zero-fill memory
 * Uses 32-byte unrolled loop for large blocks
 * @param ptr Pointer to memory block
 * @param num Number of bytes to zero
 */
void bzero(void *ptr, u32 num) {
    u8 *p = (u8 *)ptr;
    u32 align;
    u32 blocks;

    if (num >= 12) {
        /* Align to 4-byte boundary */
        align = (-(u32)p) & 3;
        num -= align;
        if (align != 0) {
            *(u32 *)p = 0;  /* Unaligned write */
            p += align;
        }

        /* Write 32-byte blocks */
        blocks = num & ~0x1F;
        num -= blocks;
        if (blocks != 0) {
            u32 *wp = (u32 *)p;
            u32 *end = (u32 *)(p + blocks);
            do {
                wp[0] = 0;
                wp[1] = 0;
                wp[2] = 0;
                wp[3] = 0;
                wp[4] = 0;
                wp[5] = 0;
                wp[6] = 0;
                wp[7] = 0;
                wp += 8;
            } while (wp != end);
            p = (u8 *)wp;
        }

        /* Write remaining 4-byte words */
        blocks = num & ~3;
        num -= blocks;
        if (blocks != 0) {
            u32 *wp = (u32 *)p;
            u32 *end = (u32 *)(p + blocks);
            do {
                *wp++ = 0;
            } while (wp != end);
            p = (u8 *)wp;
        }
    }

    /* Write remaining bytes */
    while (num > 0) {
        *p++ = 0;
        num--;
    }
}
