/* GENERATED ROM-aligned TU — segment 0x2cf0 (rom/lib_2cf0)
 * layout map 2fee9198d70b169d9db10078865019d23807b48148a346fda9e88851da0e576b; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "static_debug_context.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_2cf0/main.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_2cf0/idle_thread_entry.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_2cf0/game_init.s")
/* PROMOTED 2026-10-02 — audio_thread_entry
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/audio_thread_entry.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/audio_thread_entry.c:audio_thread_entry (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void audio_thread_entry(s32 arg) { __setfpcsr(0x01000E00); while(1) { game_loop(); } }

