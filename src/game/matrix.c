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
 * Note the SDK argument order: destination floats first, source Mtx second.
 *
 * MATCHING: 0x800092E0 (asm/us/9DE0.s), IDO -O2
 * Source: ultralib src/gu/mtxutil.c:guMtxL2F @ e24c8367 (score 0 via conveyor)
 *
 * @param mf Destination float matrix (4x4 = 16 floats)
 * @param m Source N64 Mtx (64 bytes: 32 integer, 32 fraction)
 */
void guMtxL2F(f32 mf[4][4], Mtx *m) {
    s32 i, j;
    u32 e1, e2;
    u32 *ai, *af;
    s32 q1, q2;

    ai = (u32 *)&(*m)[0][0];
    af = (u32 *)&(*m)[2][0];

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            e1 = (*ai & 0xffff0000) | ((*af >> 16) & 0xffff);
            e2 = ((*(ai++) << 16) & 0xffff0000) | (*(af++) & 0xffff);
            q1 = *(s32 *)&e1;
            q2 = *(s32 *)&e2;

            mf[i][j * 2] = (f32)q1 * (1.0f / (f32)0x00010000);
            mf[i][j * 2 + 1] = (f32)q2 * (1.0f / (f32)0x00010000);
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
