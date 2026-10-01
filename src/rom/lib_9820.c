/* GENERATED ROM-aligned TU — segment 0x9820 (rom/lib_9820)
 * layout map 0806dc8229adaa12b44b3553635fee1406bb8528ea5161fe0c509c842dfa164a; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_9820/guPerspectiveF.s")
/* PROMOTED 2026-10-01 — guPerspective
 * Source:   cloud/work/static_C3/guPerspective.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C3/guPerspective.c:guPerspective (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void guPerspective(Mtx *m, u16 *perspNorm, float fovy, float aspect, float near, float far, float scale)
{
	Matrix	mf;

	guPerspectiveF(mf, perspNorm, fovy, aspect, near, far, scale);

	guMtxF2L(mf, m);
}

