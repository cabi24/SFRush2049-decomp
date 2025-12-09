/**
 * @file os_stubs.c
 * @brief Stub functions and variadic argument savers
 *
 * These functions are either stubs or used to save variadic arguments
 * to the stack for later processing.
 */

#include "types.h"

/**
 * Variadic argument saver stub
 * (func_800086F0)
 *
 * Saves all 4 argument registers to the stack and returns.
 * Used for variadic functions or debugging.
 *
 * @param a0-a3 Arguments to save
 */
void func_800086F0(u32 a0, u32 a1, u32 a2, u32 a3) {
    /* Arguments are saved to stack by compiler */
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
}

/**
 * Variadic argument saver stub (alternate)
 * (func_8000870C)
 *
 * Identical to func_800086F0.
 *
 * @param a0-a3 Arguments to save
 */
void func_8000870C(u32 a0, u32 a1, u32 a2, u32 a3) {
    /* Arguments are saved to stack by compiler */
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
}
