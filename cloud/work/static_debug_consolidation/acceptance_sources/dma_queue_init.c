/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
void dma_queue_init(void) { gDmaInitialized=1; osCreateMesgQueue(&gDmaMessageQueue,&gDmaMessageBuffer,1); osJamMesg(&gDmaMessageQueue,NULL,0); }
