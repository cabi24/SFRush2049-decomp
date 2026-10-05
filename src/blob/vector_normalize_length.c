/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * Historical label vector_normalize_length is misleading: this builds an
 * orthonormal 3x3 basis from a direction.
 *   m[2] = normalize(dir)            (vector_copy_scale = normalise src->dst)
 *   m[0] = (dir.z, 0, -dir.x), normalised; when its length (func_8008B3C8)
 *          is <= 0.01 it falls back to the constant vector D_801141C8 (1,0,0)
 *   m[1] = m[2] x m[0]
 *   m[0] = m[1] x m[2]
 * The two cross products are written out in place (a three-pointer crossprod
 * helper is not inlined by -O3 and gives a different body). No arcade ancestor
 * identified.
 *
 * 0.01f is this function's own literal (retail .rodata 0x8012388C =
 * 0x3C23D70A); the unpatched scorer reports the two references to it as
 * unverified, the splice verifies the bytes. Also matches at -O2.
 * No compile-shaping quirks.
 */
typedef float f32;
typedef int s32;

extern f32 D_801141C8[3];
f32 func_8008B3C8(f32 *v);
void vector_copy_scale(f32 *a, f32 *b);

void vector_normalize_length(f32 *dir, f32 m[3][3]) {
    f32 len;
    f32 inv;

    m[2][0] = dir[0];
    m[2][1] = dir[1];
    m[2][2] = dir[2];
    vector_copy_scale(m[2], m[2]);
    m[0][0] = dir[2];
    m[0][1] = 0.0f;
    m[0][2] = -dir[0];
    len = func_8008B3C8(m[0]);
    if (len <= 0.01f) {
        m[0][0] = D_801141C8[0];
        m[0][1] = D_801141C8[1];
        m[0][2] = D_801141C8[2];
    } else {
        inv = 1.0f / len;
        m[0][0] *= inv;
        m[0][1] *= inv;
        m[0][2] *= inv;
    }
    m[1][0] = m[2][1] * m[0][2] - m[2][2] * m[0][1];
    m[1][1] = m[2][2] * m[0][0] - m[2][0] * m[0][2];
    m[1][2] = m[2][0] * m[0][1] - m[2][1] * m[0][0];
    m[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    m[0][1] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
    m[0][2] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
}
