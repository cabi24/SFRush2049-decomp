/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Exact function definitions from base f2e8380 src/blob/{func_8008B32C,
 * func_800B5898,func_800B5940,func_800D8154}.c; only required typedefs and
 * sinf/cosf declarations accompany them. Existing context, no new credit.
 */
typedef float f32; typedef float F32; typedef signed int s32;
F32 sinf(F32);F32 cosf(F32);
/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Scale each element of a 3-by-3 matrix, preserving row-major store order. */
void func_8008B32C(float src[3][3], float dst[3][3], float scale)
{
 int i, j;
 for (i = 0; i < 3; i++) {
  for (j = 0; j < 3; j++) {
   dst[i][j] = src[i][j] * scale;
  }
 }
}


void func_800B5898(F32 angle, F32 uv[][3]) {
    F32 ut, sint, cost;
    int i;

    if ((angle < -0.001f) || (angle > 0.001f)) {
        sint = sinf(angle);
        cost = cosf(angle);
        for (i = 0; i < 3; i++) {
            ut       = uv[i][1]*cost - uv[i][2]*sint;
            uv[i][2] = uv[i][2]*cost + uv[i][1]*sint;
            uv[i][1] = ut;
        }
    }
}


void func_800B5940(F32 angle, F32 uv[][3]) {
    F32 ut, sint, cost;
    int i;

    if ((angle < -0.001f) || (angle > 0.001f)) {
        sint = sinf(angle);
        cost = cosf(angle);
        for (i = 0; i < 3; i++) {
            ut       = uv[i][0]*cost - uv[i][1]*sint;
            uv[i][1] = uv[i][1]*cost + uv[i][0]*sint;
            uv[i][0] = ut;
        }
    }
}


s32 func_800D8154(s32 arg0) {
    s32 var_v1;

    var_v1 = 1;
    if (arg0 == 8) {
        var_v1 = 0;
    }
    return var_v1;
}

