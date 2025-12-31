/**
 * @file os_cont_status.c
 * @brief Controller status and reset functions
 *
 * Decompiled from asm/us/A050.s
 */

#include "types.h"

/**
 * Get controller status
 * (0x80009450 - osContGetStatus)
 *
 * Returns the status of the controller subsystem.
 *
 * @return 0 on success, error code on failure
 */
s32 osContGetStatus(void) {
    /* Controller status - stub */
    return 0;
}

/**
 * Parse controller status response
 * (0x800095AC) - Note: may be duplicated in os_cont.c
 */

/**
 * Reset controller RAM
 * (0x80009658 - osContRamReset)
 *
 * Resets the controller RAM (used for Controller Pak).
 */
void osContRamReset(void) {
    /* Controller RAM reset - stub */
}
