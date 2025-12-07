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
 */
void *memchr(const void *ptr, s32 value, u32 num) {
    const u8 *p = (const u8 *)ptr;

    while (num != 0) {
        if (*p == (u8)value) {
            return (void *)p;
        }
        p++;
        num--;
    }
    return NULL;
}

/**
 * Fill memory with a constant byte
 * @param ptr Pointer to memory block
 * @param value Value to set (converted to u8)
 * @param num Number of bytes to set
 * @return ptr
 */
void *memset(void *ptr, s32 value, u32 num) {
    u8 *p = (u8 *)ptr;
    u8 c = (u8)value;

    /* Fast path: if filling with zero, align and use word writes */
    if (c == 0) {
        /* Align to 4-byte boundary */
        u32 align = (u32)p & 3;
        while (num != 0 && align != 0 && align != 4) {
            *p++ = 0;
            align++;
            num--;
        }

        /* Write words */
        u32 *wp = (u32 *)p;
        while (num >= 4) {
            *wp++ = 0;
            num -= 4;
        }
        p = (u8 *)wp;
    }

    /* Write remaining bytes */
    while (num != 0) {
        *p++ = c;
        num--;
    }

    return ptr;
}

/**
 * Find first occurrence of character in string
 * @param str String to search
 * @param c Character to find
 * @return Pointer to character, or NULL if not found
 */
char *strchr(const char *str, s32 c) {
    u8 ch = (u8)c;

    while (*str != ch) {
        if (*str == '\0') {
            return NULL;
        }
        str++;
    }
    return (char *)str;
}

/**
 * Get string length
 * @param str Null-terminated string
 * @return Length of string (not including null terminator)
 */
u32 strlen(const char *str) {
    const char *p = str;

    while (*p != '\0') {
        p++;
    }
    return p - str;
}

/**
 * Copy memory
 * @param dest Destination buffer
 * @param src Source buffer
 * @param num Number of bytes to copy
 * @return dest
 */
void *memcpy(void *dest, const void *src, u32 num) {
    u8 *d = (u8 *)dest;
    const u8 *s = (const u8 *)src;

    while (num != 0) {
        *d++ = *s++;
        num--;
    }
    return dest;
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
