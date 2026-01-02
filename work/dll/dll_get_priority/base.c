/*
 * Function: dll_get_priority
 * Address:  0x8000C490
 * Category: dll
 * Status:   WIP
 *
 * Get thread priority, uses current thread if NULL passed
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

typedef struct OSThread {
    struct OSThread *next;
    s32 priority;
    /* ... other fields */
} OSThread;

extern OSThread *__osRunningThread;

s32 dll_get_priority(OSThread *thread) {
    if (thread == NULL) {
        thread = __osRunningThread;
    }
    return thread->priority;
}
