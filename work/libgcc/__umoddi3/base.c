/*
 * Function: __umoddi3
 * Address:  0x8000D91C
 * Category: libgcc
 * Status:   WIP
 *
 * Unsigned 64-bit modulo.
 * Uses MIPS III ddivu instruction.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Unsigned 64-bit modulo */
u64 __umoddi3(u64 dividend, u64 divisor) {
    return dividend % divisor;
}
