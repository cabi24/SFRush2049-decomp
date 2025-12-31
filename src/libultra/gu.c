/**
 * @file gu.c
 * @brief Graphics utility functions for matrix operations
 *
 * Decompiled from asm/us/9660.s, asm/us/9820.s, asm/us/9AB0.s
 * Contains projection and view matrix creation functions.
 *
 * These are standard libultra gu* functions used by the RSP
 * for 3D graphics rendering.
 */

#include "types.h"

#define M_PI 3.1415926f

/* External functions */
extern void guMtxIdentF(f32 mf[4][4]);
extern void guMtxF2L(f32 mf[4][4], u16 *m);
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern f32 sqrtf(f32 x);

/**
 * Create orthographic projection matrix (float)
 * (0x80008A60 - guOrthoF)
 *
 * Creates an orthographic projection matrix for 2D rendering.
 * Used for UI elements, text, and HUD overlays.
 *
 * @param mf   Destination 4x4 float matrix
 * @param l    Left clipping plane
 * @param r    Right clipping plane
 * @param b    Bottom clipping plane
 * @param t    Top clipping plane
 * @param n    Near clipping plane
 * @param f    Far clipping plane
 * @param scale Scale factor applied to all matrix elements
 */
void guOrthoF(f32 mf[4][4], f32 l, f32 r, f32 b, f32 t, f32 n, f32 f, f32 scale) {
    s32 i, j;

    guMtxIdentF(mf);

    mf[0][0] = 2.0f / (r - l);
    mf[1][1] = 2.0f / (t - b);
    mf[2][2] = -2.0f / (f - n);
    mf[3][0] = -(r + l) / (r - l);
    mf[3][1] = -(t + b) / (t - b);
    mf[3][2] = -(f + n) / (f - n);
    mf[3][3] = 1.0f;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mf[i][j] *= scale;
        }
    }
}

/**
 * Create orthographic projection matrix (fixed-point)
 * (0x80008BB4 - guOrtho)
 *
 * Wrapper that creates float matrix then converts to N64 format.
 *
 * @param m    Destination N64 Mtx (64 bytes)
 * @param l    Left clipping plane
 * @param r    Right clipping plane
 * @param b    Bottom clipping plane
 * @param t    Top clipping plane
 * @param n    Near clipping plane
 * @param f    Far clipping plane
 * @param scale Scale factor
 */
void guOrtho(u16 *m, f32 l, f32 r, f32 b, f32 t, f32 n, f32 f, f32 scale) {
    f32 mf[4][4];

    guOrthoF(mf, l, r, b, t, n, f, scale);
    guMtxF2L(mf, m);
}

/**
 * Create perspective projection matrix (float)
 * (0x80008C20 - guPerspectiveF)
 *
 * Creates a perspective projection matrix for 3D rendering.
 * Objects farther away appear smaller.
 *
 * @param mf        Destination 4x4 float matrix
 * @param perspNorm Perspective normalization value (for RSP)
 * @param fovy      Field of view in Y direction (degrees)
 * @param aspect    Aspect ratio (width/height)
 * @param near      Near clipping plane distance
 * @param far       Far clipping plane distance
 * @param scale     Scale factor applied to all matrix elements
 */
void guPerspectiveF(f32 mf[4][4], u16 *perspNorm, f32 fovy, f32 aspect,
                    f32 near, f32 far, f32 scale) {
    f32 cot;
    s32 i, j;

    guMtxIdentF(mf);

    fovy *= M_PI / 180.0f;
    cot = cosf(fovy * 0.5f) / sinf(fovy * 0.5f);

    mf[0][0] = cot / aspect;
    mf[1][1] = cot;
    mf[2][2] = (near + far) / (near - far);
    mf[2][3] = -1.0f;
    mf[3][2] = (2.0f * near * far) / (near - far);
    mf[3][3] = 0.0f;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mf[i][j] *= scale;
        }
    }

    if (perspNorm != NULL) {
        if (near + far <= 2.0f) {
            *perspNorm = 0xFFFF;
        } else {
            *perspNorm = (u16)((2.0f * 65536.0f) / (near + far));
            if (*perspNorm <= 0) {
                *perspNorm = 1;
            }
        }
    }
}

/**
 * Create perspective projection matrix (fixed-point)
 * (0x80008E50 - guPerspective)
 *
 * Wrapper that creates float matrix then converts to N64 format.
 *
 * @param m         Destination N64 Mtx (64 bytes)
 * @param perspNorm Perspective normalization value
 * @param fovy      Field of view in Y direction (degrees)
 * @param aspect    Aspect ratio (width/height)
 * @param near      Near clipping plane distance
 * @param far       Far clipping plane distance
 * @param scale     Scale factor
 */
void guPerspective(u16 *m, u16 *perspNorm, f32 fovy, f32 aspect,
                   f32 near, f32 far, f32 scale) {
    f32 mf[4][4];

    guPerspectiveF(mf, perspNorm, fovy, aspect, near, far, scale);
    guMtxF2L(mf, m);
}

/**
 * Create look-at view matrix (float)
 * (0x80008EB0 - guLookAtF)
 *
 * Creates a view matrix that positions the camera at (xEye,yEye,zEye)
 * looking toward (xAt,yAt,zAt) with the up vector (xUp,yUp,zUp).
 *
 * @param mf    Destination 4x4 float matrix
 * @param xEye  Camera X position
 * @param yEye  Camera Y position
 * @param zEye  Camera Z position
 * @param xAt   Look-at target X position
 * @param yAt   Look-at target Y position
 * @param zAt   Look-at target Z position
 * @param xUp   Up vector X component
 * @param yUp   Up vector Y component
 * @param zUp   Up vector Z component
 */
void guLookAtF(f32 mf[4][4], f32 xEye, f32 yEye, f32 zEye,
               f32 xAt, f32 yAt, f32 zAt,
               f32 xUp, f32 yUp, f32 zUp) {
    f32 len, xLook, yLook, zLook, xRight, yRight, zRight;

    guMtxIdentF(mf);

    /* Calculate look direction */
    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;

    /* Normalize look vector (negated for Z-into-screen) */
    len = sqrtf(xLook * xLook + yLook * yLook + zLook * zLook);
    len = -1.0f / len;

    xLook *= len;
    yLook *= len;
    zLook *= len;

    /* Right = Up x Look (cross product) */
    xRight = yUp * zLook - zUp * yLook;
    yRight = zUp * xLook - xUp * zLook;
    zRight = xUp * yLook - yUp * xLook;

    /* Normalize right vector */
    len = sqrtf(xRight * xRight + yRight * yRight + zRight * zRight);
    len = 1.0f / len;

    xRight *= len;
    yRight *= len;
    zRight *= len;

    /* Recalculate Up = Look x Right (orthogonal) */
    xUp = yLook * zRight - zLook * yRight;
    yUp = zLook * xRight - xLook * zRight;
    zUp = xLook * yRight - yLook * xRight;

    /* Normalize up vector */
    len = sqrtf(xUp * xUp + yUp * yUp + zUp * zUp);
    len = 1.0f / len;

    xUp *= len;
    yUp *= len;
    zUp *= len;

    /* Build the view matrix */
    mf[0][0] = xRight;
    mf[1][0] = yRight;
    mf[2][0] = zRight;
    mf[3][0] = -(xEye * xRight + yEye * yRight + zEye * zRight);

    mf[0][1] = xUp;
    mf[1][1] = yUp;
    mf[2][1] = zUp;
    mf[3][1] = -(xEye * xUp + yEye * yUp + zEye * zUp);

    mf[0][2] = xLook;
    mf[1][2] = yLook;
    mf[2][2] = zLook;
    mf[3][2] = -(xEye * xLook + yEye * yLook + zEye * zLook);

    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
    mf[3][3] = 1.0f;
}

/**
 * Create look-at view matrix (fixed-point)
 * (0x80009168 - guLookAt)
 *
 * Wrapper that creates float matrix then converts to N64 format.
 *
 * @param m     Destination N64 Mtx (64 bytes)
 * @param xEye  Camera X position
 * @param yEye  Camera Y position
 * @param zEye  Camera Z position
 * @param xAt   Look-at target X position
 * @param yAt   Look-at target Y position
 * @param zAt   Look-at target Z position
 * @param xUp   Up vector X component
 * @param yUp   Up vector Y component
 * @param zUp   Up vector Z component
 */
void guLookAt(u16 *m, f32 xEye, f32 yEye, f32 zEye,
              f32 xAt, f32 yAt, f32 zAt,
              f32 xUp, f32 yUp, f32 zUp) {
    f32 mf[4][4];

    guLookAtF(mf, xEye, yEye, zEye, xAt, yAt, zAt, xUp, yUp, zUp);
    guMtxF2L(mf, m);
}
