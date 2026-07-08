/**
 * @file checksum.c
 * @brief Checksum calculation utilities
 *
 * Decompiled from asm/us/F700.s
 * Contains 8-bit and Adler-like checksum functions
 */

#include "types.h"

/**
 * Calculate 8-bit sum checksum
 * (0x8000EB00 - __osSumcalc / checksum8)
 *
 * Sums all bytes in the buffer, returning the lower 16 bits.
 * Uses loop unrolling (4 bytes at a time) for performance.
 *
 * @param data Pointer to data buffer
 * @param len Length of data in bytes
 * @return 16-bit checksum (sum of all bytes)
 */
u16 __osSumcalc(u8 *data, s32 len) {
    u32 sum = 0;
    u32 result;
    u8 *ptr = data;
    register s32 count = 0;
    register s32 limit;

    if (len <= 0) {
        goto done;
    }

    if (((limit = len & 3)) != 0) {
        do {
            sum += *ptr++;
            count++;
        } while (count != limit);

        if (count == len) {
            result = sum;
            goto done;
        }
    }
    for (;;) {
        sum += ptr[0];
        sum += ptr[1];
        sum += ptr[2];
        sum += ptr[3];
        count += 4;
        ptr += 4;
        if (count == len) {
            break;
        }
    }

done:
    result = sum;
    return (u16)result;
}

/**
 * Controller Pak ID sector checksum
 * (0x8000EB74 - __osIdCheckSum)
 *
 * Sums the ID block's halfwords into a checksum and inverse checksum.
 * The loop bound is sizeof(__OSPackId) - sizeof(u32) = 28 bytes (the ID
 * block minus its trailing checksum pair).
 *
 * MATCHING: 0x8000EB74 (asm/us/A810.s), IDO -O2
 * Source: ultralib src/io/contpfs.c:__osIdCheckSum @ e24c8367 (score 0 via conveyor)
 *
 * @param ptr Pointer to the ID block
 * @param csum Output: sum of halfwords
 * @param icsum Output: sum of complemented halfwords
 * @return Always 0
 */
s32 __osIdCheckSum(u16 *ptr, u16 *csum, u16 *icsum) {
    u16 data = 0;
    u32 j;

    *csum = *icsum = 0;

    for (j = 0; j < 28; j += 2) {
        data = *(u16 *)((u32)ptr + j);
        *csum += data;
        *icsum += ~data;
    }

    return 0;
}
