/**
 * @file os_pi_write.c
 * @brief PI write I/O function
 *
 * Decompiled from asm/us/E3F0.s
 */

#include "types.h"

/* External PI access check */
extern s32 func_8000FD70(void);

/**
 * Write word to PI uncached address
 * (func_8000D7F0 - osPiWriteIo)
 *
 * Writes a 32-bit value to an uncached PI address.
 *
 * @param addr Physical address to write
 * @param data Value to write
 * @return 0 on success, -1 on failure
 */
s32 osPiWriteIo(u32 addr, u32 data) {
    /* Check PI access */
    if (func_8000FD70() != 0) {
        return -1;
    }

    /* Write to uncached address (KSEG1) */
    *(vu32 *)(addr | 0xA0000000) = data;

    return 0;
}
