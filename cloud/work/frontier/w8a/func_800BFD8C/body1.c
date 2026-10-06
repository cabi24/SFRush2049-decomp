
f32 sinf(f32);

void func_800BFD8C(f32 frac, f32 *q1, f32 *q2, f32 *qr)
{
    f32 dp, theta;
    f32 sint, isint;
    f32 X1, X2;
    s32 i;

    if (frac < .001f) {
        qr[0] = q1[0], qr[1] = q1[1], qr[2] = q1[2], qr[3] = q1[3];
        return;
    } else if (frac > .999f) {
        qr[0] = q2[0], qr[1] = q2[1], qr[2] = q2[2], qr[3] = q2[3];
        return;
    }
    dp = (q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3]) * .999f;
    if (dp < 0.0f) {
        qr[0] = -q2[0];
        qr[1] = -q2[1];
        qr[2] = -q2[2];
        qr[3] = -q2[3];
        dp = -dp;
    } else {
        qr[0] = q2[0], qr[1] = q2[1], qr[2] = q2[2], qr[3] = q2[3];
    }
    if (dp < .98f) {
        theta = select_screen_update(dp);
        sint = sinf(theta);
        isint = 1.0f / sint;
        X1 = sinf((1.0f - frac) * theta) * isint;
        X2 = sinf(frac * theta) * isint;
        for (i = 0; i < 4; i++) {
            qr[i] = qr[i] * X2 + q1[i] * X1;
        }
    } else {
        for (i = 0; i < 4; i++) {
            X1 = qr[i] - q1[i];
            if (X1 > 1.0f)
                X1 -= 2.0f;
            if (X1 < -1.0f)
                X1 += 2.0f;
            qr[i] = q1[i] + frac * (X1);
            if (qr[i] > 1.0f)
                qr[i] -= 2.0f;
            else if (qr[i] < -1.0f)
                qr[i] += 2.0f;
        }
    }
}
