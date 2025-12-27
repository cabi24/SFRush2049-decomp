/**
 * @file os_pi_write.c
 * @brief PI write I/O function
 *
 * Decompiled from asm/us/E3F0.s
 *
 * Provides direct (non-DMA) write access to PI address space.
 */

#include "types.h"
#include "PR/os_pi.h"

/* External PI access check */
extern s32 __osPiDeviceBusy(void);  /* func_8000FD70 */

/**
 * Write word to PI address
 * (func_8000D7F0 - osPiWriteIo)
 *
 * Writes a 32-bit value directly to a PI (cartridge) address.
 * Waits for PI to be idle before writing.
 *
 * @param devAddr Physical device address to write
 * @param data Value to write
 * @return 0 on success, -1 if PI is busy
 */
s32 osPiWriteIo(u32 devAddr, u32 data) {
    /* Check if PI is busy */
    if (__osPiDeviceBusy() != 0) {
        return -1;
    }

    /* Write to uncached address (KSEG1) */
    *(vu32 *)(devAddr | 0xA0000000) = data;

    return 0;
}
