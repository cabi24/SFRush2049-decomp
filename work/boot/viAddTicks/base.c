/*
 * Function: viAddTicks
 * Address:  0x8000153C
 * Category: boot
 * Status:   WIP
 *
 * Add ticks to VI deadline.
 * Converts float seconds to ticks using tick rate.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

extern f32 gViTickRate;
extern s32 gViDeadline;

void viAddTicks(f32 seconds) {
    gViDeadline += (s32)(seconds * gViTickRate);
}
