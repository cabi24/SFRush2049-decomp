typedef signed char s8; typedef int s32; typedef float f32;
extern s8 g;
extern s32 K;
extern s32 f(s32);
extern f32 src[100][3];
extern s32 out[100];
void inner(void)
{
    s32 i;
    f32 v[4];
    for (g = 0; g <= 5; g++) {
        if (!f(g)) continue;
        for (i = 0; i < K; i++) {
            v[0] = src[i][0]; v[1] = src[i][1]; v[2] = src[i][2]; v[3] = src[i][0] + src[i][1];
            if (i == K - 1) { v[1] += 0.1f; v[3] += 0.1f; }
            v[0] *= 0.01f; v[1] *= 0.01f; v[2] *= 0.01f; v[3] *= 0.01f;
            out[i] = v[0] * 16.0f + v[1] * 16.0f + v[2] + v[3];
        }
    }
}
void root(void) { inner(); }
