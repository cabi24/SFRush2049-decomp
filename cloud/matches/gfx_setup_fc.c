/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * gfx_setup_fc — WYawUV(angle, uv) variant (historical label is wrong): rows uv[i][0]/uv[i][2], epsilon 0.001f; N64 sign is the reverse of the arcade (ut = a*cos + b*sin; b = b*cos - a*sin).
 * Same family as func_800B5898 (arcade LIB/fmath.c *UV rotations): rotate a 3x3 unit-vector
 * matrix by angle when |angle| > epsilon. Quirks: epsilon literal is a float (c.lt.s);
 * operand order of each sum/difference follows the arcade spelling and fixes the mul.s order.
 */
typedef float F32;
extern F32 sinf(F32);
extern F32 cosf(F32);

void gfx_setup_fc(F32 angle, F32 uv[][3]) {
    F32 ut, sint, cost;
    int i;

    if ((angle < -0.001f) || (angle > 0.001f)) {
        sint = sinf(angle);
        cost = cosf(angle);
        for (i = 0; i < 3; i++) {
            ut       = uv[i][0]*cost + uv[i][2]*sint;
            uv[i][2] = uv[i][2]*cost - uv[i][0]*sint;
            uv[i][0] = ut;
        }
    }
}
