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
 * Calculate Adler-like checksum (similar to Adler-32)
 * (0x8000EB74 - __osIdCheckSum)
 *
 * Computes a rolling checksum using sum and complement sum.
 * Processes 32 bytes (0x1C + initial 4 bytes) of header data.
 *
 * @param header Pointer to header data (at least 32 bytes)
 * @param out_sum Output: sum of values
 * @param out_comp Output: sum of complements
 * @return Always 0
 */
s32 __osIdCheckSum(u16 *header, u16 *out_sum, u16 *out_comp) {
    u16 sum;
    u16 comp;
    u16 val;
    u16 *ptr;
    s32 i;

    *out_sum = 0;
    *out_comp = 0;

    /* Process first two 16-bit values from header */
    val = header[0];
    sum = val;
    *out_sum = sum;
    comp = *out_comp + ~val;
    *out_comp = comp;

    val = header[1];
    sum = *out_sum + val;
    *out_sum = sum;
    comp = *out_comp + ~val;
    *out_comp = comp;

    /* Process remaining 28 bytes (7 iterations of 4 halfwords each) */
    ptr = header + 2;
    for (i = 4; i != 0x1C; i += 8) {
        /* First halfword */
        val = ptr[0];
        sum = *out_sum + val;
        *out_sum = sum;
        comp = *out_comp + ~val;
        *out_comp = comp;

        /* Second halfword */
        val = ptr[1];
        sum = *out_sum + val;
        *out_sum = sum;
        comp = *out_comp + ~val;
        *out_comp = comp;

        /* Third halfword */
        val = ptr[2];
        sum = *out_sum + val;
        *out_sum = sum;
        comp = *out_comp + ~val;
        *out_comp = comp;

        /* Fourth halfword */
        val = ptr[3];
        sum = *out_sum + val;
        *out_sum = sum;
        comp = *out_comp + ~val;
        *out_comp = comp;

        ptr += 4;
    }

    return 0;
}
