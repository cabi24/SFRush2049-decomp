/*
 * Function: osGetActiveQueue
 * Address:  0x8000C660
 * Category: libultra/os
 * Status:   WIP
 *
 * Get the active thread queue pointer
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* External reference to active thread queue */
extern void *__osActiveQueue;

void *osGetActiveQueue(void) {
    return __osActiveQueue;
}
