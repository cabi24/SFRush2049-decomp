/*
 * Function: __fixunssfdi
 * Address:  0x80009AC8
 * Category: libgcc
 * Status:   WIP
 *
 * Convert float to unsigned 64-bit integer (truncate toward zero).
 * Handles full unsigned range by subtracting 2^63 for large values.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Convert f32 to u64 (truncate) */
u64 __fixunssfdi(f32 value) {
    return (u64)value;
}
