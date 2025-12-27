/**
 * @file os_pif.c
 * @brief PIF (Peripheral Interface) functions
 *
 * Decompiled from asm/us/8DD0.s
 */

#include "types.h"

/* External functions */
extern void func_8000DC10(void);     /* PIF acquire */
extern s32 func_8000DC80(u32 addr, u32 *data);  /* PIF read */
extern void func_8000DC54(void);     /* PIF release */

/**
 * Read word from PIF RAM
 * (func_800081D0)
 *
 * Reads a 32-bit value from PIF RAM at the specified address.
 *
 * @param addr PIF RAM address
 * @param data Pointer to store the read value
 * @return Result from PIF read operation
 */
s32 osPifReadWord(u32 addr, u32 *data) {
    s32 result;

    func_8000DC10();  /* Acquire PIF access */
    result = func_8000DC80(addr, data);  /* Read from PIF */
    func_8000DC54();  /* Release PIF access */

    return result;
}
