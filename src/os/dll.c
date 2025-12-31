/**
 * @file dll.c
 * @brief Timer queue and doubly-linked list utilities
 *
 * Decompiled from asm/us/CC50.s
 * Similar to arcade GUTS timer queue (OS/os_proto.h struct tq)
 *
 * This implements a delta-queue timer system where each node stores
 * the time remaining relative to the previous node. This allows O(1)
 * timer decrements - only the head node's delta needs updating each tick.
 */

#include "types.h"

/* Forward declarations */
struct DLLNode;
struct TimerNode;

/**
 * Basic doubly-linked list node structure
 */
typedef struct DLLNode {
    struct DLLNode *next;
    struct DLLNode *prev;
} DLLNode;

/**
 * Timer queue node structure
 * Based on arcade struct tq but adapted for N64
 *
 * Layout from assembly analysis:
 *   0x00: next pointer
 *   0x04: prev pointer
 *   0x08: reload_hi (upper 32 bits of reload time)
 *   0x0C: reload_lo (lower 32 bits of reload time)
 *   0x10: delta_hi (upper 32 bits of remaining time)
 *   0x14: delta_lo (lower 32 bits of remaining time)
 *   0x18: message queue pointer
 *   0x1C: message value
 */
typedef struct TimerNode {
    struct TimerNode *next;     /* 0x00: Next node in queue */
    struct TimerNode *prev;     /* 0x04: Previous node in queue */
    u32 reload_hi;              /* 0x08: Reload time (high) - 0 for one-shot */
    u32 reload_lo;              /* 0x0C: Reload time (low) */
    u32 delta_hi;               /* 0x10: Time remaining (high) */
    u32 delta_lo;               /* 0x14: Time remaining (low) */
    void *mq;                   /* 0x18: Message queue to send to */
    s32 msg;                    /* 0x1C: Message value */
} TimerNode;

/**
 * Timer queue head structure
 * Self-referential when empty (head->next == head)
 */
typedef struct {
    TimerNode *head;            /* 0x00: First node */
    TimerNode *tail;            /* 0x04: Last node */
    u32 field_08;               /* 0x08 */
    u32 field_0C;               /* 0x0C */
    u32 field_10;               /* 0x10 */
    u32 field_14;               /* 0x14 */
    s32 count;                  /* 0x18: Number of entries */
    s32 field_1C;               /* 0x1C */
} TimerQueueHead;

/* Global timer queue state (standard libultra names) */
extern TimerQueueHead *__osTimerList;   /* Timer queue head pointer: 0x8002C3F0 */
extern s32 gViTimeAccumHi;              /* VI time accumulator high word: 0x80037C50 */
extern s32 gViTimeAccumLo;              /* VI time accumulator low word: 0x80037C54 */
extern s32 gViLastCount;                /* VI last osGetCount value: 0x80037C58 */
extern s32 gViRetraceCount;             /* VI retrace tick counter: 0x80037C5C */
extern u32 __osTimerCounter;            /* Last update timestamp: 0x80037C60 */

/* External functions */
extern u32 osGetCount(void);                        /* Get CP0 Count register */
extern void osJamMesg(void *mq, s32 msg, s32 arg);  /* Insert message at front of queue */
extern void __osSetCompare(s32 arg);                /* Set CP0 Compare register */
extern s32 __osDisableInt(void);                    /* Disable interrupts */
extern void __osRestoreInt(s32 mask);               /* Restore interrupts */

/* Forward declarations */
void dll_reschedule(u32 hi, u32 lo);
void dll_insert(TimerNode *node);

/**
 * Remove a node from a singly-linked list
 * @param list Pointer to list head pointer
 * @param node Node to remove
 */
void dll_remove(DLLNode **list, DLLNode *node) {
    DLLNode **prev = list;
    DLLNode *curr = *list;

    while (curr != NULL) {
        if (curr == node) {
            *prev = node->next;
            return;
        }
        prev = &curr->next;
        curr = curr->next;
    }
}

/**
 * Initialize the global timer queue
 * (func_8000C090)
 *
 * Sets up an empty circular timer queue where head->next points to itself.
 */
void dll_init(void) {
    TimerQueueHead *q = __osTimerList;

    gViTimeAccumHi = 0;
    gViTimeAccumLo = 0;
    gViLastCount = 0;
    gViRetraceCount = 0;

    /* Initialize circular list - next/prev point to self */
    q->tail = (TimerNode *)q;
    q->head = q->tail;

    q->field_10 = 0;
    q->field_14 = 0;

    /* Copy field_10/14 to field_08/0C */
    q->field_08 = q->field_10;
    q->field_0C = q->field_14;

    q->count = 0;
    q->field_1C = 0;
}

/**
 * Process expired timers (delta queue maintenance)
 * (func_8000C11C - dll_update)
 *
 * This is the main timer tick function. It:
 * 1. Gets the current time and calculates elapsed time (delta)
 * 2. Subtracts delta from the head timer's remaining time
 * 3. If timer expired (delta_hi:delta_lo <= 0), fires the timer:
 *    - Removes node from queue
 *    - Sends message if queue pointer is set
 *    - Re-inserts if reload value is non-zero (periodic timer)
 * 4. Repeats until no more expired timers
 */
void dll_update(void) {
    TimerQueueHead *q = __osTimerList;
    TimerNode *node;
    u32 current_time;
    u32 elapsed;
    u32 delta_hi, delta_lo;

    /* Check if queue is empty */
    node = q->head;
    if (node == (TimerNode *)q) {
        return;
    }

loop:
    /* Re-check if queue became empty */
    node = q->head;
    if (node == (TimerNode *)q) {
        __osSetCompare(0);
        __osTimerCounter = 0;
        return;
    }

    /* Get current time and calculate elapsed since last update */
    current_time = osGetCount();
    elapsed = current_time - __osTimerCounter;
    __osTimerCounter = current_time;

    /* Check if head timer has time remaining */
    delta_hi = node->delta_hi;
    delta_lo = node->delta_lo;

    /* 64-bit comparison: if delta > elapsed, subtract and reschedule */
    if (delta_hi > 0 || delta_lo >= elapsed) {
        /* Timer not expired yet - subtract elapsed time */
        u32 new_lo = delta_lo - elapsed;
        u32 borrow = (delta_lo < elapsed) ? 1 : 0;
        u32 new_hi = delta_hi - borrow;

        node->delta_lo = new_lo;
        node->delta_hi = new_hi;

        /* Reschedule based on remaining time */
        dll_reschedule(node->delta_hi, node->delta_lo);
        return;
    }

    /* Timer expired - remove from queue */
    /* Unlink: prev->next = node->next, next->prev = node->prev */
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = NULL;
    node->prev = NULL;

    /* Send message if message queue is set */
    if (node->mq != NULL) {
        osJamMesg(node->mq, node->msg, 0);
    }

    /* Check if this is a periodic timer (reload != 0) */
    if (node->reload_hi != 0 || node->reload_lo != 0) {
        /* Reload the timer and re-insert */
        node->delta_hi = node->reload_hi;
        node->delta_lo = node->reload_lo;
        dll_insert(node);
    }

    /* Continue processing - there may be more expired timers */
    goto loop;
}

/**
 * Reschedule timer interrupt
 * (func_8000C294)
 *
 * Called to update the hardware timer based on the next expiry time.
 *
 * @param hi High 32 bits of time until next timer
 * @param lo Low 32 bits of time until next timer
 */
void dll_reschedule(u32 hi, u32 lo) {
    s32 savedMask;
    u32 current_time;
    u32 wake_time_hi, wake_time_lo;

    savedMask = __osDisableInt();

    current_time = osGetCount();
    __osTimerCounter = current_time;

    /* Calculate absolute wake time: current + delta */
    wake_time_lo = current_time + lo;
    /* Handle carry */
    wake_time_hi = (wake_time_lo < lo) ? 1 : 0;
    wake_time_hi += hi;

    /* Set hardware timer (CP0 Compare register) */
    __osSetCompare(wake_time_lo);

    __osRestoreInt(savedMask);
}

/**
 * Insert a timer node into the delta queue
 * (func_8000C308)
 *
 * Inserts the node in sorted order by expiry time.
 * Adjusts delta values of surrounding nodes accordingly.
 *
 * @param node Timer node to insert
 */
void dll_insert(TimerNode *node) {
    s32 savedMask;
    TimerQueueHead *q = __osTimerList;
    TimerNode *curr;
    u32 delta_hi, delta_lo;
    u32 node_hi, node_lo;
    u32 curr_hi, curr_lo;
    u32 borrow;

    savedMask = __osDisableInt();

    curr = q->head;
    node_hi = node->delta_hi;
    node_lo = node->delta_lo;

    /* Find insertion point - walk list while our delta > current delta */
    while (curr != (TimerNode *)q) {
        curr_hi = curr->delta_hi;
        curr_lo = curr->delta_lo;

        /* 64-bit comparison: if node_delta < curr_delta, insert here */
        if (node_hi < curr_hi ||
            (node_hi == curr_hi && node_lo < curr_lo)) {
            break;
        }

        /* Subtract current node's delta from ours */
        borrow = (node_lo < curr_lo) ? 1 : 0;
        node_lo -= curr_lo;
        node_hi -= curr_hi + borrow;

        curr = curr->next;
    }

    /* Store adjusted delta */
    node->delta_hi = node_hi;
    node->delta_lo = node_lo;

    /* If not inserting at end, adjust next node's delta */
    if (curr != (TimerNode *)q) {
        borrow = (curr->delta_lo < node_lo) ? 1 : 0;
        curr->delta_hi -= node_hi + borrow;
        curr->delta_lo -= node_lo;
    }

    /* Insert node before curr */
    node->next = curr;
    node->prev = curr->prev;
    curr->prev->next = node;
    curr->prev = node;

    __osRestoreInt(savedMask);
}

/**
 * Get thread priority (helper)
 * (func_8000C490)
 *
 * @param thread Thread to query, or NULL for current thread
 * @return Thread priority
 */
s32 dll_get_priority(void *thread) {
    extern void *__osRunningThread;  /* Current running thread: 0x8002C3E0 */

    if (thread == NULL) {
        thread = __osRunningThread;
    }
    /* Return offset 0x04 in thread structure (priority) */
    return *(s32 *)((u8 *)thread + 4);
}
