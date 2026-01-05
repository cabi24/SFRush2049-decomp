/*
 * Function: viUpdateTime
 * Address:  0x800013F4
 * Category: boot
 * Status:   WIP
 *
 * Update frame delta timing.
 * Computes delta ticks, handles accumulation mode,
 * clamps delta to max 5 ticks, converts to seconds.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

extern s32 gViCurrentTime;
extern s32 gViPrevTime;
extern s32 gViDeltaTicks;
extern s32 gViAccumEnabled;
extern s32 gViAccumTime;
extern s32 gViDeadline;
extern f32 gViSecondsPerTick;
extern f32 gViDeltaSeconds;
extern f32 gViElapsedSeconds;

void viUpdateTime(void) {
    gViDeltaTicks = gViCurrentTime - gViPrevTime;
    gViPrevTime = gViCurrentTime;

    if (gViAccumEnabled != 0) {
        gViAccumTime += gViDeltaTicks;
        gViDeadline += gViDeltaTicks;
    } else {
        gViElapsedSeconds = (f32)(gViCurrentTime - gViAccumTime) * gViSecondsPerTick;
    }

    if (gViDeltaTicks >= 6) {
        gViDeltaTicks = 5;
    }

    gViDeltaSeconds = (f32)gViDeltaTicks * gViSecondsPerTick;
}
