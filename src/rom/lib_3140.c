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

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_3140/dma_wait.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_3140/dma_signal.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_3140/lzss_decompress.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_3140/inflate_decompress.s")
