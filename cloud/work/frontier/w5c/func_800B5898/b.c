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
