#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
void dma_queue_init(void) { gDmaInitialized=1; osCreateMesgQueue(&gDmaMessageQueue,&gDmaMessageBuffer,1); osJamMesg(&gDmaMessageQueue,NULL,0); }

#pragma GLOBAL_ASM("build/C68/lib_3140/dma_wait.s")
#pragma GLOBAL_ASM("build/C68/lib_3140/dma_signal.s")
#pragma GLOBAL_ASM("build/C68/lib_3140/lzss_decompress.s")
#pragma GLOBAL_ASM("build/C68/lib_3140/inflate_decompress.s")
