/* GENERATED ROM-aligned TU — segment 0xe9a0 (rom/lib_e9a0)
 * layout map 3babb96e2ff8b36483524b7acb5405673e5c98d295075fdfac8ed2bdda2fb775; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_e9a0/osPiSetDeviceTiming.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_e9a0/osSpTaskLoad_full.s")
/* PROMOTED 2026-07-15 — __osInsertTimer
 * Source:   src/rom_auto/__osInsertTimer.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/rom_auto/__osInsertTimer.c:__osInsertTimer (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osInsertTimer(void) {
    if (__osPiMgrState.flag == 0) {
        return 0;
    }
    return __osPiMgrState.unk8;
}

