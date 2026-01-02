/*
 * Function: __muldi3
 * Address:  0x8000DA58
 * Category: libgcc
 * Status:   WIP
 *
 * 64-bit unsigned multiplication.
 * Returns (a * b) using dmultu instruction.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* 64-bit multiplication */
u64 __muldi3(u64 a, u64 b) {
    return a * b;
}
