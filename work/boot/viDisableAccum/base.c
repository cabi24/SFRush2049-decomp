/*
 * Function: viDisableAccum
 * Address:  0x800013DC
 * Category: boot
 * Status:   WIP
 *
 * Disable VI time accumulation
 * Sets global flag gViAccumEnabled to 0
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

extern s32 gViAccumEnabled;

void viDisableAccum(void) {
    gViAccumEnabled = 0;
}
