/**
 * @file os_message.c
 * @brief N64 OS message queue functions
 *
 * Decompiled from asm/us/7600.s
 */

#include "PR/os_message.h"
#include "PR/os_thread.h"

/* Empty thread queue sentinel */
extern OSThread *__osThreadQueue;

/**
 * Initialize a message queue
 * @param mq Pointer to message queue structure
 * @param msg Pointer to message buffer array
 * @param count Number of messages the queue can hold
 */
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count) {
    mq->mtqueue = (OSThread *)&__osThreadQueue;
    mq->fullqueue = (OSThread *)&__osThreadQueue;
    mq->validCount = 0;
    mq->first = 0;
    mq->msgCount = count;
    mq->msg = msg;
}

/* External OS functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32);
extern void __osEnqueueAndYield(OSThread **);
extern OSThread *__osPopThread(OSThread **);
extern void osStartThread(OSThread *);
extern OSThread *__osRunningThread;

/**
 * Send a message to a queue
 * @param mq Message queue
 * @param msg Message to send
 * @param flags OS_MESG_NOBLOCK or OS_MESG_BLOCK
 * @return 0 on success, -1 if queue full and non-blocking
 */
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags) {
    s32 savedMask;
    s32 index;

    savedMask = __osDisableInt();

    while (mq->validCount >= mq->msgCount) {
        if (flags == OS_MESG_BLOCK) {
            __osRunningThread->state = OS_STATE_WAITING;
            __osEnqueueAndYield(&mq->fullqueue);
        } else {
            __osRestoreInt(savedMask);
            return -1;
        }
    }

    /* Calculate index for new message (circular buffer) */
    index = (mq->first + mq->validCount) % mq->msgCount;
    mq->msg[index] = msg;
    mq->validCount++;

    /* Wake up any thread waiting to receive */
    if (mq->mtqueue->next != NULL) {
        osStartThread(__osPopThread(&mq->mtqueue));
    }

    __osRestoreInt(savedMask);
    return 0;
}

/**
 * Receive a message from a queue
 * @param mq Message queue
 * @param msg Pointer to store received message (can be NULL)
 * @param flags OS_MESG_NOBLOCK or OS_MESG_BLOCK
 * @return 0 on success, -1 if queue empty and non-blocking
 */
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags) {
    s32 savedMask;

    savedMask = __osDisableInt();

    while (mq->validCount == 0) {
        if (flags == OS_MESG_NOBLOCK) {
            __osRestoreInt(savedMask);
            return -1;
        }
        __osRunningThread->state = OS_STATE_WAITING;
        __osEnqueueAndYield(&mq->mtqueue);
    }

    /* Copy message if pointer provided */
    if (msg != NULL) {
        *msg = mq->msg[mq->first];
    }

    /* Advance circular buffer */
    mq->first = (mq->first + 1) % mq->msgCount;
    mq->validCount--;

    /* Wake up any thread waiting to send */
    if (mq->fullqueue->next != NULL) {
        osStartThread(__osPopThread(&mq->fullqueue));
    }

    __osRestoreInt(savedMask);
    return 0;
}
