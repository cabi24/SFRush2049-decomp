/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * 3x3 matrix product, row-vector convention: out[i] = in[i] * m for the
 * three rows of `in` (out[i][j] = sum_k in[i][k] * m[k][j]).  Leaf, no
 * globals.  Called twice in a row by camera_first_person (0x800BF838).
 * No arcade ancestor identified.  Also matches at -O2.
 * Operand order matters: every product is written in[i][k] * m[k][j]
 * (m[2][j] * in[i][2] for the third term swaps two loads per row).
 */
typedef float f32;
typedef int s32;

void func_800BF780(f32 m[3][3], f32 in[3][3], f32 out[3][3]) {
    s32 i;

    for (i = 0; i < 3; i++) {
        out[i][0] = in[i][0] * m[0][0] + in[i][1] * m[1][0] + in[i][2] * m[2][0];
        out[i][1] = in[i][0] * m[0][1] + in[i][1] * m[1][1] + in[i][2] * m[2][1];
        out[i][2] = in[i][0] * m[0][2] + in[i][1] * m[1][2] + in[i][2] * m[2][2];
    }
}
