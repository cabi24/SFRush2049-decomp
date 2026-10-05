/* Source: historicalsource/rushtherock at 845329d7b36f5a384c5625ed9a0aef584ab46139.
 * game/vecmath.c bodies; only F32 -> f32 and static linkage adapted.
 * LIB/fmath.h macros below are unchanged.
 */
static void vecsub(register f32 *ap, register f32 *bp, register f32 *rp)
{
	*rp++ = *ap++ - *bp++;
	*rp++ = *ap++ - *bp++;
	*rp++ = *ap++ - *bp++;
}

static void scalmul(f32 *a, f32 b, f32 *r)
{
	register int i;
	register f32 *ap,*bp,*rp;
	f32 bval;

	ap=a;
	bval = b;

	bp= &bval;
	rp=r;
	for(i=0;i<3;++i)
		*rp++ = *ap++ * *bp;
}

static void vecadd(register f32 *ap, register f32 *bp, register f32 *rp)
{
	*rp++ = *ap++ + *bp++;
	*rp++ = *ap++ + *bp++;
	*rp++ = *ap++ + *bp++;
}

#define SubVector(v1,v2,r)		(r[0] = v1[0]-v2[0], r[1] = v1[1]-v2[1], r[2] = v1[2]-v2[2])
#define ScaleAddVector(a,s,b,r)	(r[0] = (a[0]*(s)+b[0]), r[1] = (a[1]*(s)+b[1]), r[2] = (a[2]*(s)+b[2]))
