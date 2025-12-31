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

#endif /* VECMATH_H */
