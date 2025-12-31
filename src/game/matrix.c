/**
 * @file matrix.c
 * @brief 4x4 Matrix operations for N64 graphics
 *
 * Decompiled from asm/us/9DE0.s
 * Contains matrix creation and conversion functions used by the RSP.
 *
 * N64 uses fixed-point 16.16 format for matrix values in Mtx structures.
 * These functions convert between float and fixed-point representations.
 */

#include "types.h"

/**
 * N64 matrix format (64 bytes):
 * Integer parts (32 bytes): 4x4 s16 values, column-major
 * Fraction parts (32 bytes): 4x4 u16 values, column-major
 *
 * Full value = integer + fraction/65536
 */

/**
 * Initialize a 4x4 identity matrix (float)
 * (0x80009394 - guMtxIdentF / mtxf_identity)
 *
 * Creates a 4x4 identity matrix:
 *   | 1  0  0  0 |
 *   | 0  1  0  0 |
 *   | 0  0  1  0 |
 *   | 0  0  0  1 |
 *
 * @param mtx Pointer to 16-float matrix (64 bytes)
 */
void guMtxIdentF(f32 mtx[4][4]) {
    s32 i, j;
    f32 one = 1.0f;
    f32 zero = 0.0f;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == j) {
                mtx[i][j] = one;
            } else {
                mtx[i][j] = zero;
            }
        }
    }
}

/**
 * Convert float matrix to N64 fixed-point format
 * (0x800091E0 - guMtxF2L / mtxf_to_mtx)
 *
 * Converts a 4x4 float matrix to N64's 16.16 fixed-point Mtx format.
 * The scale factor is 65536.0 (0x10000).
 *
 * The N64 Mtx structure stores:
 * - First 32 bytes: integer parts as 4x4 s16
 * - Next 32 bytes: fraction parts as 4x4 u16
 *
 * @param src Source float matrix (4x4 = 16 floats)
 * @param dst Destination N64 Mtx (64 bytes)
 */
void guMtxF2L(f32 src[4][4], u16 *dst) {
    s32 i, j;
    f32 scale = 65536.0f;  /* 0x47800000 */
    s32 intPart, fracPart;
    u16 *intDst = dst;
    u16 *fracDst = dst + 16;  /* Offset by 32 bytes (16 u16s) */

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            /* Convert two floats at a time */
            s32 val0 = (s32)(src[i][j * 2 + 0] * scale);
            s32 val1 = (s32)(src[i][j * 2 + 1] * scale);

            /* Pack integer parts: high 16 bits of each value */
            *intDst++ = ((val1 >> 16) & 0xFFFF) | ((val0 >> 16) << 16 & 0xFFFF0000);

            /* Pack fraction parts: low 16 bits of each value */
            *fracDst++ = ((val1 & 0xFFFF) << 16) | (val0 & 0xFFFF);
        }
    }
}

/**
 * Convert N64 fixed-point matrix to float
 * (0x800092E0 - guMtxL2F / mtx_to_mtxf)
 *
 * Converts N64's 16.16 fixed-point Mtx format back to floats.
 * The scale factor is 1/65536.0 (0x37800000).
 *
 * @param src Source N64 Mtx (64 bytes)
 * @param dst Destination float matrix (4x4 = 16 floats)
 */
void guMtxL2F(u16 *src, f32 dst[4][4]) {
    s32 i, j;
    f32 invScale = 1.0f / 65536.0f;  /* 0x37800000 */
    u16 *intSrc = src;
    u16 *fracSrc = src + 16;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            u32 intPacked = *(u32 *)intSrc;
            u32 fracPacked = *(u32 *)fracSrc;

            /* Unpack first value */
            s32 int0 = (s32)(intPacked & 0xFFFF0000) | (fracPacked >> 16);
            /* Unpack second value */
            s32 int1 = ((s32)(intPacked << 16)) | (fracPacked & 0xFFFF);

            dst[i][j * 2 + 0] = (f32)int0 * invScale;
            dst[i][j * 2 + 1] = (f32)int1 * invScale;

            intSrc += 2;
            fracSrc += 2;
        }
    }
}

/**
 * Initialize N64 identity matrix (fixed-point)
 * (0x8000941C - guMtxIdent)
 *
 * Creates an identity matrix directly in N64 Mtx format.
 * Equivalent to: guMtxIdentF(temp); guMtxF2L(temp, mtx);
 *
 * @param mtx Pointer to N64 Mtx structure (64 bytes)
 */
void guMtxIdent(u16 *mtx) {
    f32 temp[4][4];

    guMtxIdentF(temp);
    guMtxF2L(temp, mtx);
}
