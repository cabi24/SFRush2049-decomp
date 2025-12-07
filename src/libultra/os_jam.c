/**
 * @file os_jam.c
 * @brief Message queue jam (insert at front) function
 *
 * Decompiled from asm/us/81E0.s
 * Contains osJamMesg which inserts a message at the front of a queue
 */

#include "types.h"

/* Forward declarations */
struct OSThread;

/* Message queue structure */
typedef struct OSMesgQueue_s {
    struct OSThread *mtqueue;      /* 0x00: Threads waiting to receive */
    struct OSThread *fullqueue;    /* 0x04: Threads waiting to send */
    s32 validCount;                /* 0x08: Number of valid messages */
    s32 first;                     /* 0x0C: Index of first valid message */
    s32 msgCount;                  /* 0x10: Maximum messages in queue */
    void **msg;                    /* 0x14: Message buffer array */
} OSMesgQueue;

/* Thread structure (partial - for state access) */
typedef struct OSThread {
    struct OSThread *next;  /* 0x00: Next thread in queue */
    s32 priority;           /* 0x04: Thread priority */
    void *queue;            /* 0x08: Queue this thread belongs to */
    s16 state;              /* 0x10: Thread state */
} OSThread;

/* External data */
extern OSThread *D_8002C3E0;    /* Currently running thread */

/* External functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
extern void __osEnqueueAndYield(OSThread **queue);
extern OSThread *__osPopThread(OSThread **queue);
extern void osStartThread(OSThread *thread);

/**
 * Insert message at front of queue
 * (func_800075E0 - osJamMesg)
 *
 * Like osSendMesg, but inserts at the front of the queue rather
 * than the back. This gives the message higher priority.
 *
 * @param mq Message queue
 * @param msg Message to insert
 * @param flags OS_MESG_NOBLOCK or OS_MESG_BLOCK
 * @return 0 on success, -1 if queue full and non-blocking
 */
s32 osJamMesg(OSMesgQueue *mq, void *msg, s32 flags) {
    s32 savedMask;
    s32 index;

    savedMask = __osDisableInt();

    /* Wait/fail if queue is full */
    while (mq->validCount >= mq->msgCount) {
        if (flags == 1) {  /* OS_MESG_BLOCK */
            D_8002C3E0->state = 8;  /* OS_STATE_WAITING */
            __osEnqueueAndYield(&mq->fullqueue);
        } else {
            __osRestoreInt(savedMask);
            return -1;
        }
    }

    /* Calculate index for front of queue (one before first) */
    /* Handles wrap-around: (first + msgCount - 1) % msgCount */
    index = (mq->first + mq->validCount + mq->msgCount - 1) % mq->msgCount;

    /* Actually need to calculate position before first */
    /* Move first back by one (with wrap) */
    index = (mq->first + mq->msgCount - 1) % mq->msgCount;
    mq->first = index;
    mq->msg[index] = msg;
    mq->validCount++;

    /* Wake up any thread waiting to receive */
    if (mq->mtqueue->next != NULL) {
        osStartThread(__osPopThread(&mq->mtqueue));
    }

    __osRestoreInt(savedMask);
    return 0;
}
