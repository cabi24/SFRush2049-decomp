/*
 * Function: dma_signal
 * Address:  0x8000262C
 * Category: boot
 * Status:   WIP
 *
 * Signal DMA complete by sending message to DMA queue.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

typedef void *OSMesgQueue;
typedef void *OSMesg;

extern OSMesgQueue D_8002F190;  /* DMA completion queue */
extern s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);

void dma_signal(void) {
    osJamMesg(&D_8002F190, NULL, 0);
}
