/**
 * vecmath.h - Vector and matrix math utilities
 *
 * Based on arcade LIB/fmath.h
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

#endif /* VECMATH_H */
