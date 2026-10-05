/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800BFBE8 (0x800BFBE8, 384 bytes): quaternion -> 3x3 rotation matrix (row stride 4 floats,
 * i.e. a 3x4 / 4x3 matrix).  Arcade ancestor: CreateQuatMat() in LIB/fmath.c
 * (rushtherock), copied verbatim with float literals (2.0f/0.0f/1.0f instead of 2.0/0.0/1.0).
 * `normal` != 0 means q is already unit length (s = 2); otherwise s = 2/|q|^2 (0 if |q| == 0).
 * The block-scoped xx/yy/zz/xy/... locals of the arcade source are what give retail its
 * 32-byte frame and the stack round-trips of xs/xx/yy/zz.  Also matches at -O2.
 */
typedef float f32;

void func_800BFBE8(f32 *mat, f32 q[], int normal)
{
	f32 s;
	f32 xs,	ys,	zs;
	if (!normal) {
		f32 Nq;
		Nq = q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3];
		s = (Nq > 0.0f) ? (2.0f / Nq) : 0.0f;
	} else
		s = 2.0f;
	xs = q[0]*s;
	ys = q[1]*s;
	zs = q[2]*s;
	{
		f32 xx = q[0]*xs;
		f32 yy = q[1]*ys;
		f32 zz = q[2]*zs;
		mat[0]= 1.0f - (yy + zz);
		mat[4]= 1.0f - (xx + zz);
		mat[8]= 1.0f - (xx + yy);
	}
	{
		f32 xy = q[0]*ys;
		f32 wz = q[3]*zs;
		mat[1]= -(xy + wz);
		mat[3]= -(xy - wz);
	}
	{
		f32 yz = q[1]*zs;
		f32 wx = q[3]*xs;
		mat[5]= -(yz + wx);
		mat[7]= -(yz - wx);
	}
	{
		f32 xz = q[0]*zs;
		f32 wy = q[3]*ys;
		mat[2]= xz - wy;
		mat[6]= xz + wy;
	}
}
