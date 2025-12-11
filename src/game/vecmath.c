/**
 * vecmath.c - Vector and matrix math utilities
 *
 * Based on arcade LIB/fmath.c
 * Adapted for N64 platform with proper types
 *
 * These functions are used throughout the game for:
 * - Car orientation/rotation
 * - Camera transforms
 * - Physics calculations
 * - Collision detection
 */

#include "types.h"

/* Small angle threshold for rotation optimizations */
#define ANGLE_EPSILON 0.001f

/* External trig functions (from libm) */
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern f32 sqrtf(f32 x);

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
