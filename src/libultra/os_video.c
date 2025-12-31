/**
 * @file os_video.c
 * @brief Video system setup functions
 *
 * Decompiled from asm/us/8E10.s
 */

#include "types.h"

/**
 * Create scheduler
 * (0x80008210 - osCreateScheduler)
 *
 * Creates and initializes the OS scheduler for video and task management.
 *
 * @param mode Video mode
 * @param stack Scheduler stack pointer
 * @param pri Priority
 * @param numClients Number of scheduler clients
 */
void osCreateScheduler(s32 mode, void *stack, void *pri, s32 numClients) {
    /* Scheduler creation - stub */
    (void)mode;
    (void)stack;
    (void)pri;
    (void)numClients;
}

/**
 * Set scheduler video mode
 * (0x80008380 - osScSetVideoMode)
 *
 * Configures the scheduler's video mode settings.
 *
 * @param mode Video mode (NTSC/PAL)
 * @param flags Mode flags
 */
void osScSetVideoMode(s32 mode, s32 flags) {
    /* Video mode set - stub */
    (void)mode;
    (void)flags;
}

/**
 * Scheduler update
 * (0x80008460 - osScUpdate)
 *
 * Updates the scheduler state, typically called each frame.
 */
void osScUpdate(void) {
    /* Scheduler update - stub */
}
