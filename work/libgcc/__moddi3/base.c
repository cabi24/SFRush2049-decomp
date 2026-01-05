/*
 * Function: __moddi3
 * Address:  0x8000DAE8
 * Category: libgcc
 * Status:   WIP
 *
 * Signed 64-bit modulo.
 * Uses MIPS III ddiv instruction.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Signed 64-bit modulo */
s64 __moddi3(s64 dividend, s64 divisor) {
    return dividend % divisor;
}
