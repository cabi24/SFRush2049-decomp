/**
 * @file os_sync.c
 * @brief Synchronization primitives (mutex-like)
 *
 * Decompiled from asm/us/3140.s
 * Contains functions for mutual exclusion using message queues
 *
 * This implements a simple mutex/lock mechanism using osRecvMesg/osSendMesg.
 * A message in the queue means the lock is free; empty queue means locked.
 */

#include "types.h"

/* External data */
extern u8 D_8002B030;               /* Lock initialized flag */
extern u8 D_8002F190[24];           /* Lock message queue (OSMesgQueue) */
extern u8 D_8002F1A8[4];            /* Lock message buffer */

/* External functions */
extern void osCreateMesgQueue(void *mq, void *msgBuf, s32 count);
extern s32 osSendMesg(void *mq, s32 msg, s32 flags);
extern s32 osRecvMesg(void *mq, s32 *msg, s32 flags);
extern s32 func_80004AFC(void *arg0, void *arg1);  /* Protected operation */

/* Forward declarations */
void sync_init(void);
s32 sync_acquire(s32 blocking);
void sync_release(void);

/**
 * Initialize the synchronization lock
 * (func_80002540)
 *
 * Creates a message queue with one slot and posts an initial
 * message to indicate the lock is available.
 */
void sync_init(void) {
    D_8002B030 = 1;

    /* Create message queue with 1 slot */
    osCreateMesgQueue(D_8002F190, D_8002F1A8, 1);

    /* Post initial message to indicate lock is free */
    osSendMesg(D_8002F190, 0, 0);
}

/**
 * Acquire the synchronization lock
 * (func_80002598)
 *
 * @param blocking If non-zero, block until lock is acquired.
 *                 If zero, return immediately if lock is unavailable.
 * @return 1 if lock was acquired, 0 if not (only when non-blocking)
 */
s32 sync_acquire(s32 blocking) {
    s32 msg;

    /* Initialize if not already done */
    if (D_8002B030 == 0) {
        sync_init();
    }

    if (blocking != 0) {
        /* Blocking acquire - wait for message */
        osRecvMesg(D_8002F190, &msg, 1);
    } else {
        /* Non-blocking acquire - try to get message */
        if (osRecvMesg(D_8002F190, &msg, 0) == -1) {
            return 0;  /* Lock not available */
        }
    }

    return 1;  /* Lock acquired */
}

/**
 * Release the synchronization lock
 * (func_8000262C)
 *
 * Posts a message to indicate the lock is now free.
 */
void sync_release(void) {
    osSendMesg(D_8002F190, 0, 0);
}

/**
 * Execute operation with lock held
 * (func_80002660)
 *
 * Acquires lock, executes operation, releases lock.
 *
 * @param arg0 First argument for protected operation
 * @param arg1 Second argument for protected operation
 * @return Result from protected operation, or 0 if lock not acquired
 */
s32 sync_execute(void *arg0, void *arg1) {
    s32 result;

    /* Acquire lock (blocking) */
    if (sync_acquire(1) == 0) {
        return 0;
    }

    /* Execute protected operation */
    result = func_80004AFC(arg0, arg1);

    /* Release lock */
    sync_release();

    return result;
}

/**
 * Execute operation with three arguments while holding lock
 * (func_800026C0)
 *
 * @param arg0 First argument for protected operation
 * @param arg1 Second argument for protected operation
 * @param arg2 Third argument for protected operation
 * @return Result from protected operation, or 0 if lock not acquired
 */
extern s32 func_80006814(void *arg0, void *arg1, void *arg2);  /* 3-arg protected op */

s32 sync_execute3(void *arg0, void *arg1, void *arg2) {
    s32 result;

    /* Acquire lock (blocking) */
    if (sync_acquire(1) == 0) {
        return 0;
    }

    /* Execute protected operation */
    result = func_80006814(arg0, arg1, arg2);

    /* Release lock */
    sync_release();

    return result;
}
