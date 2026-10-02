/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
s32 dma_wait(s32 block) { OSMesg msg; if (!gDmaInitialized) dma_queue_init(); if(block) { osRecvMesg(&gDmaMessageQueue,&msg,OS_MESG_BLOCK); } else { if(osRecvMesg(&gDmaMessageQueue,&msg,OS_MESG_NOBLOCK)==-1) return 0; } return 1; }
