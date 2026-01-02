/*
 * Function: osJamMesg
 * Address:  0x800075E0
 * Category: libultra/os
 * Status:   WIP
 *
 * Insert message at front of queue (high priority).
 * Similar to osSendMesg but inserts at front instead of back.
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

#define OS_MESG_NOBLOCK 0
#define OS_MESG_BLOCK   1
#define OS_STATE_WAITING 8

typedef void *OSMesg;

typedef struct OSThread_s {
    struct OSThread_s *next;
    u16 state;
} OSThread;

typedef struct {
    OSThread *mtqueue;
    OSThread *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

extern OSThread *D_8002C3E0;

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32 mask);
extern void __osEnqueueAndYield(OSThread **queue);
extern OSThread *__osPopThread(OSThread **queue);
extern void osStartThread(OSThread *thread);

s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag) {
    u32 saveMask;
    s32 index;
    OSThread *thread;

    saveMask = __osDisableInt();

    /* Wait for space in queue */
    while (mq->validCount >= mq->msgCount) {
        if (flag == OS_MESG_BLOCK) {
            D_8002C3E0->state = OS_STATE_WAITING;
            __osEnqueueAndYield(&mq->fullqueue);
        } else {
            __osRestoreInt(saveMask);
            return -1;
        }
    }

    /* Calculate write index at end of queue */
    index = (mq->first + mq->validCount) % mq->msgCount;

    /* Store message */
    mq->msg[index] = msg;
    mq->validCount++;

    /* Wake up any waiting receiver */
    thread = mq->mtqueue->next;
    if (thread != NULL) {
        thread = __osPopThread(&mq->mtqueue);
        osStartThread(thread);
    }

    __osRestoreInt(saveMask);
    return 0;
}
