/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "types.h"
#define ROUND(d) (int)(((d)>=0.0)?((d)+0.5):((d)-0.5))
#define ABS(d) (((d)>0)?(d):-(d))
extern const f64 gCosCoeffs[5],gCosAngleScale,gCosPiOver2Hi,gCosPiOver2Lo;
extern const f32 gCosOne,gNaNf;
float
cosf( float x )
{
float	absx;
double	dx, xsq, poly;
double	dn;
int	n;
double	result;
int	ix, xpt;


	ix = *(int *)&x;
	xpt = (ix >> 22);
	xpt &= 0x1ff;

	/* xpt is exponent(x) + 1 bit of mantissa */


	if ( xpt < 0x136 )
	{
		/* |x| < 2^28 */

		/* use the standard algorithm from Cody and Waite, doing
		   the computations in double precision
		*/

		absx = ABS(x);

		dx = absx;

		dn = dx*gCosAngleScale + 0.5;
		n = ROUND(dn);
		dn = n;

		dn -= 0.5;

		dx = dx - dn*gCosPiOver2Hi;
		dx = dx - dn*gCosPiOver2Lo;	/* dx = x - (n - 0.5)*pi */

		xsq = dx*dx;

		poly = ((gCosCoeffs[4]*xsq + gCosCoeffs[3])*xsq + gCosCoeffs[2])*xsq + gCosCoeffs[1];

		result = dx + (dx*xsq)*poly;

		/* negate result if n is odd */

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

	return ( gCosOne );
}
