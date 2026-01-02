/*
 * Function: __osSetFpcCsr
 * Address:  0x8000D790
 * Category: libultra/os
 * Status:   WIP
 *
 * Set FPU control/status register ($31)
 * Returns previous value.
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Read old FPC CSR, write new value, return old */
u32 __osSetFpcCsr(u32 csr) {
    u32 old;
    __asm__ volatile(
        "cfc1 %0, $31\n\t"
        "ctc1 %1, $31"
        : "=r"(old)
        : "r"(csr)
    );
    return old;
}
