/**
 * vecmath.h - Vector and matrix math utilities
 *
 * Based on arcade source files:
 *   - game/vecmath.c: magnitude, invmagnitude, direction, scalmul, scaldiv,
 *                     vecadd, vecsub, crossprod, dotprod, veccopy
 *   - game/unitvecs.c: bodtorw, rwtobod, rotateuv, rotuv, mkuv, matcopy,
 *                      matmul, fmatcopy, frot, furot, fyaw, fpitch, froll
 *   - game/d3math.c: CopyUV, TransposeUV, ScaleUV, FixRowUV, FixColumnUV,
 *                    RotateUV, YawUV, PitchUV, RollUV, WYawUV, WPitchUV, WRollUV,
 *                    BodyVector, WorldVector, CreatePYRMatrix, CreateRYPMatrix,
 *                    SlowNormalVector, NormalVector
 *   - LIB/fmath.c: xxsqrt, invsqr, fcos, fsin, hypotsincos
 *   - LIB/fmath.h: Vector/Matrix macros (SubVector, AddVector, ScaleVector, etc.)
 *
 * Used for car physics, camera transforms, and collision detection
 */

#ifndef VECMATH_H
#define VECMATH_H

#include "types.h"

/******* ARCADE CONSTANTS (from LIB/fmath.h and game/vecmath.h) *******/

#define PI              3.141592654f
#define PI_2            1.570796327f
#define BIGFLOAT        1.0e20f

/* Coordinate indices */
#ifndef XCOMP
#define XCOMP           0
#define YCOMP           1
#define ZCOMP           2
#endif

/* Angle conversion constants (from d3math.c) */
#define radtodeg        (180.0f / 3.141592654f)
#define degtorad        (3.141592654f / 180.0f)
#define radtoideg       ((0x40000000 * 2.0f) / 3.141592654f)
#define idegtorad       (3.141592654f / (0x40000000 * 2.0f))
#define radtosdeg       (0x8000 / 3.141592654f)
#define sdegtorad       (3.141592654f / 0x8000)
#define idegtodeg       (90.0f / 0x40000000)
#define degtoideg       (0x40000000 / 90.0f)
#define msininv         (1.0f / 0x7fff)
#define rdpstorpm       (60.0f / (2.0f * 3.14159f))
#define rpmtordps       (2.0f * 3.14159f / 60.0f)
#define pival           3.141592654f

/* Fixed-point scale (0x4000 = 1.0) */
#define FIXSCALE        0x4000
#define INV_FIXSCALE    (1.0f / 0x4000)

/* Unit vector rotation interval */
#define ROTSPERFIX      250

/******* ARCADE MACROS (from LIB/fmath.h) *******/

#define mabs(v)         (((v) < 0) ? -(v) : (v))
#define ABS(a)          ((a) < 0 ? -(a) : (a))
#define SQR(a)          ((a) * (a))
#define MIN(a, b)       (((a) < (b)) ? (a) : (b))
#define MAX(a, b)       (((a) > (b)) ? (a) : (b))
#define SIGN(a)         (((a) >= 0) ? 1 : -1)
#define MINMAX(a, min, max) ((a) < (min) ? (min) : (a) > (max) ? (max) : (a))
#define RAD(a)          (((a) * PI) * (1.0f / 180.0f))
#define DEG(a)          (((a) * 180.0f) * (1.0f / PI))

/* Vector macros (from LIB/fmath.h) */
#define CopyVector(v1, v2)      ((v2)[0] = (v1)[0], (v2)[1] = (v1)[1], (v2)[2] = (v1)[2])
#define ZeroVector(v1)          ((v1)[0] = 0.0f, (v1)[1] = 0.0f, (v1)[2] = 0.0f)
#define SubVector(v1, v2, r)    ((r)[0] = (v1)[0] - (v2)[0], (r)[1] = (v1)[1] - (v2)[1], (r)[2] = (v1)[2] - (v2)[2])
#define AddVector(v1, v2, r)    ((r)[0] = (v1)[0] + (v2)[0], (r)[1] = (v1)[1] + (v2)[1], (r)[2] = (v1)[2] + (v2)[2])
#define ScaleVector(v1, s, r)   ((r)[0] = (v1)[0] * (s), (r)[1] = (v1)[1] * (s), (r)[2] = (v1)[2] * (s))
#define CrossVector(v1, v2, r)  ((r)[0] = (v1)[1] * (v2)[2] - (v1)[2] * (v2)[1], \
                                 (r)[1] = (v1)[2] * (v2)[0] - (v1)[0] * (v2)[2], \
                                 (r)[2] = (v1)[0] * (v2)[1] - (v1)[1] * (v2)[0])
#define DotVector(v1, v2)       ((v1)[0] * (v2)[0] + (v1)[1] * (v2)[1] + (v1)[2] * (v2)[2])
#define VectorLength(v1)        sqrtf(SQR((v1)[0]) + SQR((v1)[1]) + SQR((v1)[2]))
#define VectorLength2(v1)       (SQR((v1)[0]) + SQR((v1)[1]) + SQR((v1)[2]))

/* Macro versions of vector functions (from game/vecmath.h) */
#define mscalmul(a, b, r)   { (r)[0] = (a)[0] * (b); (r)[1] = (a)[1] * (b); (r)[2] = (a)[2] * (b); }
#define mscaldiv(a, b, r)   { (r)[0] = (a)[0] / (b); (r)[1] = (a)[1] / (b); (r)[2] = (a)[2] / (b); }
#define mvecadd(a, b, r)    { (r)[0] = (a)[0] + (b)[0]; (r)[1] = (a)[1] + (b)[1]; (r)[2] = (a)[2] + (b)[2]; }
#define mvecsub(a, b, r)    { (r)[0] = (a)[0] - (b)[0]; (r)[1] = (a)[1] - (b)[1]; (r)[2] = (a)[2] - (b)[2]; }
#define mveccopy(a, r)      { (r)[0] = (a)[0]; (r)[1] = (a)[1]; (r)[2] = (a)[2]; }

/******* ARCADE UNIT VECTOR STRUCTURE (from game/vecmath.h) *******/

/**
 * uvect - Unit vector structure for rotation matrices
 * Based on arcade: game/vecmath.h
 *
 * Uses both fixed-point (0x4000 = 1.0) and floating-point representations.
 * The fixcnt field tracks when to re-orthonormalize the matrix.
 */
typedef struct uvect {
    s16     uvs[3][3];      /* Short unit vectors (0x4000 = 1.0) */
    f32     fpuvs[3][3];    /* Float unit vectors */
    s16     fixcnt;         /* Rotations until orthonormalization */
} uvect;

/* Unit matrix constant (extern, defined in vecmath.c) */
extern const struct uvect munituvs;

/* Vector operations */
void vec_copy(const f32 src[3], f32 dst[3]);
void vec_add(const f32 a[3], const f32 b[3], f32 result[3]);
void vec_sub(const f32 a[3], const f32 b[3], f32 result[3]);
void vec_scale(const f32 v[3], f32 scale, f32 result[3]);
f32  vec_dot(const f32 a[3], const f32 b[3]);
void vec_cross(const f32 a[3], const f32 b[3], f32 result[3]);
f32  vec_length_sq(const f32 v[3]);
f32  vec_length(const f32 v[3]);
void vec_normalize(f32 v[3]);
f32  vec_dist_sq(const f32 a[3], const f32 b[3]);

/* Matrix operations */
void mtx_copy(const f32 src[3][3], f32 dst[3][3]);
void mtx_transpose(f32 m[3][3]);
void mtx_transpose_copy(const f32 src[3][3], f32 dst[3][3]);
void mtx_scale(const f32 src[3][3], f32 dst[3][3], f32 scale);
void mtx_identity(f32 m[3][3]);
void mtx_vec_mul(const f32 m[3][3], const f32 v[3], f32 result[3]);

/* Local-space rotations */
void mtx_yaw(f32 angle, f32 uv[3][3]);
void mtx_pitch(f32 angle, f32 uv[3][3]);
void mtx_roll(f32 angle, f32 uv[3][3]);
void mtx_rotate(const f32 rpyv[3], f32 uv[3][3]);

/* World-space rotations */
void mtx_world_yaw(f32 angle, f32 uv[3][3]);
void mtx_world_pitch(f32 angle, f32 uv[3][3]);
void mtx_world_roll(f32 angle, f32 uv[3][3]);

/* Orthonormalization */
void mtx_fix_rows(f32 m[3][3]);
void mtx_fix_cols(f32 m[3][3]);

/******* ARCADE-COMPATIBLE NAMES *******/
/* These match the arcade vecmath.c function names exactly */

f32  magnitude(f32 vec[3]);             /* Vector magnitude */
f32  invmagnitude(f32 vec[3]);          /* 1/magnitude */
void direction(f32 vec[3], f32 dir[3]); /* Normalize to unit vector */
void scalmul(f32 *a, f32 b, f32 *r);    /* r = a * b (scalar) */
void scaldiv(f32 *a, f32 b, f32 *r);    /* r = a / b (scalar) */
void vecadd(f32 *ap, f32 *bp, f32 *rp); /* r = a + b */
void vecsub(f32 *ap, f32 *bp, f32 *rp); /* r = a - b */
void crossprod(f32 a[3], f32 b[3], f32 r[3]); /* r = a x b */
f32  dotprod(f32 a[3], f32 b[3]);       /* a . b */
void veccopy(f32 *a, f32 *r);           /* r = a */

/******* COORDINATE TRANSFORMS *******/

void ftransvec(f32 in[3], f32 out[3], f32 uvs[3][3]);     /* Body to world */
void finvtransvec(f32 in[3], f32 out[3], f32 uvs[3][3]);  /* World to body */
void fmatcopy(f32 *src, f32 *dst);                        /* Copy 3x3 matrix */

/******* MATH HELPERS (from d3math.c) *******/

f32 xxsqrt(f32 x);                          /* Square root wrapper */
f32 invsqr(f32 val);                        /* Inverse square root with zero protection */

/******* INTEGER VECTOR OPERATIONS (from vecmath.c) *******/

void ivecadd(s32 *ap, s32 *bp, s32 *rp);    /* Integer vector add */
void ivecsub(s32 *ap, s32 *bp, s32 *rp);    /* Integer vector subtract */

/******* SHORT INTEGER VECTOR OPERATIONS (from vecmath.c) *******/

f32  smagnitude(s16 vec[3]);                /* Short vector magnitude */
f32  invsmagnitude(s16 vec[3]);             /* Inverse short vector magnitude */
void sdirection(s16 vec[3], s16 dir[3]);    /* Normalize short vector */
void scrossprod(s16 a[3], s16 b[3], s16 r[3]); /* Short cross product */

/******* UNIT VECTOR COORDINATE TRANSFORMS (from unitvecs.c) *******/

/* Float vector transforms using uvect structure */
void bodtorw(f32 *v, f32 *vprime, struct uvect *uvst);      /* Body to world coords */
void rwtobod(f32 *vprime, f32 *v, struct uvect *uvst);      /* World to body coords */

/* Integer vector transforms */
void ibodtorw(s32 *v, s32 *vprime, struct uvect *uvst);     /* Integer body to world */
void irwtobod(s32 *vprime, s32 *v, struct uvect *uvst);     /* Integer world to body */

/* Short vector transforms */
void sbodtorw(s16 *v, s16 *vprime, struct uvect *uvst);     /* Short body to world */
void srwtobod(s16 *vprime, s16 *v, struct uvect *uvst);     /* Short world to body */

/* Float vector transforms using raw float matrix */
void fbodtorw(f32 *v, f32 *vprime, f32 uvs[3][3]);          /* Float body to world */
void frwtobod(f32 *vprime, f32 *v, f32 uvs[3][3]);          /* Float world to body */

/******* UNIT VECTOR ROTATION (from unitvecs.c) *******/

/* Float unit vector rotation functions */
void rotateuv(f32 rv[3], f32 uvs[3][3]);    /* Rotate float UV by angle vector */
void rotuv(f32 rv[3], struct uvect *uvst);  /* Rotate uvect by angle vector (body axis) */
void urotuv(f32 rv[3], struct uvect *uvst); /* Rotate uvect by angle vector (universe axis) */

/* Create unit vectors from angles */
void mkuv(s16 phi, s16 theta, s16 psi, struct uvect *uvst); /* Make UV from short angles */

/* Convert between short and float UVs */
void makefpuvs(struct uvect *uvst);         /* Generate float UVs from short UVs */
void makesuvs(struct uvect *uvst);          /* Generate short UVs from float UVs */

/* Matrix copy operations */
void matcopy(s16 *ap, s16 *bp);             /* Copy 3x3 short matrix */

/* Matrix multiply */
void matmul(s16 a[3][3], s16 b[3][3], s16 result[3][3]); /* Multiply short matrices */

/* Matrix orthonormalization */
void matfix(s16 uv[3][3], s16 result[3][3]); /* Fix short matrix orthonormality */

/* Matrix transpose */
void transpose(s16 uv[3][3], s16 result[3][3]); /* Transpose short matrix */

/******* FLOAT ROTATION CORE (from unitvecs.c) *******/

/* Low-level rotation primitives */
void frot(f32 sint, f32 cost, f32 *p1, f32 *p2);   /* Rotate in object frame */
void furot(f32 sint, f32 cost, f32 *p1, f32 *p2);  /* Rotate in universe frame */

/* Float unit vector rotations (body axis) */
void froll(f32 sint, f32 cost, f32 uv[3][3]);      /* Roll around X axis */
void fpitch(f32 sint, f32 cost, f32 uv[3][3]);     /* Pitch around Y axis */
void fyaw(f32 sint, f32 cost, f32 uv[3][3]);       /* Yaw around Z axis */

/* Float unit vector rotations (universe axis) */
void furoll(f32 sint, f32 cost, f32 uv[3][3]);     /* Universe roll */
void fupitch(f32 sint, f32 cost, f32 uv[3][3]);    /* Universe pitch */
void fuyaw(f32 sint, f32 cost, f32 uv[3][3]);      /* Universe yaw */

/******* SHORT ROTATION CORE (from unitvecs.c) *******/

/* Low-level short rotation primitive */
void rot(f32 sint, f32 cost, s16 *p1, s16 *p2);    /* Short rotate in object frame */
void urot(f32 sint, f32 cost, s16 *p1, s16 *p2);   /* Short rotate in universe frame */

/* Short unit vector rotations (body axis) */
void mroll(s16 sint, s16 cost, s16 uv[3][3]);      /* Roll around X axis */
void mpitch(s16 sint, s16 cost, s16 uv[3][3]);     /* Pitch around Y axis */
void myaw(s16 sint, s16 cost, s16 uv[3][3]);       /* Yaw around Z axis */

/* Short unit vector rotations (universe axis) */
void muroll(s16 sint, s16 cost, s16 uv[3][3]);     /* Universe roll */
void mupitch(s16 sint, s16 cost, s16 uv[3][3]);    /* Universe pitch */
void muyaw(s16 sint, s16 cost, s16 uv[3][3]);      /* Universe yaw */

/******* TRIGONOMETRY HELPERS (from d3math.c) *******/

/* Short angle trig (scale: 0x4000 = 1.0) */
s16 mssin(s16 val);                                /* Short sine */
s16 mscos(s16 val);                                /* Short cosine */

/* Hypotenuse sine/cosine */
void hypotsincos(f32 x, f32 y, f32 *sinp, f32 *cosp); /* Float version */
void shypotsincos(s16 x, s16 y, s16 *sinp, s16 *cosp); /* Short version */

/* Calculate sine/cosine of phi, theta, psi */
void sincosptp(s16 phi, s16 theta, s16 psi,
               s16 *sphip, s16 *cphip,
               s16 *sthetap, s16 *cthetap,
               s16 *spsip, s16 *cpsip);

/******* TRIG FUNCTION ALIASES (from LIB/fmath.h) *******/
/* These map arcade names to standard C library functions */

#define fsin(a)     sinf(a)
#define fcos(a)     cosf(a)
#define ftan(a)     tanf(a)
#define fasin(a)    asinf(a)
#define facos(a)    acosf(a)
#define fatan(a)    atanf(a)
#define fatan2(y,x) atan2f(y,x)
#define fsqrt(a)    sqrtf(a)

#endif /* VECMATH_H */
