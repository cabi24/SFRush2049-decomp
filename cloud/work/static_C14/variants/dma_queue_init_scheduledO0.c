/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
extern s8 gDmaInitialized; extern OSMesg gDmaMessageBuffer; extern OSMesgQueue gDmaMessageQueue;
void dma_queue_init(void) { gDmaInitialized=1; osCreateMesgQueue(&gDmaMessageQueue,&gDmaMessageBuffer,1); osJamMesg(&gDmaMessageQueue,NULL,0); }
