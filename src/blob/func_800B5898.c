/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800B5898 — arcade LIB/fmath.c WPitchUV(angle, uv): rotate the three rows of a
 * 3x3 unit-vector matrix about the universe X axis (columns 1 and 2) when
 * |angle| > 0.001. Pasted from the arcade function; the only N64 changes are
 * float literals (-0.001f / 0.001f, retail compares with c.lt.s) and sinf/cosf
 * for fsin/fcos. Quirk: the operand order of the uv[i][2] sum
 * (uv[i][2]*cost + uv[i][1]*sint, as in the arcade) selects the mul.s order.
 */
typedef float F32;
extern F32 sinf(F32);
extern F32 cosf(F32);

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
