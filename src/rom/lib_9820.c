/* GENERATED ROM-aligned TU — segment 0x9820 (rom/lib_9820)
 * layout map 0806dc8229adaa12b44b3553635fee1406bb8528ea5161fe0c509c842dfa164a; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — guPerspectiveF
 * Source:   cloud/work/static_C6/guPerspectiveF.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_C6/guPerspectiveF.c:guPerspectiveF (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void guPerspectiveF(float mf[4][4], u16 *perspNorm, float fovy, float aspect, float near, float far, float scale)
{
	float	cot;
	int	i, j;

	guMtxIdentF(mf);

	fovy *= gOrthoScale;
	cot = cosf (fovy/2) / sinf (fovy/2);

	mf[0][0] = cot / aspect;
	mf[1][1] = cot;
	mf[2][2] = (near + far) / (near - far);
	mf[2][3] = -1;
	mf[3][2] = (2 * near * far) / (near - far);
	mf[3][3] = 0;

	for (i=0; i<4; i++)
	    for (j=0; j<4; j++)
		mf[i][j] *= scale;

	if (perspNorm != (u16 *) NULL) {
	    if (near+far<=2.0) {
		*perspNorm = (u16) 0xFFFF;
	    } else  {
		*perspNorm = (u16) ((2.0*65536.0)/(near+far));
		if (*perspNorm<=0) 
		    *perspNorm = (u16) 0x0001;
	    }
	}
}

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

