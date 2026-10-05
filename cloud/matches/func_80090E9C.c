/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80090E9C — YawUV(angle, uv) variant: columns uv[0][i]/uv[2][i], epsilon 0.0001f; N64 sign is the reverse of the arcade (ut = a*cos + b*sin; b = b*cos - a*sin).
 * Same family as func_800B5898 (arcade LIB/fmath.c *UV rotations): rotate a 3x3 unit-vector
 * matrix by angle when |angle| > epsilon. Quirks: epsilon literal is a float (c.lt.s);
 * operand order of each sum/difference follows the arcade spelling and fixes the mul.s order.
 */
typedef float F32;
extern F32 sinf(F32);
extern F32 cosf(F32);

void func_80090E9C(F32 angle, F32 uv[][3]) {
    F32 ut, sint, cost;
    int i;

    if ((angle < -0.0001f) || (angle > 0.0001f)) {
        sint = sinf(angle);
        cost = cosf(angle);
        for (i = 0; i < 3; i++) {
            ut       = uv[0][i]*cost + uv[2][i]*sint;
            uv[2][i] = uv[2][i]*cost - uv[0][i]*sint;
            uv[0][i] = ut;
        }
    }
}
