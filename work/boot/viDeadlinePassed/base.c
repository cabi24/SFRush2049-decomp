/*
 * Function: viDeadlinePassed
 * Address:  0x800015BC
 * Category: boot
 * Status:   WIP
 *
 * Check if VI deadline has passed
 * Returns 1 if deadline <= currentTime, 0 otherwise
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

extern s32 gViDeadline;
extern s32 gViCurrentTime;

s32 viDeadlinePassed(void) {
    s32 remaining = gViDeadline - gViCurrentTime;
    return remaining < 1;
}
