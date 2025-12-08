/**
 * @file os_cpu.c
 * @brief CPU control register functions
 *
 * Decompiled from asm/us/E370.s, 55F0.s
 * Contains functions for accessing MIPS R4300i CP0 registers
 * and FPU control registers.
 *
 * These are typically implemented as inline assembly or
 * handwritten assembly functions.
 */

#include "types.h"

/**
 * Set CP0 Status register
 * (func_8000D770 - __osSetSR)
 *
 * The Status register controls:
 * - Interrupt enable (IE bit 0)
 * - Interrupt mask (IM bits 8-15)
 * - Kernel/User mode
 * - 64-bit addressing mode
 * - Coprocessor usability
 *
 * @param sr New Status register value
 *
 * Note: Implemented in assembly (mtc0 $a0, $12)
 */
/* Assembly stub - actual implementation in asm */
#if 0
void __osSetSR(u32 sr) {
    __mtc0(12, sr);
}
#endif

/**
 * Get CP0 Status register
 * (func_8000D780 - __osGetSR)
 *
 * @return Current Status register value
 *
 * Note: Implemented in assembly (mfc0 $v0, $12)
 */
/* Assembly stub - actual implementation in asm */
#if 0
u32 __osGetSR(void) {
    return __mfc0(12);
}
#endif

/**
 * Set FPU Control/Status register
 * (func_800049F0 - __osSetFpcCsr)
 *
 * The FPU CSR controls:
 * - Rounding mode (bits 0-1)
 * - Exception flags (bits 2-6)
 * - Exception enables (bits 7-11)
 * - Exception causes (bits 12-17)
 * - Condition bit (bit 23)
 * - Flush-to-zero (bit 24)
 *
 * @param csr New FPU CSR value
 *
 * Note: Implemented in assembly (ctc1 $a0, $31)
 */
/* Assembly stub - actual implementation in asm */
#if 0
void __osSetFpcCsr(u32 csr) {
    __ctc1(31, csr);
}
#endif

/**
 * Get and Set FPU Control/Status register
 * (func_8000D790 - __osGetFpcCsr)
 *
 * Reads the current FPU CSR value and sets a new one.
 * This is an atomic swap operation for the FPU control register.
 *
 * Assembly:
 *   cfc1 $v0, $31    ; Read current CSR into $v0
 *   ctc1 $a0, $31    ; Write new CSR from $a0
 *   jr $ra           ; Return
 *
 * @param newCsr New FPU CSR value to set
 * @return Previous FPU CSR value
 *
 * Note: This is a leaf function implemented in assembly.
 */
/* Assembly stub - actual implementation in asm */
#if 0
u32 __osGetFpcCsr(u32 newCsr) {
    u32 oldCsr;
    oldCsr = __cfc1(31);
    __ctc1(31, newCsr);
    return oldCsr;
}
#endif

/**
 * Get CP0 Cause register
 * (func_8000DBB0 - __osGetCause)
 *
 * The Cause register indicates the cause of the most recent
 * exception. Contains:
 * - Exception code (bits 2-6)
 * - Pending hardware interrupts (bits 8-15)
 * - Coprocessor error (bits 28-29)
 * - Branch delay slot (bit 31)
 *
 * @return Current Cause register value
 *
 * Assembly:
 *   mfc0 $v0, $13   ; Read Cause register into $v0
 *   jr $ra          ; Return
 *
 * Note: This is a leaf function implemented in assembly.
 */
u32 __osGetCause(void) {
    /* NOTE: Returns CP0 register $13 (Cause)
     * Cannot implement in C - requires inline assembly: mfc0 $v0, $13 */
    return 0; /* placeholder */
}
