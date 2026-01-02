/*
 * Function: viEnableAccum
 * Address:  0x800013C0
 * Category: boot
 * Status:   WIP
 *
 * Enable VI time accumulation
 * Sets global flag gViAccumEnabled to 1
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

extern s32 gViAccumEnabled;

void viEnableAccum(void) {
    gViAccumEnabled = 1;
}
