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
#include "PR/os_message.h"

/* External data - Sync lock state */
extern u8 __osSyncInitialized;      /* Lock initialized flag */
extern OSMesgQueue __osSyncQueue;   /* Lock message queue */
extern OSMesg __osSyncMsgBuf;       /* Lock message buffer */

/* Protected operation functions */
extern s32 __osSyncProtectedOp(void *arg0, void *arg1);

/* Forward declarations */
void sync_init(void);
s32 sync_acquire(s32 blocking);
void sync_release(void);

/**
 * Initialize the synchronization lock
 * (0x80002540)
 *
 * Creates a message queue with one slot and posts an initial
 * message to indicate the lock is available.
 */
/* MATCHED: 0x80002540 */
void sync_init(void) {
    __osSyncInitialized = 1;

    /* Create message queue with 1 slot */
    osCreateMesgQueue(&__osSyncQueue, &__osSyncMsgBuf, 1);

    /* Post initial message to indicate lock is free */
    osSendMesg(&__osSyncQueue, NULL, OS_MESG_NOBLOCK);
}

/**
 * Acquire the synchronization lock
 * (0x80002598)
 *
 * @param blocking If non-zero, block until lock is acquired.
 *                 If zero, return immediately if lock is unavailable.
 * @return 1 if lock was acquired, 0 if not (only when non-blocking)
 */
/* MATCHED: 0x80002598 */
s32 sync_acquire(s32 blocking) {
    OSMesg msg;

    /* Initialize if not already done */
    if (__osSyncInitialized == 0) {
        sync_init();
    }

    if (blocking != 0) {
        /* Blocking acquire - wait for message */
        osRecvMesg(&__osSyncQueue, &msg, OS_MESG_BLOCK);
    } else {
        /* Non-blocking acquire - try to get message */
        if (osRecvMesg(&__osSyncQueue, &msg, OS_MESG_NOBLOCK) == -1) {
            return 0;  /* Lock not available */
        }
    }

    return 1;  /* Lock acquired */
}

/**
 * Release the synchronization lock
 * (0x8000262C)
 *
 * Posts a message to indicate the lock is now free.
 */
/* MATCHED: 0x8000262C */
void sync_release(void) {
    osSendMesg(&__osSyncQueue, NULL, OS_MESG_NOBLOCK);
}

/**
 * Execute operation with lock held
 * (0x80002660)
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
    result = __osSyncProtectedOp(arg0, arg1);

    /* Release lock */
    sync_release();

    return result;
}

/* Protected operation with 3 arguments */
extern s32 __osSyncProtectedOp3(void *arg0, void *arg1, void *arg2);

/**
 * Execute operation with three arguments while holding lock
 * (0x800026C0)
 *
 * @param arg0 First argument for protected operation
 * @param arg1 Second argument for protected operation
 * @param arg2 Third argument for protected operation
 * @return Result from protected operation, or 0 if lock not acquired
 */
s32 sync_execute3(void *arg0, void *arg1, void *arg2) {
    s32 result;

    /* Acquire lock (blocking) */
    if (sync_acquire(1) == 0) {
        return 0;
    }

    /* Execute protected operation */
    result = __osSyncProtectedOp3(arg0, arg1, arg2);

    /* Release lock */
    sync_release();

    return result;
}
