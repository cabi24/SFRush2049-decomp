/* GENERATED ROM-aligned TU — segment 0x9de0 (rom/lib_9de0)
 * layout map f4d21241ed69e607619775287abc7b113e3dd26a0a988799dd883df6aa3b91a7; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_9de0/guMtxF2L.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_9de0/guMtxL2F.s")
/* PROMOTED 2026-07-11 — guMtxIdentF
 * Source:   src/game/matrix.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/game/matrix.c:guMtxIdentF (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void guMtxIdentF(f32 mtx[4][4]) {
    s32 i, j;
    f32 one = 1.0f;
    f32 zero = 0.0f;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == j) {
                mtx[i][j] = one;
            } else {
                mtx[i][j] = zero;
            }
        }
    }
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_9de0/guMtxIdent.s")
