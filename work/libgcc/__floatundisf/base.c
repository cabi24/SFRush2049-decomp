/*
 * Function: __floatundisf
 * Address:  0x80009BC8
 * Category: libgcc
 * Status:   WIP
 *
 * Convert unsigned 64-bit integer to float.
 * If high bit set, adds 2^64 to correct for signed cvt.s.l.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Convert u64 to f32 */
f32 __floatundisf(u64 value) {
    return (f32)value;
}
