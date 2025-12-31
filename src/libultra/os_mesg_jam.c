/**
 * @file os_mesg_jam.c
 * @brief Message queue jam (high priority send) functions
 *
 * Decompiled from asm/us/8040.s
 */

#include "types.h"
#include "PR/os_thread.h"
#include "PR/os_message.h"

/* External functions */
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
extern OSThread *__osPopThread(OSThread **queue);
extern void osStartThread(OSThread *thread);
extern void __osEnqueueAndYield(OSThread **queue);

/* Uses __osRunningThread from os_thread.h */

/**
 * Jam message to front of queue
 * (0x80007440 - osJamMesg)
 *
 * Sends a message to the front of a message queue (high priority).
 * If the queue is full and OS_MESG_BLOCK is specified, blocks until space.
 *
 * @param mq Message queue
 * @param msg Message to send
 * @param flags OS_MESG_NOBLOCK or OS_MESG_BLOCK
 * @return 0 on success, -1 if queue full (non-blocking)
 */
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags) {
    s32 saved;
    s32 index;
    OSThread *thread;

    saved = __osDisableInt();

    /* Check if queue is full */
    while (mq->validCount >= mq->msgCount) {
        if (flags == OS_MESG_BLOCK) {
            /* Block - put current thread to sleep */
            __osRunningThread->state = OS_STATE_WAITING;
            __osEnqueueAndYield(&mq->fullqueue);
        } else {
            /* Non-blocking - return error */
            __osRestoreInt(saved);
            return -1;
        }
    }

    /* Calculate index for front insertion */
    index = (mq->first + mq->msgCount - 1 + mq->validCount) % mq->msgCount;
    mq->first = index;

    /* Store message */
    mq->msg[index] = msg;
    mq->validCount++;

    /* Wake up any waiting receiver */
    if (mq->mtqueue != NULL) {
        thread = __osPopThread(&mq->mtqueue);
        osStartThread(thread);
    }

    __osRestoreInt(saved);
    return 0;
}

/**
 * Set thread message queue
 * (0x80007590 - __osSetThreadMesgQueue)
 *
 * Associates a message queue with the current thread.
 *
 * @param mq Message queue to associate
 */
void __osSetThreadMesgQueue(OSMesgQueue *mq) {
    s32 saved;

    saved = __osDisableInt();

    /* Store queue in thread structure */
    __osRunningThread->priority = (s32)mq;

    /* Set flag bit 4 */
    *(u16 *)__osRunningThread |= 0x10;

    __osRestoreInt(saved);
}
