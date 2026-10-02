/* GENERATED ROM-aligned TU — segment 0x9330 (rom/lib_9330)
 * layout map 030152813d73343687c493ddf9a7a7f8fe1c8cd4cdb4d934e7fc9618169c2e19; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* Canonical SDK math macros and real readonly constants; no new storage. */
#undef ABS
#define ABS(d) (((d)>0)?(d):-(d))
#define ROUND(d) (int)(((d)>=0.0)?((d)+0.5):((d)-0.5))
extern const f64 gSinCoeffs[5],gTwoOverPi,gPiOver2Hi,gPiOver2Lo;
extern const f64 gCosCoeffs[5],gCosAngleScale,gCosPiOver2Hi,gCosPiOver2Lo;
extern const f32 gNaN,gCosOne,gNaNf;


/* PROMOTED 2026-10-01 — sinf
 * Source:   cloud/work/static_C22/sinf.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul
 * Evidence: lock:cloud/work/static_C22/sinf.c:sinf (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
float
sinf( float x )
{
double	dx, xsq, poly;
double	dn;
int	n;
double	result;
int	ix, xpt;


	ix = *(int *)&x;
	xpt = (ix >> 22);
	xpt &= 0x1ff;

	/* xpt is exponent(x) + 1 bit of mantissa */

	if ( xpt < 0xff )	
	{
		/* |x| < 1.5 */

		dx = x;

		if ( xpt >= 0xe6 )
		{
			/* |x| >= 2^(-12) */

			/* compute sin(x) with a standard polynomial approximation */

			xsq = dx*dx;

			poly = ((gSinCoeffs[4]*xsq + gSinCoeffs[3])*xsq + gSinCoeffs[2])*xsq + gSinCoeffs[1];

			result = dx + (dx*xsq)*poly;

			return ( (float)result );
		}

		return ( x );
	}

	if ( xpt < 0x136 )
	{
		/* |x| < 2^28 */

		dx = x;

		/*  reduce argument to +/- pi/2  */

		dn = dx*gTwoOverPi;

		n = ROUND(dn);
		dn = n;

		dx = dx - dn*gPiOver2Hi;
		dx = dx - dn*gPiOver2Lo;	/* dx = x - n*pi */

		/* compute sin(dx) as before, negating result if n is odd
		*/

		xsq = dx*dx;

		poly = ((gSinCoeffs[4]*xsq + gSinCoeffs[3])*xsq + gSinCoeffs[2])*xsq + gSinCoeffs[1];

		result = dx + (dx*xsq)*poly;


		if ( (n & 1) == 0 )
			return ( (float)result );

		return ( -(float)result );
	}

	if ( x != x )
	{
		/* x is a NaN; return a quiet NaN */

#ifdef _IP_NAN_SETS_ERRNO

		*__errnoaddr = EDOM;
#endif
		
		return ( gNaNf );
	}

	/* just give up and return 0.0 */

	return ( gNaN );
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_9330/cosf.s")
