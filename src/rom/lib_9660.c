/* GENERATED ROM-aligned TU — segment 0x9660 (rom/lib_9660)
 * layout map 98ded2baa087f2258440a099641ffc555a5a39a13d2e01cdf20194b1e3769ee1; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — guOrthoF
 * Source:   cloud/work/static_C6/guOrthoF.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_C6/guOrthoF.c:guOrthoF (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void guOrthoF(float mf[4][4], float l, float r, float b, float t, float n, float f, float scale)
{
	int	i, j;

	guMtxIdentF(mf);

	mf[0][0] = 2/(r-l);
	mf[1][1] = 2/(t-b);
	mf[2][2] = -2/(f-n);
	mf[3][0] = -(r+l)/(r-l);
	mf[3][1] = -(t+b)/(t-b);
	mf[3][2] = -(f+n)/(f-n);
	mf[3][3] = 1;

	for (i=0; i<4; i++)
	    for (j=0; j<4; j++)
		mf[i][j] *= scale;
}

/* PROMOTED 2026-10-01 — guOrtho
 * Source:   cloud/work/static_C3/guOrtho.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C3/guOrtho.c:guOrtho (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void guOrtho(Mtx *m, float l, float r, float b, float t, float n, float f, float scale)
{
	Matrix	mf;

	guOrthoF(mf, l, r, b, t, n, f, scale);

	guMtxF2L(mf, m);
}

