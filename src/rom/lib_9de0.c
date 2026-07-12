/* GENERATED ROM-aligned TU — segment 0x9de0 (rom/lib_9de0)
 * layout map f4d21241ed69e607619775287abc7b113e3dd26a0a988799dd883df6aa3b91a7; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_9de0/guMtxF2L.s")
/* PROMOTED 2026-07-11 — guMtxL2F
 * Source:   src/game/matrix.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/game/matrix.c:guMtxL2F (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void guMtxL2F(f32 mf[4][4], Mtx *m) {
    s32 i, j;
    u32 e1, e2;
    u32 *ai, *af;
    s32 q1, q2;

    ai = (u32 *)&(*m)[0][0];
    af = (u32 *)&(*m)[2][0];

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            e1 = (*ai & 0xffff0000) | ((*af >> 16) & 0xffff);
            e2 = ((*(ai++) << 16) & 0xffff0000) | (*(af++) & 0xffff);
            q1 = *(s32 *)&e1;
            q2 = *(s32 *)&e2;

            mf[i][j * 2] = (f32)q1 * (1.0f / (f32)0x00010000);
            mf[i][j * 2 + 1] = (f32)q2 * (1.0f / (f32)0x00010000);
        }
    }
}

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

/* PROMOTED 2026-07-12 — guMtxIdent
 * Source:   src/game/matrix.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/game/matrix.c:guMtxIdent (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void guMtxIdent(u16 *mtx) {
    f32 temp[4][4];

    guMtxIdentF(temp);
    guMtxF2L(temp, mtx);
}

