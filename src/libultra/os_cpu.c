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
 * Assembly: mtc0 $a0, $12; nop; jr $ra; nop
 */
void __osSetSR(u32 sr) {
    __asm__ volatile("mtc0 %0, $12" : : "r"(sr));
}

/**
 * Get CP0 Status register
 * (func_8000D780 - __osGetSR)
 *
 * @return Current Status register value
 *
 * Assembly: mfc0 $v0, $12; jr $ra; nop
 */
u32 __osGetSR(void) {
    u32 sr;
    __asm__ volatile("mfc0 %0, $12" : "=r"(sr));
    return sr;
}

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
 * Assembly: ctc1 $a0, $31; jr $ra; nop
 */
void __osSetFpcCsr(u32 csr) {
    __asm__ volatile("ctc1 %0, $31" : : "r"(csr));
}

/**
 * Get FPU Control/Status register
 * (func_8000D790 - __osGetFpcCsr)
 *
 * Reads the current FPU CSR value.
 *
 * Assembly: cfc1 $v0, $31; jr $ra; nop
 *
 * @return Current FPU CSR value
 */
u32 __osGetFpcCsr(void) {
    u32 csr;
    __asm__ volatile("cfc1 %0, $31" : "=r"(csr));
    return csr;
}

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
 * Assembly: mfc0 $v0, $13; jr $ra; nop
 */
u32 __osGetCause(void) {
    u32 cause;
    __asm__ volatile("mfc0 %0, $13" : "=r"(cause));
    return cause;
}
