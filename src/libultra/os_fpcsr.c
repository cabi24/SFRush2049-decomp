/**
 * @file os_fpcsr.c
 * @brief FPU control/status register and PI I/O functions
 *
 * Decompiled from asm/us/E390.s
 */

#include "types.h"

/* External PI access check */
extern s32 func_8000FD70(void);

/**
 * Get and set FPU control/status register
 * (func_8000D790 - __osSetFpcCsr)
 *
 * Sets the FPU control/status register to a new value
 * and returns the old value.
 *
 * @param value New FPCSR value
 * @return Previous FPCSR value
 */
u32 __osSetFpcCsr(u32 value) {
    u32 old;

    /* CFC1 - read FPCSR into old */
    __asm__ volatile("cfc1 %0, $31" : "=r"(old));

    /* CTC1 - write value to FPCSR */
    __asm__ volatile("ctc1 %0, $31" : : "r"(value));

    return old;
}

/**
 * Read word from PI uncached address
 * (func_8000D7A0 - osPiReadIo)
 *
 * Reads a 32-bit value from an uncached PI address.
 *
 * @param addr Physical address to read
 * @param data Pointer to store the read value
 * @return 0 on success, -1 on failure
 */
s32 osPiReadIo(u32 addr, u32 *data) {
    /* Check PI access */
    if (func_8000FD70() != 0) {
        return -1;
    }

    /* Read from uncached address (KSEG1) */
    *data = *(vu32 *)(addr | 0xA0000000);

    return 0;
}
