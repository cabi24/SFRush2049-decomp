/*
 * Function: __osSiRelAccess
 * Address:  0x8000E5F4
 * Category: libultra/si
 * Status:   WIP
 *
 * Release SI mutex by sending message to SI semaphore queue
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

typedef void *OSMesgQueue;
typedef void *OSMesg;

extern OSMesgQueue __osSiMesg;  /* SI semaphore queue at 0x80037C98 */
extern s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);

void __osSiRelAccess(void) {
    osJamMesg(&__osSiMesg, NULL, 0);
}
