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
