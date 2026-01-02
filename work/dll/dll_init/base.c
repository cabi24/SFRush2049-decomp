/*
 * Function: dll_init
 * Address:  0x8000C090
 * Category: dll
 * Status:   WIP
 *
 * Initialize timer queue (doubly-linked list) globals.
 * Sets up circular DLL and clears timer state.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Timer queue structure */
typedef struct {
    void *next;        /* 0x00 */
    void *prev;        /* 0x04 */
    u32 field_08;      /* 0x08 */
    u32 field_0C;      /* 0x0C */
    u32 field_10;      /* 0x10 */
    u32 field_14;      /* 0x14 */
    u32 field_18;      /* 0x18 */
    u32 field_1C;      /* 0x1C */
} TimerQueue;

extern u32 D_80037C50;
extern u32 D_80037C54;
extern u32 D_80037C58;
extern u32 D_80037C5C;
extern TimerQueue *D_8002C3F0;

void dll_init(void) {
    TimerQueue *tq;

    D_80037C54 = 0;
    D_80037C50 = 0;
    D_80037C58 = 0;
    D_80037C5C = 0;

    /* Make circular list pointing to self */
    tq = D_8002C3F0;
    tq->prev = tq;
    tq->next = tq->prev;

    /* Clear timer fields */
    tq->field_10 = 0;
    tq->field_14 = 0;
    tq->field_08 = tq->field_10;
    tq->field_0C = tq->field_14;
    tq->field_18 = 0;
    tq->field_1C = 0;
}
