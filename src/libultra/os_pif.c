/**
 * @file os_pif.c
 * @brief PIF (Peripheral Interface) functions
 *
 * Decompiled from asm/us/8DD0.s
 */

#include "types.h"

/* External functions */
extern void osPiGetAccess(void);                   /* 0x8000DC10 */
extern s32 osPiReadWord(u32 addr, u32 *data);      /* 0x8000DC80 */
extern void osPiReleaseAccess(void);               /* 0x8000DC54 */

/**
 * Read word from PIF RAM
 * (0x800081D0)
 *
 * Reads a 32-bit value from PIF RAM at the specified address.
 *
 * @param addr PIF RAM address
 * @param data Pointer to store the read value
 * @return Result from PIF read operation
 */
s32 osPifReadWord(u32 addr, u32 *data) {
    s32 result;

    osPiGetAccess();
    result = osPiReadWord(addr, data);
    osPiReleaseAccess();

    return result;
}
