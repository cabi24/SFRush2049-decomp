/* GENERATED ROM-aligned TU — segment 0x2cf0 (rom/lib_2cf0)
 * layout map 2fee9198d70b169d9db10078865019d23807b48148a346fda9e88851da0e576b; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "static_debug_context.h"

/* PROMOTED 2026-10-02 — main
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/main.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/main.c:main (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void main(void *argument) {
    u32 index;
    u32 address;
    u32 header[16];
    __osInitialize_common();
    address = 0xFFB000;
    for (index = 0; index < 16; index++, address += 4) {
        osPiRawReadWord(address, &header[index]);
    }
    osCreateThread(&gIdleThread, 1, idle_thread_entry, argument, gStackIdle + 0x190, 2);
    osStartThread(&gIdleThread);
}

/* PROMOTED 2026-10-02 — idle_thread_entry
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/idle_thread_entry.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/idle_thread_entry.c:idle_thread_entry (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void idle_thread_entry(void *argument) {
    osCreatePiManager(150, &gViModeTable, gViModeLan1, 200);
    osCreateViManager(NULL, 0);
    osCreateThread(&gGameThread, 6, game_init, argument, gStackGame + 0x960, 4);
    osStartThread(&gGameThread);
    for (;;) {}
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_2cf0/game_init.s")
/* PROMOTED 2026-10-02 — audio_thread_entry
 * Source:   cloud/work/static_debug_consolidation/acceptance_sources/audio_thread_entry.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_debug_consolidation/acceptance_sources/audio_thread_entry.c:audio_thread_entry (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void audio_thread_entry(s32 arg) { __setfpcsr(0x01000E00); while(1) { game_loop(); } }

