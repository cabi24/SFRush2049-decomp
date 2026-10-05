
extern f32 D_8011F010[4];
f32 func_8009C3F8(s32 flag, f32 x)
{
    f32 g;
    f32 y;
    f32 r;
    s32 i;

    y = fabsf(x);
    i = flag;
    if (y < 2.3e-10f) {
        r = y;
    } else if (y >= 1.0f) {
        r = 1.5707964f;
    } else {
        if (y > 0.5f) {
            i = 1 - flag;
            g = ((0.5f - y) + 0.5f) / 2.0f;
            y = sqrtf(g);
            y = -(y + y);
        } else {
            g = y * y;
        }
        r = (((((-0.69674575f * g + 10.152522f) * g + -39.688862f) * g + 57.20823f) * g + -27.368494f) * g) /
            (((((g + -23.823858f) * g + 150.95271f) * g + -381.86304f) * g + 417.14432f) * g + -164.21097f) * y + y;
    }
    if (flag != 0) {
        if (x < 0.0f) {
            r = (D_8011F010[i + 2] + r) + D_8011F010[i + 2];
        } else {
            r = (D_8011F010[i] - r) + D_8011F010[i];
        }
    } else {
        r = (D_8011F010[i] + r) + D_8011F010[i];
        if (x < 0.0f) {
            r = -r;
        }
    }
    return r;
}
