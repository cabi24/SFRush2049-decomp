/*
 * Function: __fixunsdfdi
 * Address:  0x80009A28
 * Category: libgcc
 * Status:   WIP
 *
 * Convert double to unsigned 64-bit integer (truncate toward zero).
 * Handles full unsigned range by subtracting 2^63 for large values.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Convert f64 to u64 (truncate) */
u64 __fixunsdfdi(f64 value) {
    return (u64)value;
}
