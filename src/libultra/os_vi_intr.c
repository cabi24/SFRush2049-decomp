/**
 * @file os_vi_intr.c
 * @brief VI interrupt handler
 *
 * Decompiled from asm/us/D270.s
 */

#include "types.h"

/**
 * Swap VI context
 * (func_8000C670 - __osViSwapContext)
 *
 * Swaps between the current and next VI contexts.
 * Called during VI retrace interrupt to switch display buffers.
 */
void __osViSwapContext(void) {
    /* VI context swap - implemented in os_vi_init.c */
}
