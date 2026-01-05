/*
 * Function: __floatdidf
 * Address:  0x80009B64
 * Category: libgcc
 * Status:   WIP
 *
 * Convert signed 64-bit integer to double.
 * Uses MIPS III cvt.d.l instruction.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Convert s64 to f64 */
f64 __floatdidf(s64 value) {
    return (f64)value;
}
