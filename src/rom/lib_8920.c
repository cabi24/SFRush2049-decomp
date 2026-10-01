/* GENERATED ROM-aligned TU — segment 0x8920 (rom/lib_8920)
 * layout map b9a70b91b06cedf61b120acb4d15a522280da2ddf26eeabd90d3c33d1579274e; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osViSetSpecialFeatures
 * Source:   cloud/work/static_C/osViSetSpecialFeatures.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/osViSetSpecialFeatures.c:osViSetSpecialFeatures (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void osViSetSpecialFeatures(u32 features) {
    u32 temp_a1;

    temp_a1 = __osDisableInt();
    if (features & 1) {
        __osViContext->control |= 8;
    }
    if (features & 2) {
        __osViContext->control &= ~8;
    }
    if (features & 4) {
        __osViContext->control |= 4;
    }
    if (features & 8) {
        __osViContext->control &= ~4;
    }
    if (features & 0x10) {
        __osViContext->control |= 0x10;
    }
    if (features & 0x20) {
        __osViContext->control &= ~0x10;
    }
    if (features & 0x40) {
        __osViContext->control |= 0x10000;
        __osViContext->control &= ~0x300;
    }
    if (features & 0x80) {
        __osViContext->control &= 0xFFFEFFFF;
        __osViContext->control |= __osViContext->modep->comRegs.ctrl & 0x300;
    }
    __osViContext->state |= 8;
    __osRestoreInt(temp_a1);
}

