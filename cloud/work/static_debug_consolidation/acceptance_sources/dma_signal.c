/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
void dma_signal(void) {
    osJamMesg(&gDmaMessageQueue, NULL, 0);
}
