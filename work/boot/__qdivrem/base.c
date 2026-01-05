/*
 * Function: __qdivrem
 * Address:  0x8000DA88
 * Category: boot
 * Status:   WIP
 *
 * 64-bit division returning both quotient and remainder.
 * Divisor is sign-extended from 16 bits to 64 bits.
 * Uses MIPS III ddivu instruction for 64-bit unsigned divide.
 *
 * Compiler flags: -g0 -O2 -mips3 -G 0 -non_shared
 *
 * Note: Target uses ddivu (MIPS III 64-bit divide) inline.
 * IDO compiler required for matching build.
 */

#include "types.h"

/*
 * Compute quotient and remainder of 64-bit unsigned division.
 *
 * Assembly does:
 *   1. Load divisor as s16 from sp+0x12, sign-extend to 64-bit
 *   2. Store a2/a3 to stack, reload as 64-bit dividend
 *   3. ddivu dividend, divisor
 *   4. Store mflo (quotient) to *a0
 *   5. Repeat ddivu, store mfhi (remainder) to *a1
 *
 * @param quotient  Output: dividend / divisor
 * @param remainder Output: dividend % divisor
 * @param dividend  64-bit dividend
 * @param divisor   16-bit signed divisor (sign-extended to 64-bit)
 */
void __qdivrem(u64 *quotient, u64 *remainder, u64 dividend, s16 divisor) {
    u64 div64 = (u64)(s64)divisor;  /* Sign-extend s16 to u64 */

    *quotient = dividend / div64;
    *remainder = dividend % div64;
}
