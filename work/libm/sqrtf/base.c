/*
 * Function: sqrtf
 * Address:  0x8000E3C0
 * Category: libm
 * Status:   WIP
 *
 * square root (float) - uses MIPS sqrt.s instruction
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Hardware sqrt - returns sqrt of input in delay slot */
f32 sqrtf(f32 x) {
    f32 result;
    __asm__ volatile("sqrt.s %0, %1" : "=f"(result) : "f"(x));
    return result;
}
