/* GENERATED ROM-aligned TU — segment 0x3140 (rom/lib_3140)
 * layout map a29a2c188301e27d34ee1904853343fcdf197c4de42725a016014b9cb43b5b7f; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "static_debug_context.h"

/* PROMOTED 2026-10-02 — dma_queue_init
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/dma_queue_init.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/dma_queue_init.c:dma_queue_init (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void dma_queue_init(void) { gDmaInitialized=1; osCreateMesgQueue(&gDmaMessageQueue,&gDmaMessageBuffer,1); osJamMesg(&gDmaMessageQueue,NULL,0); }

/* PROMOTED 2026-10-02 — dma_wait
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/dma_wait.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/dma_wait.c:dma_wait (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 dma_wait(s32 block) { OSMesg msg; if (!gDmaInitialized) dma_queue_init(); if(block) { osRecvMesg(&gDmaMessageQueue,&msg,OS_MESG_BLOCK); } else { if(osRecvMesg(&gDmaMessageQueue,&msg,OS_MESG_NOBLOCK)==-1) return 0; } return 1; }

/* PROMOTED 2026-10-02 — dma_signal
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/dma_signal.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/dma_signal.c:dma_signal (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void dma_signal(void) {
    osJamMesg(&gDmaMessageQueue, NULL, 0);
}

/* PROMOTED 2026-10-02 — lzss_decompress
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/lzss_decompress.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/lzss_decompress.c:lzss_decompress (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 lzss_decompress(void* src,void* dst) { s32 result; if(!dma_wait(1)) return 0; result=lzss_decode(src,dst); dma_signal(); return result; }

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_3140/inflate_decompress.s")
