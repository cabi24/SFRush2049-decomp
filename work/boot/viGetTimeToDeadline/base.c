/*
 * Function: viGetTimeToDeadline
 * Address:  0x80001578
 * Category: boot
 * Status:   WIP
 *
 * Get time remaining until deadline in seconds.
 * Converts tick difference to float seconds.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

extern s32 gViDeadline;
extern s32 gViCurrentTime;
extern f32 gViSecondsPerTick;

f32 viGetTimeToDeadline(void) {
    s32 remaining = gViDeadline - gViCurrentTime;
    return (f32)remaining * gViSecondsPerTick;
}
