/**
 * vecmath.c - Vector and matrix math utilities
 *
 * Based on arcade source files:
 *   - game/vecmath.c: magnitude, invmagnitude, direction, scalmul, scaldiv,
 *                     vecadd, vecsub, crossprod, dotprod, veccopy
 *   - game/unitvecs.c: bodtorw, rwtobod, rotateuv, fmatcopy
 *   - game/d3math.c: CopyUV, TransposeUV, ScaleUV, FixRowUV, FixColumnUV,
 *                    RotateUV, YawUV, PitchUV, RollUV, WYawUV, WPitchUV, WRollUV,
 *                    BodyVector, WorldVector
 *   - LIB/fmath.c: xxsqrt, invsqr
 *
 * Adapted for N64 platform with proper types. These functions are used for:
 * - Car orientation/rotation (UV matrix system)
 * - Camera transforms
 * - Physics calculations
 * - Collision detection
 *
 * N64 ROM addresses mapped to arcade equivalents:
 *   0x8008B474 vector_copy_scale -> vecmath.c:scalmul()
 *   0x8008B4C4 vector_normalize_length -> fmath.c:SlowNormalVector()
 *   0x8008B660 vector3d_store_transform -> unitvecs.c:bodtorw()
 *   0x8008D714 matrix_rotation_build -> fmath.c:CreateRYPMatrix()
 *   0x8008D764 euler_to_matrix -> fmath.c:CreatePYRMatrix()
 *   0x8008D8D8 matrix_scale_apply -> fmath.c:ScaleUV()
 *   0x800A373C drone_rwtobod -> unitvecs.c:rwtobod()
 */

#include "types.h"
#include "game/vecmath.h"

/* Small angle threshold for rotation optimizations */
#define ANGLE_EPSILON 0.001f

/* External trig functions (from libm) */
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern f32 sqrtf(f32 x);
extern f32 atan2f(f32 y, f32 x);

/* Global for sdirection (arcade compatibility) */
static f32 invmag_global;

/******* MATH HELPERS (from d3math.c) *******/

/**
 * xxsqrt - Square root wrapper (arcade name)
 * Based on arcade: d3math.c:xxsqrt()
 */
f32 xxsqrt(f32 x) {
    return sqrtf(x);
}

/**
 * invsqr - Inverse square root with zero protection
 * Based on arcade: d3math.c:invsqr()
 */
f32 invsqr(f32 val) {
    if (val < 1e-4f && val > -1e-4f) {
        val = (val < 0.0f) ? -1e-4f : 1e-4f;
    }
    return 1.0f / xxsqrt(val);
}

/******* VECTOR OPERATIONS *******/

/**
 * vec_copy - Copy a 3-element vector
 * Based on arcade: fmath.c vector operations
 *
 * @param src Source vector
 * @param dst Destination vector
 */
void vec_copy(const f32 src[3], f32 dst[3]) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

/**
 * vec_add - Add two 3-element vectors
 *
 * @param a First vector
 * @param b Second vector
 * @param result Output: a + b
 */
void vec_add(const f32 a[3], const f32 b[3], f32 result[3]) {
    result[0] = a[0] + b[0];
    result[1] = a[1] + b[1];
    result[2] = a[2] + b[2];
}

/**
 * vec_sub - Subtract two 3-element vectors
 * Based on arcade: vecsub()
 *
 * @param a First vector
 * @param b Second vector
 * @param result Output: a - b
 */
void vec_sub(const f32 a[3], const f32 b[3], f32 result[3]) {
    result[0] = a[0] - b[0];
    result[1] = a[1] - b[1];
    result[2] = a[2] - b[2];
}

/**
 * vec_scale - Scale a vector by a scalar
 *
 * @param v Input vector
 * @param scale Scale factor
 * @param result Output: v * scale
 */
void vec_scale(const f32 v[3], f32 scale, f32 result[3]) {
    result[0] = v[0] * scale;
    result[1] = v[1] * scale;
    result[2] = v[2] * scale;
}

/**
 * vec_dot - Dot product of two 3-element vectors
 * Based on arcade: dotvector()
 *
 * @param a First vector
 * @param b Second vector
 * @return Dot product (a . b)
 */
f32 vec_dot(const f32 a[3], const f32 b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

/**
 * vec_cross - Cross product of two 3-element vectors
 * Based on arcade: CrossVector()
 *
 * @param a First vector
 * @param b Second vector
 * @param result Output: a x b
 */
void vec_cross(const f32 a[3], const f32 b[3], f32 result[3]) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

/**
 * vec_length_sq - Squared length of a vector (avoids sqrt)
 *
 * @param v Input vector
 * @return |v|^2
 */
f32 vec_length_sq(const f32 v[3]) {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

/**
 * vec_length - Length (magnitude) of a vector
 *
 * @param v Input vector
 * @return |v|
 */
f32 vec_length(const f32 v[3]) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/**
 * vec_normalize - Normalize a vector to unit length
 * Based on arcade: SlowNormalVector()
 *
 * @param v Vector to normalize (modified in place)
 */
void vec_normalize(f32 v[3]) {
    f32 len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (len > 0.0001f) {
        f32 inv_len = 1.0f / len;
        v[0] *= inv_len;
        v[1] *= inv_len;
        v[2] *= inv_len;
    }
}

/**
 * vec_dist_sq - Squared distance between two points
 * Used for collision detection (avoids sqrt)
 *
 * @param a First point
 * @param b Second point
 * @return |a - b|^2
 */
f32 vec_dist_sq(const f32 a[3], const f32 b[3]) {
    f32 dx = a[0] - b[0];
    f32 dy = a[1] - b[1];
    f32 dz = a[2] - b[2];
    return dx * dx + dy * dy + dz * dz;
}

/******* MATRIX (UV) OPERATIONS *******/

/**
 * mtx_copy - Copy a 3x3 rotation matrix
 * Based on arcade: CopyUV()
 *
 * @param src Source matrix
 * @param dst Destination matrix
 */
void mtx_copy(const f32 src[3][3], f32 dst[3][3]) {
    s32 i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            dst[i][j] = src[i][j];
        }
    }
}

/**
 * mtx_transpose - Transpose a 3x3 matrix in place
 * Based on arcade: TransposeUV()
 *
 * @param m Matrix to transpose
 */
void mtx_transpose(f32 m[3][3]) {
    f32 tmp;

    tmp = m[0][1];
    m[0][1] = m[1][0];
    m[1][0] = tmp;

    tmp = m[0][2];
    m[0][2] = m[2][0];
    m[2][0] = tmp;

    tmp = m[1][2];
    m[1][2] = m[2][1];
    m[2][1] = tmp;
}

/**
 * mtx_transpose_copy - Transpose a matrix into another
 * Based on arcade: TransposeCopyUV()
 *
 * @param src Source matrix
 * @param dst Destination matrix (transposed)
 */
void mtx_transpose_copy(const f32 src[3][3], f32 dst[3][3]) {
    dst[0][0] = src[0][0];
    dst[0][1] = src[1][0];
    dst[0][2] = src[2][0];

    dst[1][0] = src[0][1];
    dst[1][1] = src[1][1];
    dst[1][2] = src[2][1];

    dst[2][0] = src[0][2];
    dst[2][1] = src[1][2];
    dst[2][2] = src[2][2];
}

/**
 * mtx_scale - Scale a 3x3 matrix
 * Based on arcade: ScaleUV()
 *
 * @param src Source matrix
 * @param dst Destination matrix
 * @param scale Scale factor
 */
void mtx_scale(const f32 src[3][3], f32 dst[3][3], f32 scale) {
    s32 i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            dst[i][j] = src[i][j] * scale;
        }
    }
}

/**
 * mtx_identity - Set a 3x3 matrix to identity
 *
 * @param m Matrix to initialize
 */
void mtx_identity(f32 m[3][3]) {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f;
}

/**
 * mtx_vec_mul - Multiply a matrix by a vector
 *
 * @param m 3x3 matrix
 * @param v Input vector
 * @param result Output vector (m * v)
 */
void mtx_vec_mul(const f32 m[3][3], const f32 v[3], f32 result[3]) {
    result[0] = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2];
    result[1] = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2];
    result[2] = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2];
}

/******* ROTATION OPERATIONS *******/

/**
 * mtx_yaw - Rotate matrix around Y axis (heading)
 * Based on arcade: YawUV()
 *
 * @param angle Rotation angle in radians
 * @param uv Matrix to rotate (column-major like arcade)
 */
void mtx_yaw(f32 angle, f32 uv[3][3]) {
    f32 ut, sint, cost;
    s32 i;

    if (angle < -ANGLE_EPSILON || angle > ANGLE_EPSILON) {
        sint = sinf(angle);
        cost = cosf(angle);
        for (i = 0; i < 3; i++) {
            ut       = uv[0][i] * cost - uv[2][i] * sint;
            uv[2][i] = uv[2][i] * cost + uv[0][i] * sint;
            uv[0][i] = ut;
        }
    }
}

/**
 * mtx_pitch - Rotate matrix around X axis (tilt)
 * Based on arcade: PitchUV()
 *
 * @param angle Rotation angle in radians
 * @param uv Matrix to rotate
 */
void mtx_pitch(f32 angle, f32 uv[3][3]) {
    f32 ut, sint, cost;
    s32 i;

    if (angle < -ANGLE_EPSILON || angle > ANGLE_EPSILON) {
        sint = sinf(angle);
        cost = cosf(angle);
        for (i = 0; i < 3; i++) {
            ut       = uv[1][i] * cost - uv[2][i] * sint;
            uv[2][i] = uv[2][i] * cost + uv[1][i] * sint;
            uv[1][i] = ut;
        }
    }
}

/**
 * mtx_roll - Rotate matrix around Z axis (bank)
 * Based on arcade: RollUV()
 *
 * @param angle Rotation angle in radians
 * @param uv Matrix to rotate
 */
void mtx_roll(f32 angle, f32 uv[3][3]) {
    f32 ut, sint, cost;
    s32 i;

    if (angle < -ANGLE_EPSILON || angle > ANGLE_EPSILON) {
        sint = sinf(angle);
        cost = cosf(angle);
        for (i = 0; i < 3; i++) {
            ut       = uv[0][i] * cost - uv[1][i] * sint;
            uv[1][i] = uv[1][i] * cost + uv[0][i] * sint;
            uv[0][i] = ut;
        }
    }
}

/**
 * mtx_rotate - Rotate matrix by roll/pitch/yaw vector
 * Based on arcade: RotateUV()
 *
 * @param rpyv Roll/Pitch/Yaw vector (radians)
 * @param uv Matrix to rotate
 */
void mtx_rotate(const f32 rpyv[3], f32 uv[3][3]) {
    if (rpyv[1] != 0.0f) {
        mtx_yaw(rpyv[1], uv);
    }
    if (rpyv[0] != 0.0f) {
        mtx_pitch(rpyv[0], uv);
    }
    if (rpyv[2] != 0.0f) {
        mtx_roll(rpyv[2], uv);
    }
}

/******* WORLD-SPACE ROTATION *******/

/**
 * mtx_world_yaw - Yaw in world (universe) axis
 * Based on arcade: WYawUV()
 *
 * @param angle Rotation angle in radians
 * @param uv Matrix to rotate (row-major)
 */
void mtx_world_yaw(f32 angle, f32 uv[3][3]) {
    f32 ut, sint, cost;
    s32 i;

    if (angle < -ANGLE_EPSILON || angle > ANGLE_EPSILON) {
        sint = sinf(angle);
        cost = cosf(angle);
        for (i = 0; i < 3; i++) {
            ut       = uv[i][0] * cost - uv[i][2] * sint;
            uv[i][2] = uv[i][2] * cost + uv[i][0] * sint;
            uv[i][0] = ut;
        }
    }
}

/**
 * mtx_world_pitch - Pitch in world (universe) axis
 * Based on arcade: WPitchUV()
 *
 * @param angle Rotation angle in radians
 * @param uv Matrix to rotate
 */
void mtx_world_pitch(f32 angle, f32 uv[3][3]) {
    f32 ut, sint, cost;
    s32 i;

    if (angle < -ANGLE_EPSILON || angle > ANGLE_EPSILON) {
        sint = sinf(angle);
        cost = cosf(angle);
        for (i = 0; i < 3; i++) {
            ut       = uv[i][1] * cost - uv[i][2] * sint;
            uv[i][2] = uv[i][2] * cost + uv[i][1] * sint;
            uv[i][1] = ut;
        }
    }
}

/**
 * mtx_world_roll - Roll in world (universe) axis
 * Based on arcade: WRollUV()
 *
 * @param angle Rotation angle in radians
 * @param uv Matrix to rotate
 */
void mtx_world_roll(f32 angle, f32 uv[3][3]) {
    f32 ut, sint, cost;
    s32 i;

    if (angle < -ANGLE_EPSILON || angle > ANGLE_EPSILON) {
        sint = sinf(angle);
        cost = cosf(angle);
        for (i = 0; i < 3; i++) {
            ut       = uv[i][0] * cost - uv[i][1] * sint;
            uv[i][1] = uv[i][1] * cost + uv[i][0] * sint;
            uv[i][0] = ut;
        }
    }
}

/******* ORTHONORMALIZATION *******/

/**
 * mtx_fix_rows - Orthonormalize a matrix (row-wise)
 * Based on arcade: FixRowUV()
 *
 * @param m Matrix to fix
 */
void mtx_fix_rows(f32 m[3][3]) {
    /* Normalize row 0 */
    vec_normalize(m[0]);

    /* Normalize row 2 */
    vec_normalize(m[2]);

    /* Row 1 = Row 2 x Row 0 */
    vec_cross(m[2], m[0], m[1]);
}

/**
 * mtx_fix_cols - Orthonormalize a matrix (column-wise)
 * Based on arcade: FixColumnUV()
 *
 * @param m Matrix to fix
 */
void mtx_fix_cols(f32 m[3][3]) {
    mtx_transpose(m);
    mtx_fix_rows(m);
    mtx_transpose(m);
}

/******* ARCADE-COMPATIBLE ALIASES *******/

/**
 * magnitude - Vector magnitude (arcade name)
 * Based on arcade: vecmath.c:magnitude()
 *
 * @param vec Input vector
 * @return |vec|
 */
f32 magnitude(f32 vec[3]) {
    return sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

/**
 * invmagnitude - Inverse vector magnitude
 * Based on arcade: vecmath.c:invmagnitude()
 *
 * @param vec Input vector
 * @return 1 / |vec|
 */
f32 invmagnitude(f32 vec[3]) {
    f32 len_sq = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
    if (len_sq > 0.0001f) {
        return 1.0f / sqrtf(len_sq);
    }
    return 0.0f;
}

/**
 * direction - Normalize vector to unit length
 * Based on arcade: vecmath.c:direction()
 *
 * @param vec Input vector
 * @param dir Output unit vector
 */
void direction(f32 vec[3], f32 dir[3]) {
    f32 invmag = invmagnitude(vec);
    dir[0] = vec[0] * invmag;
    dir[1] = vec[1] * invmag;
    dir[2] = vec[2] * invmag;
}

/**
 * scalmul - Scale multiply (arcade name)
 * Based on arcade: vecmath.c:scalmul()
 *
 * @param a Input vector
 * @param b Scalar
 * @param r Output vector: a * b
 */
void scalmul(f32 *a, f32 b, f32 *r) {
    register s32 i;
    register f32 *ap, *bp, *rp;
    f32 bval;

    ap = a;
    bval = b;
    bp = &bval;
    rp = r;
    for (i = 0; i < 3; ++i) {
        *rp++ = *ap++ * *bp;
    }
}

/**
 * scaldiv - Scale divide (arcade name)
 * Based on arcade: vecmath.c:scaldiv()
 *
 * @param a Input vector
 * @param b Scalar divisor
 * @param r Output vector: a / b
 */
void scaldiv(f32 *a, f32 b, f32 *r) {
    register s32 i;
    register f32 *ap, *bp, *rp;
    f32 binv;

    ap = a;
    binv = 1.0f / b;
    bp = &binv;
    rp = r;
    for (i = 0; i < 3; ++i) {
        *rp++ = *ap++ * *bp;
    }
}

/**
 * vecadd - Vector add (arcade name)
 * Based on arcade: vecmath.c:vecadd()
 */
void vecadd(register f32 *ap, register f32 *bp, register f32 *rp) {
    *rp++ = *ap++ + *bp++;
    *rp++ = *ap++ + *bp++;
    *rp++ = *ap++ + *bp++;
}

/**
 * vecsub - Vector subtract (arcade name)
 * Based on arcade: vecmath.c:vecsub()
 */
void vecsub(register f32 *ap, register f32 *bp, register f32 *rp) {
    *rp++ = *ap++ - *bp++;
    *rp++ = *ap++ - *bp++;
    *rp++ = *ap++ - *bp++;
}

/**
 * ivecadd - Integer vector add (arcade name)
 * Based on arcade: vecmath.c:ivecadd()
 */
void ivecadd(register s32 *ap, register s32 *bp, register s32 *rp) {
    *rp++ = *bp++ + *ap++;
    *rp++ = *bp++ + *ap++;
    *rp++ = *bp++ + *ap++;
}

/**
 * ivecsub - Integer vector subtract (arcade name)
 * Based on arcade: vecmath.c:ivecsub()
 */
void ivecsub(register s32 *ap, register s32 *bp, register s32 *rp) {
    *rp++ = *ap++ - *bp++;
    *rp++ = *ap++ - *bp++;
    *rp++ = *ap++ - *bp++;
}

/**
 * crossprod - Cross product (arcade name)
 * Based on arcade: vecmath.c:crossprod()
 */
void crossprod(f32 a[3], f32 b[3], f32 r[3]) {
    f32 temp[3];
    r[0]    = a[1] * b[2];
    temp[0] = a[2] * b[1];
    r[1]    = a[2] * b[0];
    temp[1] = a[0] * b[2];
    r[2]    = a[0] * b[1];
    temp[2] = a[1] * b[0];
    vecsub(r, temp, r);
}

/**
 * dotprod - Dot product (arcade name)
 * Based on arcade: vecmath.c:dotprod()
 */
f32 dotprod(f32 a[3], f32 b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

/**
 * veccopy - Vector copy (arcade name)
 * Based on arcade: vecmath.c:veccopy()
 */
void veccopy(f32 *a, f32 *r) {
    register s32 i;
    register f32 *ap, *rp;

    ap = a;
    rp = r;
    for (i = 0; i < 3; ++i) {
        *rp++ = *ap++;
    }
}

/******* SHORT INTEGER VECTOR OPS (Fixed-point 0x4000=1.0) *******/

/**
 * smagnitude - Short integer vector magnitude
 * Based on arcade: vecmath.c:smagnitude()
 */
f32 smagnitude(s16 vec[3]) {
    return xxsqrt((f32)vec[0] * (f32)vec[0] +
                  (f32)vec[1] * (f32)vec[1] +
                  (f32)vec[2] * (f32)vec[2]);
}

/**
 * invsmagnitude - Inverse short integer vector magnitude
 * Based on arcade: vecmath.c:invsmagnitude()
 */
f32 invsmagnitude(s16 vec[3]) {
    return invsqr((f32)vec[0] * (f32)vec[0] +
                  (f32)vec[1] * (f32)vec[1] +
                  (f32)vec[2] * (f32)vec[2]);
}

/**
 * sdirection - Normalize short integer vector (0x4000 = 1.0 scale)
 * Based on arcade: vecmath.c:sdirection()
 */
void sdirection(s16 vec[3], s16 dir[3]) {
    f32 invmag;

    invmag = invsmagnitude(vec) * 0x4000;
    dir[0] = (s16)(vec[0] * invmag);
    dir[1] = (s16)(vec[1] * invmag);
    dir[2] = (s16)(vec[2] * invmag);
}

/**
 * scrossprod - Short integer cross product (0x4000 = 1.0 scale)
 * Based on arcade: vecmath.c:scrossprod()
 */
void scrossprod(s16 a[3], s16 b[3], s16 r[3]) {
    r[0] = (s16)(((f32)a[1] * (f32)b[2] - (f32)a[2] * (f32)b[1]) * (1.0f / 0x4000));
    r[1] = (s16)(((f32)a[2] * (f32)b[0] - (f32)a[0] * (f32)b[2]) * (1.0f / 0x4000));
    r[2] = (s16)(((f32)a[0] * (f32)b[1] - (f32)a[1] * (f32)b[0]) * (1.0f / 0x4000));
}

/******* COORDINATE TRANSFORMS *******/

/**
 * ftransvec - Transform vector by rotation matrix
 * Based on arcade: math.c:ftransvec()
 *
 * Transforms a body-coordinate vector to world coordinates.
 *
 * @param in Input vector (body coords)
 * @param out Output vector (world coords)
 * @param uvs 3x3 rotation matrix
 */
void ftransvec(f32 in[3], f32 out[3], f32 uvs[3][3]) {
    out[0] = in[0] * uvs[0][0] + in[1] * uvs[1][0] + in[2] * uvs[2][0];
    out[1] = in[0] * uvs[0][1] + in[1] * uvs[1][1] + in[2] * uvs[2][1];
    out[2] = in[0] * uvs[0][2] + in[1] * uvs[1][2] + in[2] * uvs[2][2];
}

/**
 * finvtransvec - Inverse transform vector by rotation matrix
 * Based on arcade: math.c
 *
 * Transforms a world-coordinate vector to body coordinates.
 *
 * @param in Input vector (world coords)
 * @param out Output vector (body coords)
 * @param uvs 3x3 rotation matrix
 */
void finvtransvec(f32 in[3], f32 out[3], f32 uvs[3][3]) {
    out[0] = in[0] * uvs[0][0] + in[1] * uvs[0][1] + in[2] * uvs[0][2];
    out[1] = in[0] * uvs[1][0] + in[1] * uvs[1][1] + in[2] * uvs[1][2];
    out[2] = in[0] * uvs[2][0] + in[1] * uvs[2][1] + in[2] * uvs[2][2];
}

/**
 * fmatcopy - Copy 3x3 float matrix
 * Based on arcade: unitvecs.c:fmatcopy()
 *
 * @param src Source matrix (as float pointer)
 * @param dst Destination matrix (as float pointer)
 */
void fmatcopy(f32 *ap, f32 *bp) {
    *bp++ = *ap++;
    *bp++ = *ap++;
    *bp++ = *ap++;

    *bp++ = *ap++;
    *bp++ = *ap++;
    *bp++ = *ap++;

    *bp++ = *ap++;
    *bp++ = *ap++;
    *bp++ = *ap++;
}

/******* UNIT VECTOR STRUCTURE OPERATIONS (from unitvecs.c) *******/

/**
 * Unit identity matrix constant
 * Based on arcade: unitvecs.c:munituvs
 */
const struct uvect munituvs = {
    {
        {0x4000, 0, 0},
        {0, 0x4000, 0},
        {0, 0, 0x4000}
    },
    {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}
    },
    ROTSPERFIX
};

/**
 * bodtorw - Convert vector from body to real world coordinates
 * Based on arcade: unitvecs.c:bodtorw()
 *
 * @param v Input vector (body coords)
 * @param vprime Output vector (world coords)
 * @param uvst Unit vector structure
 */
void bodtorw(f32 *v, f32 *vprime, struct uvect *uvst) {
    f32 *uvp, r;
    s32 i;

    uvp = uvst->fpuvs[0];

    i = 3;
    do {
        r = *v++ * *uvp++;
        r += *v++ * *uvp++;
        *vprime++ = r + *v++ * *uvp++;
        v -= 3;
    } while (--i);
}

/**
 * rwtobod - Convert vector from real world to body coordinates
 * Based on arcade: unitvecs.c:rwtobod()
 *
 * @param vprime Input vector (world coords)
 * @param v Output vector (body coords)
 * @param uvst Unit vector structure
 */
void rwtobod(f32 *vprime, f32 *v, struct uvect *uvst) {
    f32 *uvp, r;
    s32 i;

    uvp = uvst->fpuvs[0];

    i = 3;
    do {
        r = *vprime++ * *uvp;
        uvp += 3;
        r += *vprime++ * *uvp;
        uvp += 3;
        *v++ = r + *vprime++ * *uvp;
        uvp -= 5;
        vprime -= 3;
    } while (--i);
}

/**
 * ibodtorw - Convert integer vector from body to real world coordinates
 * Based on arcade: unitvecs.c:ibodtorw()
 *
 * @param v Input vector (body coords, integer)
 * @param vprime Output vector (world coords, integer)
 * @param uvst Unit vector structure
 */
void ibodtorw(s32 *v, s32 *vprime, struct uvect *uvst) {
    s16 *uvp;
    s32 i;
    f32 r;

    uvp = uvst->uvs[0];
    i = 3;
    do {
        r = (f32)*v++ * (f32)*uvp++;
        r += (f32)*v++ * (f32)*uvp++;
        *vprime++ = (s32)((r + (f32)*v++ * (f32)*uvp++) * INV_FIXSCALE);
        v -= 3;
    } while (--i);
}

/**
 * irwtobod - Convert integer vector from real world to body coordinates
 * Based on arcade: unitvecs.c:irwtobod()
 *
 * @param vprime Input vector (world coords, integer)
 * @param v Output vector (body coords, integer)
 * @param uvst Unit vector structure
 */
void irwtobod(s32 *vprime, s32 *v, struct uvect *uvst) {
    s16 *uvp;
    s32 i;
    f32 r;

    uvp = uvst->uvs[0];
    i = 3;
    do {
        r = (f32)*vprime++ * (f32)*uvp;
        uvp += 3;

        r += (f32)*vprime++ * (f32)*uvp;
        uvp += 3;

        *v++ = (s32)((r + (f32)*vprime++ * (f32)*uvp) * INV_FIXSCALE);
        vprime -= 3;
        uvp -= 5;
    } while (--i);
}

/**
 * sbodtorw - Convert short vector from body to real world coordinates
 * Based on arcade: unitvecs.c:sbodtorw()
 *
 * @param v Input vector (body coords, short)
 * @param vprime Output vector (world coords, short)
 * @param uvst Unit vector structure
 */
void sbodtorw(s16 *v, s16 *vprime, struct uvect *uvst) {
    s16 *uvp;
    s32 i;
    f32 r;

    uvp = uvst->uvs[0];
    i = 3;
    do {
        r = (f32)*v++ * (f32)*uvp++;
        r += (f32)*v++ * (f32)*uvp++;
        *vprime++ = (s16)((r + (f32)*v++ * (f32)*uvp++) * INV_FIXSCALE);
        v -= 3;
    } while (--i);
}

/**
 * srwtobod - Convert short vector from real world to body coordinates
 * Based on arcade: unitvecs.c:srwtobod()
 *
 * @param vprime Input vector (world coords, short)
 * @param v Output vector (body coords, short)
 * @param uvst Unit vector structure
 */
void srwtobod(s16 *vprime, s16 *v, struct uvect *uvst) {
    s16 *uvp;
    s32 i;
    f32 r;

    uvp = uvst->uvs[0];
    i = 3;
    do {
        r = (f32)*vprime++ * (f32)*uvp;
        uvp += 3;

        r += (f32)*vprime++ * (f32)*uvp;
        uvp += 3;

        *v++ = (s16)((r + (f32)*vprime++ * (f32)*uvp) * INV_FIXSCALE);
        vprime -= 3;
        uvp -= 5;
    } while (--i);
}

/**
 * fbodtorw - Convert float vector from body to world using float matrix
 *
 * @param v Input vector (body coords)
 * @param vprime Output vector (world coords)
 * @param uvs 3x3 float rotation matrix
 */
void fbodtorw(f32 *v, f32 *vprime, f32 uvs[3][3]) {
    f32 *uvp, r;
    s32 i;

    uvp = uvs[0];

    i = 3;
    do {
        r = *v++ * *uvp++;
        r += *v++ * *uvp++;
        *vprime++ = r + *v++ * *uvp++;
        v -= 3;
    } while (--i);
}

/**
 * frwtobod - Convert float vector from world to body using float matrix
 *
 * @param vprime Input vector (world coords)
 * @param v Output vector (body coords)
 * @param uvs 3x3 float rotation matrix
 */
void frwtobod(f32 *vprime, f32 *v, f32 uvs[3][3]) {
    f32 *uvp, r;
    s32 i;

    uvp = uvs[0];

    i = 3;
    do {
        r = *vprime++ * *uvp;
        uvp += 3;
        r += *vprime++ * *uvp;
        uvp += 3;
        *v++ = r + *vprime++ * *uvp;
        uvp -= 5;
        vprime -= 3;
    } while (--i);
}

/******* UNIT VECTOR ROTATION (from unitvecs.c) *******/

/**
 * rotateuv - Rotate float 3x3 unit vector by rotation vector in body axis
 * Based on arcade: unitvecs.c:rotateuv()
 *
 * @param rv Rotation vector [roll, pitch, yaw] in radians
 * @param uvs 3x3 float unit vector matrix
 */
void rotateuv(f32 rv[3], f32 uvs[3][3]) {
    if (rv[ZCOMP]) {
        fyaw(sinf(rv[ZCOMP]), cosf(rv[ZCOMP]), uvs);
    }
    if (rv[YCOMP]) {
        fpitch(sinf(rv[YCOMP]), cosf(rv[YCOMP]), uvs);
    }
    if (rv[XCOMP]) {
        froll(sinf(rv[XCOMP]), cosf(rv[XCOMP]), uvs);
    }
}

/**
 * rotuv - Rotate uvect structure by rotation vector in body axis
 * Based on arcade: unitvecs.c:rotuv()
 *
 * @param rv Rotation vector in radians
 * @param uvstruct Unit vector structure to rotate
 */
void rotuv(f32 rv[3], struct uvect *uvstruct) {
    s16 fixed[3][3];

    if (rv[ZCOMP]) {
        fyaw(sinf(rv[ZCOMP]), cosf(rv[ZCOMP]), uvstruct->fpuvs);
    }
    if (rv[YCOMP]) {
        fpitch(sinf(rv[YCOMP]), cosf(rv[YCOMP]), uvstruct->fpuvs);
    }
    if (rv[XCOMP]) {
        froll(sinf(rv[XCOMP]), cosf(rv[XCOMP]), uvstruct->fpuvs);
    }

    makesuvs(uvstruct);

    if (uvstruct->fixcnt <= 0) {
        uvstruct->fixcnt = ROTSPERFIX;
        matfix(uvstruct->uvs, fixed);
        matcopy((s16 *)(fixed), (s16 *)(uvstruct->uvs));
        makefpuvs(uvstruct);
    } else {
        uvstruct->fixcnt--;
    }
}

/**
 * urotuv - Rotate uvect structure by rotation vector in universe axis
 * Based on arcade: unitvecs.c:urotuv()
 *
 * @param rv Rotation vector in radians
 * @param uvstruct Unit vector structure to rotate
 */
void urotuv(f32 rv[3], struct uvect *uvstruct) {
    s16 fixed[3][3];

    if (rv[ZCOMP]) {
        fuyaw(sinf(rv[ZCOMP]), cosf(rv[ZCOMP]), uvstruct->fpuvs);
    }
    if (rv[YCOMP]) {
        fupitch(sinf(rv[YCOMP]), cosf(rv[YCOMP]), uvstruct->fpuvs);
    }
    if (rv[XCOMP]) {
        furoll(sinf(rv[XCOMP]), cosf(rv[XCOMP]), uvstruct->fpuvs);
    }

    makesuvs(uvstruct);

    if (uvstruct->fixcnt <= 0) {
        uvstruct->fixcnt = ROTSPERFIX;
        matfix(uvstruct->uvs, fixed);
        matcopy((s16 *)(fixed), (s16 *)(uvstruct->uvs));
        makefpuvs(uvstruct);
    } else {
        uvstruct->fixcnt--;
    }
}

/**
 * mkuv - Create unit vector structure from Euler angles
 * Based on arcade: unitvecs.c:mkuv()
 *
 * @param phi Roll angle (short, 0x8000 = pi)
 * @param theta Pitch angle
 * @param psi Yaw angle
 * @param uvstruct Output unit vector structure
 */
void mkuv(s16 phi, s16 theta, s16 psi, struct uvect *uvstruct) {
    f32 rotv[3];
    *uvstruct = munituvs;

    rotv[0] = phi * sdegtorad;
    rotv[1] = theta * sdegtorad;
    rotv[2] = psi * sdegtorad;

    rotuv(rotv, uvstruct);
}

/**
 * makefpuvs - Generate float UVs from short UVs
 * Based on arcade: unitvecs.c:makefpuvs()
 *
 * @param uvstruct Unit vector structure
 */
void makefpuvs(struct uvect *uvstruct) {
    s16 *suvp;
    f32 *fpuvp;
    s32 i;

    for (i = 9 + 1, suvp = uvstruct->uvs[0], fpuvp = uvstruct->fpuvs[0];
         --i;) {
        *fpuvp++ = *suvp++ * INV_FIXSCALE;
    }
}

/**
 * makesuvs - Generate short UVs from float UVs
 * Based on arcade: unitvecs.c:makesuvs()
 *
 * @param uvstruct Unit vector structure
 */
void makesuvs(struct uvect *uvstruct) {
    s16 *suvp;
    f32 *fpuvp;
    s32 i;

    for (i = 9 + 1, suvp = uvstruct->uvs[0], fpuvp = uvstruct->fpuvs[0];
         --i;) {
        *suvp++ = (s16)(*fpuvp++ * 16384.0f);
    }
}

/**
 * matcopy - Copy 3x3 short matrix
 * Based on arcade: unitvecs.c:matcopy()
 *
 * @param ap Source matrix
 * @param bp Destination matrix
 */
void matcopy(s16 *ap, s16 *bp) {
    *bp++ = *ap++;
    *bp++ = *ap++;
    *bp++ = *ap++;

    *bp++ = *ap++;
    *bp++ = *ap++;
    *bp++ = *ap++;

    *bp++ = *ap++;
    *bp++ = *ap++;
    *bp++ = *ap++;
}

/**
 * matmul - Multiply two 3x3 short matrices
 * Based on arcade: unitvecs.c:matmul()
 *
 * @param a First matrix
 * @param b Second matrix
 * @param result Output matrix (a * b)
 */
void matmul(s16 a[3][3], s16 b[3][3], s16 result[3][3]) {
    s16 *resp, *ap, *bp;
    s32 i;
    s32 j;

    for (j = 3, resp = result[0], ap = a[0]; j--; ap += 3) {
        for (i = 3, bp = b[0]; i--; bp++, resp++) {
            *resp = (s16)((ap[0] * bp[0] + ap[1] * bp[3] + ap[2] * bp[6]) >> 14);
        }
    }
}

/**
 * matfix - Orthonormalize short matrix
 * Based on arcade: unitvecs.c:matfix()
 *
 * @param uv Input matrix
 * @param result Output orthonormalized matrix
 */
void matfix(s16 uv[3][3], s16 result[3][3]) {
    scrossprod(uv[0], uv[1], result[2]);   /* Make all vectors perp */
    scrossprod(result[2], uv[0], result[1]);
    scrossprod(result[1], result[2], result[0]);

    sdirection(result[0], result[0]);      /* Make vectors of length 1 */
    sdirection(result[1], result[1]);
    sdirection(result[2], result[2]);
}

/**
 * transpose - Transpose short 3x3 matrix
 * Based on arcade: unitvecs.c:transpose()
 *
 * @param uv Input matrix
 * @param result Output transposed matrix
 */
void transpose(s16 uv[3][3], s16 result[3][3]) {
    s32 i;
    s16 *resp, *uvp;

    resp = (s16 *)(&(uv[0][0]));
    uvp = (s16 *)(&(result[0][0]));
    for (i = 3; --i >= 0;) {
        *resp++ = *uvp;
        uvp += 3;
        *resp++ = *uvp;
        uvp += 3;
        *resp++ = *uvp;
        uvp -= 5;
    }
}

/******* ROTATION PRIMITIVES (from unitvecs.c) *******/

/**
 * rot - Rotate short unit vector in object frame
 * Based on arcade: unitvecs.c:rot()
 */
void rot(f32 sint, f32 cost, s16 *p1, s16 *p2) {
    f32 f1, f2;

    f1 = *p1;
    f2 = *p2;
    *p1 = (s16)(f1 * cost + f2 * sint);
    p1 += 3;
    *p2 = (s16)(f2 * cost - f1 * sint);
    p2 += 3;

    f1 = *p1;
    f2 = *p2;
    *p1 = (s16)(f1 * cost + f2 * sint);
    p1 += 3;
    *p2 = (s16)(f2 * cost - f1 * sint);
    p2 += 3;

    f1 = *p1;
    f2 = *p2;
    *p1 = (s16)(f1 * cost + f2 * sint);
    *p2 = (s16)(f2 * cost - f1 * sint);
}

/**
 * urot - Rotate short unit vector in universe frame
 * Based on arcade: unitvecs.c:urot()
 */
void urot(f32 sint, f32 cost, s16 *p1, s16 *p2) {
    f32 f1, f2;

    f1 = *p1;
    f2 = *p2;
    *p1++ = (s16)(f1 * cost + f2 * sint);
    *p2++ = (s16)(f2 * cost - f1 * sint);

    f1 = *p1;
    f2 = *p2;
    *p1++ = (s16)(f1 * cost + f2 * sint);
    *p2++ = (s16)(f2 * cost - f1 * sint);

    f1 = *p1;
    f2 = *p2;
    *p1 = (s16)(f1 * cost + f2 * sint);
    *p2 = (s16)(f2 * cost - f1 * sint);
}

/**
 * mroll - Rotate short unit vectors around X body axis
 * Based on arcade: unitvecs.c:mroll()
 */
void mroll(s16 sint, s16 cost, s16 uv[3][3]) {
    rot(sint * INV_FIXSCALE, cost * INV_FIXSCALE, &uv[0][1], &uv[0][2]);
}

/**
 * muroll - Rotate short unit vectors around X universe axis
 * Based on arcade: unitvecs.c:muroll()
 */
void muroll(s16 sint, s16 cost, s16 uv[3][3]) {
    urot(sint * INV_FIXSCALE, cost * INV_FIXSCALE, &uv[1][0], &uv[2][0]);
}

/**
 * mpitch - Rotate short unit vectors around Y body axis
 * Based on arcade: unitvecs.c:mpitch()
 */
void mpitch(s16 sint, s16 cost, s16 uv[3][3]) {
    /* Pitch has signs reversed for some reason */
    rot(-sint * INV_FIXSCALE, cost * INV_FIXSCALE, &uv[0][0], &uv[0][2]);
}

/**
 * mupitch - Rotate short unit vectors around Y universe axis
 * Based on arcade: unitvecs.c:mupitch()
 */
void mupitch(s16 sint, s16 cost, s16 uv[3][3]) {
    /* Pitch has signs reversed for some reason */
    urot(-sint * INV_FIXSCALE, cost * INV_FIXSCALE, &uv[0][0], &uv[2][0]);
}

/**
 * myaw - Rotate short unit vectors around Z body axis
 * Based on arcade: unitvecs.c:myaw()
 */
void myaw(s16 sint, s16 cost, s16 uv[3][3]) {
    rot(sint * INV_FIXSCALE, cost * INV_FIXSCALE, &uv[0][1], &uv[0][0]);
}

/**
 * muyaw - Rotate short unit vectors around Z universe axis
 * Based on arcade: unitvecs.c:muyaw()
 */
void muyaw(s16 sint, s16 cost, s16 uv[3][3]) {
    urot(sint * INV_FIXSCALE, cost * INV_FIXSCALE, &uv[1][0], &uv[0][0]);
}

/**
 * frot - Rotate float unit vector in object frame
 * Based on arcade: unitvecs.c:frot()
 */
void frot(f32 sint, f32 cost, f32 *p1, f32 *p2) {
    f32 ut;

    ut = *p1 * cost + *p2 * sint;
    *p2 = *p2 * cost - *p1 * sint;
    p2 += 3;
    *p1 = ut;
    p1 += 3;

    ut = *p1 * cost + *p2 * sint;
    *p2 = *p2 * cost - *p1 * sint;
    p2 += 3;
    *p1 = ut;
    p1 += 3;

    ut = *p1 * cost + *p2 * sint;
    *p2 = *p2 * cost - *p1 * sint;
    *p1 = ut;
}

/**
 * furot - Rotate float unit vector in universe frame
 * Based on arcade: unitvecs.c:furot()
 */
void furot(f32 sint, f32 cost, f32 *p1, f32 *p2) {
    f32 ut;

    ut = *p1 * cost + *p2 * sint;
    *p2 = *p2 * cost - *p1 * sint;
    p2++;
    *p1 = ut;
    p1++;

    ut = *p1 * cost + *p2 * sint;
    *p2 = *p2 * cost - *p1 * sint;
    p2++;
    *p1 = ut;
    p1++;

    ut = *p1 * cost + *p2 * sint;
    *p2 = *p2 * cost - *p1 * sint;
    *p1 = ut;
}

/**
 * froll - Rotate float unit vectors around X body axis
 * Based on arcade: unitvecs.c:froll()
 */
void froll(f32 sint, f32 cost, f32 uv[3][3]) {
    frot(sint, cost, &uv[0][1], &uv[0][2]);
}

/**
 * furoll - Rotate float unit vectors around X universe axis
 * Based on arcade: unitvecs.c:furoll()
 */
void furoll(f32 sint, f32 cost, f32 uv[3][3]) {
    furot(sint, cost, &uv[1][0], &uv[2][0]);
}

/**
 * fpitch - Rotate float unit vectors around Y body axis
 * Based on arcade: unitvecs.c:fpitch()
 */
void fpitch(f32 sint, f32 cost, f32 uv[3][3]) {
    /* Pitch doesn't work unless signs reversed */
    frot(-sint, cost, &uv[0][0], &uv[0][2]);
}

/**
 * fupitch - Rotate float unit vectors around Y universe axis
 * Based on arcade: unitvecs.c:fupitch()
 */
void fupitch(f32 sint, f32 cost, f32 uv[3][3]) {
    /* Pitch doesn't work unless signs reversed */
    furot(-sint, cost, &uv[0][0], &uv[2][0]);
}

/**
 * fyaw - Rotate float unit vectors around Z body axis
 * Based on arcade: unitvecs.c:fyaw()
 */
void fyaw(f32 sint, f32 cost, f32 uv[3][3]) {
    frot(sint, cost, &uv[0][0], &uv[0][1]);
}

/**
 * fuyaw - Rotate float unit vectors around Z universe axis
 * Based on arcade: unitvecs.c:fuyaw()
 */
void fuyaw(f32 sint, f32 cost, f32 uv[3][3]) {
    furot(sint, cost, &uv[0][0], &uv[1][0]);
}

/******* TRIGONOMETRY HELPERS (from d3math.c) *******/

/**
 * mssin - Short angle sine function
 * Based on arcade: d3math.c:mssin()
 *
 * Takes a short angle (0x8000 = pi radians) and returns
 * sine scaled to 0x4000 = 1.0
 *
 * @param val Angle in short degrees (0x8000 = pi)
 * @return Sine value scaled (0x4000 = 1.0)
 */
s16 mssin(s16 val) {
    return (s16)(sinf(sdegtorad * val) * (f32)0x4000);
}

/**
 * mscos - Short angle cosine function
 * Based on arcade: d3math.c:mscos()
 *
 * Takes a short angle (0x8000 = pi radians) and returns
 * cosine scaled to 0x4000 = 1.0
 *
 * @param val Angle in short degrees (0x8000 = pi)
 * @return Cosine value scaled (0x4000 = 1.0)
 */
s16 mscos(s16 val) {
    return (s16)(cosf(sdegtorad * val) * (f32)0x4000);
}

/**
 * hypotsincos - Calculate sine and cosine from x,y coordinates
 * Based on arcade: d3math.c:hypotsincos()
 *
 * Given x and y, returns the sine and cosine of the angle adjacent to x
 *
 * @param x X coordinate
 * @param y Y coordinate
 * @param sinp Output sine
 * @param cosp Output cosine
 */
void hypotsincos(f32 x, f32 y, f32 *sinp, f32 *cosp) {
    f32 angle;

    angle = atan2f(x, y);
    *sinp = sinf(angle);
    *cosp = cosf(angle);
}

/**
 * shypotsincos - Calculate sine and cosine from short x,y coordinates
 * Based on arcade: d3math.c:shypotsincos()
 *
 * Given x and y (shorts), returns sine and cosine scaled to 0x4000 = 1.0
 *
 * @param x X coordinate (short)
 * @param y Y coordinate (short)
 * @param sinp Output sine (0x4000 = 1.0)
 * @param cosp Output cosine (0x4000 = 1.0)
 */
void shypotsincos(s16 x, s16 y, s16 *sinp, s16 *cosp) {
    f32 fx, fy;
    f32 invhypot;

    fx = (f32)x;
    fy = (f32)y;
    invhypot = 0x4000 * invsqr(fx * fx + fy * fy);
    *sinp = (s16)(fx * invhypot);
    *cosp = (s16)(fy * invhypot);
}

/**
 * sincosptp - Calculate sine/cosine for phi, theta, psi angles
 * Based on arcade: d3math.c:sincosptp()
 *
 * @param phi Roll angle (short degrees)
 * @param theta Pitch angle (short degrees)
 * @param psi Yaw angle (short degrees)
 * @param sphip Output sine of phi
 * @param cphip Output cosine of phi
 * @param sthetap Output sine of theta
 * @param cthetap Output cosine of theta
 * @param spsip Output sine of psi
 * @param cpsip Output cosine of psi
 */
void sincosptp(s16 phi, s16 theta, s16 psi,
               s16 *sphip, s16 *cphip,
               s16 *sthetap, s16 *cthetap,
               s16 *spsip, s16 *cpsip) {
    *sthetap = mssin(theta);
    *cthetap = mscos(theta);
    *sphip = mssin(phi);
    *cphip = mscos(phi);
    *spsip = mssin(psi);
    *cpsip = mscos(psi);
}
