/**
 * @file os_debug.c
 * @brief Debug stub functions
 *
 * Decompiled from asm/us/92F0.s
 * These appear to be debug/profiling stub functions.
 */

#include "types.h"

/**
 * Profile start hook (debug variant)
 * (0x800086F0 - __osProfileStart)
 *
 * Debug profiling entry point stub.
 * Stores arguments to stack and returns - actual profiling disabled.
 *
 * Note: This is a duplicate of the definition in os_stubs.c.
 * Only one should be included in the build.
 */
void __osProfileStart(s32 a0, s32 a1, s32 a2, s32 a3) {
    /* Stub - just stores args and returns */
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
}

/**
 * Profile end hook (debug variant)
 * (0x8000870C - __osProfileEnd)
 *
 * Debug profiling exit point stub.
 * Stores arguments to stack and returns - actual profiling disabled.
 *
 * Note: This is a duplicate of the definition in os_stubs.c.
 * Only one should be included in the build.
 */
void __osProfileEnd(s32 a0, s32 a1, s32 a2, s32 a3) {
    /* Stub - just stores args and returns */
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
}
