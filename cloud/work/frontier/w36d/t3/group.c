typedef float f32; typedef int s32;
extern f32 A, B, C, D, E;
extern f32 out[8];
void inner(void)
{
    f32 a = A, b = B;
    out[0] = C + (a + b) / 2.0f;
    out[1] = C - (a + b) / 2.0f;
    out[2] = D + (a - b) / 2.0f;
}
void root(void) { inner(); }
