/*
 * Function: viScheduleTick
 * Address:  0x800014F0
 * Category: boot
 * Status:   WIP
 *
 * Schedule next tick deadline.
 * Saves current time snapshot and sets deadline.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

extern s32 gViCurrentTime;
extern s32 gViLastSnapshotTime;
extern s32 gViDeadline;
extern f32 gViTickRate;

void viScheduleTick(f32 seconds) {
    gViLastSnapshotTime = gViCurrentTime;
    gViDeadline = (s32)(seconds * gViTickRate) + gViCurrentTime;
}
