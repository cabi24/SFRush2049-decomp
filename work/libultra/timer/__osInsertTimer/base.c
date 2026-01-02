/*
 * Function: __osInsertTimer
 * Address:  0x8000E390
 * Category: libultra/timer
 * Status:   WIP
 *
 * Get timer from timer queue head
 * Uses bnel (branch-not-equal-likely) pattern
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Timer queue struct - field 0 is enabled, field 8 is current timer */
typedef struct {
    void *enabled;      /* 0x0: non-zero if timer queue active */
    void *field_04;     /* 0x4: unused here */
    void *timer;        /* 0x8: current timer pointer */
} OSTimerQueue;

extern OSTimerQueue D_8002C380;

/* Returns current timer if queue enabled, else NULL */
void *__osInsertTimer(void) {
    if (D_8002C380.enabled != 0) {
        return D_8002C380.timer;
    }
    return 0;
}
