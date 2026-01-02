/*
 * Function: __osPopThread
 * Address:  0x8000D10C
 * Category: libultra/thread
 * Status:   WIP
 *
 * Pop the highest priority thread from a thread queue
 * Returns the popped thread and updates queue head
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

typedef struct OSThread {
    struct OSThread *next;
    /* ... other fields */
} OSThread;

OSThread *__osPopThread(OSThread **queue) {
    OSThread *thread = *queue;
    *queue = thread->next;
    return thread;
}
