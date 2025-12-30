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
 *
 * NOTE: For matching builds with IDO, these should be kept as
 * pure assembly files. This C version is for reference only.
 */

#include "types.h"

#ifdef NON_MATCHING
/* GCC inline assembly version - for non-matching builds */

/**
 * Set CP0 Status register
 * (func_8000D770 - __osSetSR)
 */
void __osSetSR(u32 sr) {
    __asm__ volatile("mtc0 %0, $12" : : "r"(sr));
}

/**
 * Get CP0 Status register
 * (func_8000D780 - __osGetSR)
 */
u32 __osGetSR(void) {
    u32 sr;
    __asm__ volatile("mfc0 %0, $12" : "=r"(sr));
    return sr;
}

/**
 * Set FPU Control/Status register
 * (func_800049F0 - __osSetFpcCsr)
 */
void __osSetFpcCsr(u32 csr) {
    __asm__ volatile("ctc1 %0, $31" : : "r"(csr));
}

/**
 * Get FPU Control/Status register
 * (func_8000D790 - __osGetFpcCsr)
 */
u32 __osGetFpcCsr(void) {
    u32 csr;
    __asm__ volatile("cfc1 %0, $31" : "=r"(csr));
    return csr;
}

/**
 * Get CP0 Cause register
 * (func_8000DBB0 - __osGetCause)
 */
u32 __osGetCause(void) {
    u32 cause;
    __asm__ volatile("mfc0 %0, $13" : "=r"(cause));
    return cause;
}

#else
/* IDO version - stub implementations for compilation testing */
/* For matching builds, use the assembly in asm/us/*.s */

void __osSetSR(u32 sr) {
    /* Stub - actual implementation in assembly */
    (void)sr;
}

u32 __osGetSR(void) {
    /* Stub - actual implementation in assembly */
    return 0;
}

void __osSetFpcCsr(u32 csr) {
    /* Stub - actual implementation in assembly */
    (void)csr;
}

u32 __osGetFpcCsr(void) {
    /* Stub - actual implementation in assembly */
    return 0;
}

u32 __osGetCause(void) {
    /* Stub - actual implementation in assembly */
    return 0;
}

#endif /* NON_MATCHING */
