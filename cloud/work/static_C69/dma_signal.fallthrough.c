/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
extern OSMesgQueue gDmaMessageQueue;
void dma_signal(void) {
    osJamMesg(&gDmaMessageQueue, NULL, 0);
}
/* Warning: struct __OSThreadprofile_s is not defined (only forward-declared) */
