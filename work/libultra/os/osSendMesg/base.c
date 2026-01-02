/*
 * Function: osSendMesg
 * Address:  0x80007440
 * Category: libultra/os
 * Status:   WIP
 *
 * Send message to queue. Blocks if queue full and OS_MESG_BLOCK specified.
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK   1

typedef void *OSMesg;

/* Thread state: blocked waiting for message queue space */
#define OS_STATE_WAITING 8

typedef struct OSThread_s {
    struct OSThread_s *next;
    /* ... other fields ... */
    u16 state;  /* offset 0x10 */
} OSThread;

typedef struct {
    OSThread *mtqueue;   /* 0x00: threads waiting to send (queue full) */
    OSThread *fullqueue; /* 0x04: threads waiting to receive (queue empty) */
    s32 validCount;      /* 0x08: messages currently in queue */
    s32 first;           /* 0x0C: index of first message */
    s32 msgCount;        /* 0x10: queue capacity */
    OSMesg *msg;         /* 0x14: message array pointer */
} OSMesgQueue;

extern OSThread *D_8002C3E0;  /* __osRunningThread */

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);
extern void __osEnqueueAndYield(OSThread **queue);
extern OSThread *__osPopThread(OSThread **queue);
extern void osStartThread(OSThread *thread);

s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag) {
    u32 saveMask;
    s32 index;
    OSThread *thread;

    saveMask = __osDisableInt();

    /* If queue is full */
    while (mq->validCount >= mq->msgCount) {
        if (flag == OS_MESG_BLOCK) {
            /* Block: set state and enqueue on mtqueue */
            D_8002C3E0->state = OS_STATE_WAITING;
            __osEnqueueAndYield(&mq->mtqueue);
        } else {
            /* Non-blocking: return error */
            __osRestoreInt(saveMask);
            return -1;
        }
    }

    /* Calculate write index using modulo (circular buffer) */
    index = (mq->first + mq->validCount - 1) % mq->msgCount;
    mq->first = index;

    /* Store message and increment count */
    mq->msg[mq->first] = msg;
    mq->validCount++;

    /* Wake up any thread waiting for messages */
    thread = mq->fullqueue->next;
    if (thread != NULL) {
        thread = __osPopThread(&mq->fullqueue);
        osStartThread(thread);
    }

    __osRestoreInt(saveMask);
    return 0;
}
