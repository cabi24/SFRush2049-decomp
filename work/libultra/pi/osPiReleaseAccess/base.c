/*
 * Function: osPiReleaseAccess
 * Address:  0x8000DC54
 * Category: libultra/pi
 * Status:   WIP
 *
 * Release PI mutex by sending message to PI semaphore queue
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

typedef void *OSMesgQueue;
typedef void *OSMesg;

extern OSMesgQueue D_80037C78;  /* PI semaphore queue */
extern s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);

void osPiReleaseAccess(void) {
    osJamMesg(&D_80037C78, NULL, 0);
}
