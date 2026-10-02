/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
extern s8 gDmaInitialized;
extern OSMesgQueue gDmaMessageQueue;
extern void dma_queue_init(void), dma_signal(void);
extern s32 dma_wait(s32);
extern s32 lzss_decode(void*,void*), inflate_entry(void*,void*,s32);
s32 inflate_decompress(void* src,void* dst,s32 n) { s32 result; if(!dma_wait(1)) return 0; result=inflate_entry(src,dst,n); dma_signal(); return result; }
