/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "static_debug_context.h"
s32 lzss_decompress(void* src,void* dst) { s32 result; if(!dma_wait(1)) return 0; result=lzss_decode(src,dst); dma_signal(); return result; }
