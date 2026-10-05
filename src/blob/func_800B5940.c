/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800B5940 — arcade WRollUV(angle, uv): rows uv[i][0..1], epsilon 0.001f; arcade signs.
 * Same family as func_800B5898 (arcade LIB/fmath.c *UV rotations): rotate a 3x3 unit-vector
 * matrix by angle when |angle| > epsilon. Quirks: epsilon literal is a float (c.lt.s);
 * operand order of each sum/difference follows the arcade spelling and fixes the mul.s order.
 */
typedef float F32;
extern F32 sinf(F32);
extern F32 cosf(F32);

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
