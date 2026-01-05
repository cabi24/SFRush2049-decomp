/*
 * Function: __floatundidf
 * Address:  0x80009B94
 * Category: libgcc
 * Status:   WIP
 *
 * Convert unsigned 64-bit integer to double.
 * If high bit set, adds 2^64 to correct for signed cvt.d.l.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Convert u64 to f64 */
f64 __floatundidf(u64 value) {
    return (f64)value;
}
