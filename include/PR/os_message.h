/**
 * @file os_message.h
 * @brief N64 OS message queue definitions
 */

#ifndef _OS_MESSAGE_H_
#define _OS_MESSAGE_H_

#include "types.h"
#include "PR/os_thread.h"

/* Message type */
typedef void *OSMesg;

/* Message queue structure */
typedef struct OSMesgQueue_s {
    OSThread    *mtqueue;       /* Queue for threads blocked on receive */
    OSThread    *fullqueue;     /* Queue for threads blocked on send */
    s32         validCount;     /* Number of valid messages */
    s32         first;          /* Index of first valid message */
    s32         msgCount;       /* Total capacity */
    OSMesg      *msg;           /* Message buffer array */
} OSMesgQueue;

/* Message queue flags */
#define OS_MESG_NOBLOCK     0
#define OS_MESG_BLOCK       1

/* Function prototypes */
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);

#endif /* _OS_MESSAGE_H_ */
