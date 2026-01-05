/*
 * Function: dll_get_priority
 * Address:  0x8000C490
 * Category: dll
 * Status:   WIP
 *
 * Get thread priority, uses current thread if NULL passed.
 * Global D_8002C3E0 is the current thread pointer.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

typedef struct TimerNode {
    struct TimerNode *next;
    s32 priority;
} TimerNode;

extern TimerNode *D_8002C3E0;

s32 dll_get_priority(TimerNode *node) {
    if (node == NULL) {
        node = D_8002C3E0;
    }
    return node->priority;
}
