/*
 * Function: osDpWait
 * Address:  0x80007B80
 * Category: libultra/dp
 * Status:   WIP
 *
 * Wait for RDP to be ready by setting SP status
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* SP status bit to signal DP sync */
#define SP_SET_SIG2 0x400

extern void __osSpSetStatus(u32 status);

void osDpWait(void) {
    __osSpSetStatus(SP_SET_SIG2);
}
