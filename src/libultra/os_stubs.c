/**
 * @file os_stubs.c
 * @brief Stub functions and variadic argument savers
 *
 * These functions are either stubs or used to save variadic arguments
 * to the stack for later processing.
 */

#include "types.h"

/**
 * Profile start hook
 * (func_800086F0 - __osProfileStart)
 *
 * Debug profiling entry point stub.
 * Saves arguments to stack and returns - actual profiling disabled.
 *
 * @param a0-a3 Profile context arguments
 */
void __osProfileStart(u32 a0, u32 a1, u32 a2, u32 a3) {
    /* Arguments are saved to stack by compiler */
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
}

/**
 * Profile end hook
 * (func_8000870C - __osProfileEnd)
 *
 * Debug profiling exit point stub.
 * Saves arguments to stack and returns - actual profiling disabled.
 *
 * @param a0-a3 Profile context arguments
 */
void __osProfileEnd(u32 a0, u32 a1, u32 a2, u32 a3) {
    /* Arguments are saved to stack by compiler */
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
}
