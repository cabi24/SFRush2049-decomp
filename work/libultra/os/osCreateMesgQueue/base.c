/*
 * Function: osCreateMesgQueue
 * Address:  0x80006A00
 * Category: libultra/os
 * Status:   WIP
 *
 * Initialize a message queue structure.
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

typedef void *OSMesg;

typedef struct OSThread_s {
    struct OSThread_s *next;
} OSThread;

typedef struct {
    OSThread *mtqueue;   /* 0x00: threads waiting to send */
    OSThread *fullqueue; /* 0x04: threads waiting to receive */
    s32 validCount;      /* 0x08: current message count */
    s32 first;           /* 0x0C: index of first message */
    s32 msgCount;        /* 0x10: queue capacity */
    OSMesg *msg;         /* 0x14: message array */
} OSMesgQueue;

/* Empty queue sentinel */
extern OSThread D_8002C3D0;

void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 msgCount) {
    mq->mtqueue = &D_8002C3D0;
    mq->fullqueue = &D_8002C3D0;
    mq->validCount = 0;
    mq->first = 0;
    mq->msgCount = msgCount;
    mq->msg = msg;
}
