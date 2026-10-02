/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesgQueue gDmaMessageQueue;
extern void dma_queue_init(void), dma_signal(void);
extern s32 dma_wait(s32);
extern s32 lzss_decode(void*,void*), inflate_entry(void*,void*,s32);
s32 dma_wait(s32 block) { OSMesg msg; if (!gDmaInitialized) dma_queue_init(); if(block) { osRecvMesg(&gDmaMessageQueue,&msg,OS_MESG_BLOCK); } else { if(osRecvMesg(&gDmaMessageQueue,&msg,OS_MESG_NOBLOCK)==-1) return 0; } return 1; }
