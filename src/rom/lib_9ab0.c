/* GENERATED ROM-aligned TU — segment 0x9ab0 (rom/lib_9ab0)
 * layout map 74e9a46a95093a4d531d2536f3f2429d20724fbd0dab51520f62aba35978f9d3; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_9ab0/guLookAtF.s")
/* PROMOTED 2026-10-01 — guLookAt
 * Source:   cloud/work/static_C3/guLookAt.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C3/guLookAt.c:guLookAt (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void guLookAt (Mtx *m, float xEye, float yEye, float zEye,
	       float xAt,  float yAt,  float zAt,
	       float xUp,  float yUp,  float zUp)
{
	Matrix	mf;

	guLookAtF(mf, xEye, yEye, zEye, xAt, yAt, zAt, xUp, yUp, zUp);

	guMtxF2L(mf, m);
}

