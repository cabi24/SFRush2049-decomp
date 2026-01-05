/*
 * Function: __floatdisf
 * Address:  0x80009B7C
 * Category: libgcc
 * Status:   WIP
 *
 * Convert signed 64-bit integer to float.
 * Uses MIPS III cvt.s.l instruction.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Convert s64 to f32 */
f32 __floatdisf(s64 value) {
    return (f32)value;
}
