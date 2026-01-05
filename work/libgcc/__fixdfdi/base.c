/*
 * Function: __fixdfdi
 * Address:  0x800099F0
 * Category: libgcc
 * Status:   WIP
 *
 * Convert double to signed 64-bit integer (truncate toward zero).
 * Uses MIPS III trunc.l.d instruction.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Convert f64 to s64 (truncate) */
s64 __fixdfdi(f64 value) {
    return (s64)value;
}
