/*
 * Function: __setfpcsr
 * Address:  0x800049F0
 * Category: boot
 * Status:   WIP
 *
 * Set FPU control/status register ($31) without returning old value
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Set FPC CSR register */
void __setfpcsr(u32 csr) {
    __asm__ volatile(
        "ctc1 %0, $31\n\t"
        "nop\n\t"
        "nop"
        :
        : "r"(csr)
    );
}
