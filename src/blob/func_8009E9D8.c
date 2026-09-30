/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef float f32;
extern f32 D_801613F0[];
extern f32 D_80123B00;
float sqrtf(float);
#pragma intrinsic (sqrtf)

void func_8009E9D8(f32 *m) {
    f32 a, b, len;

    a = D_801613F0[2];
    b = D_801613F0[10];
    len = sqrtf(a * a + b * b);
    if (len < D_80123B00) {
        a = 0.0f;
        b = 1.0f;
    } else {
        a /= len;
        b /= len;
    }
    m[0] = -b;
    m[4] = 0.0f;
    m[8] = -a;
    m[1] = 0.0f;
    m[9] = 0.0f;
    m[2] = a;
    m[6] = 0.0f;
    m[10] = -b;
    m[5] = 1.0f;
}
