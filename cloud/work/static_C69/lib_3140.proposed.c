#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesg gDmaMessageBuffer;
extern OSMesgQueue gDmaMessageQueue;
extern s32 lzss_decode(void *,void *);
extern s32 inflate_entry(void *,void *,s32);
typedef struct { unsigned sign:1; unsigned exponent:11; unsigned fraction:20; unsigned low; } DoubleBits;
typedef union { double value; DoubleBits bits; } DoubleUnion;
void dma_queue_init(void) { gDmaInitialized=1; osCreateMesgQueue(&gDmaMessageQueue,&gDmaMessageBuffer,1); osJamMesg(&gDmaMessageQueue,NULL,0); }

s32 dma_wait(s32 block) { OSMesg msg; if (!gDmaInitialized) dma_queue_init(); if(block) { osRecvMesg(&gDmaMessageQueue,&msg,OS_MESG_BLOCK); } else { if(osRecvMesg(&gDmaMessageQueue,&msg,OS_MESG_NOBLOCK)==-1) return 0; } return 1; }

void dma_signal(void) {
    osJamMesg(&gDmaMessageQueue, NULL, 0);
}
/* Warning: struct __OSThreadprofile_s is not defined (only forward-declared) */

s32 lzss_decompress(void* src,void* dst) { s32 result; if(!dma_wait(1)) return 0; result=lzss_decode(src,dst); dma_signal(); return result; }

s32 inflate_decompress(void* src,void* dst,s32 n) { s32 result; if(!dma_wait(1)) return 0; result=inflate_entry(src,dst,n); dma_signal(); return result; }

