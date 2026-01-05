/*
 * Function: __fixsfdi
 * Address:  0x80009A0C
 * Category: libgcc
 * Status:   WIP
 *
 * Convert float to signed 64-bit integer (truncate toward zero).
 * Uses MIPS III trunc.l.s instruction.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Convert f32 to s64 (truncate) */
s64 __fixsfdi(f32 value) {
    return (s64)value;
}
