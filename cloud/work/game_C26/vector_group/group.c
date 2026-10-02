/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef signed char s8;
typedef unsigned char u8;
extern f32 D_801247FC, D_80124800, D_80124804;
extern f32 sqrtf(f32);
#pragma intrinsic (sqrtf)
extern void func_800A61B0(f32 *, f32 *, f32 *);
void random_int(void *car, f32 *direction) {
    f32 vector[3];
    f32 output[3];
    f32 scale, sum, factor;
    f32 *dst, *src;
    if (*(s8 *)((u8 *)car + 0x640) == 0) scale = D_801247FC;
    else scale = D_80124800;
    { f32 x = direction[0], y = direction[1], z = direction[2];
    sum = z*z + (x*x + y*y); }
    if (sum < D_80124804) {
        vector[0] = scale;
        vector[1] = 0.0f;
        vector[2] = 0.0f;
    } else {
        factor = scale / sqrtf(sum);
        dst = vector;
        src = direction;
        do {
            *dst++ = *src++ * factor;
        } while (dst != vector + 3);
    }
    func_800A61B0(vector, output, (f32 *)((u8 *)car + 0x7A0));
    *(f32 *)((u8 *)car + 0x124) = output[0] + *(f32 *)((u8 *)car + 0x124);
    *(f32 *)((u8 *)car + 0x128) = output[1] + *(f32 *)((u8 *)car + 0x128);
    *(f32 *)((u8 *)car + 0x12C) = output[2] + *(f32 *)((u8 *)car + 0x12C);
}

void func_800A61B0(f32 *arg0, f32 *arg1, f32 *arg2) {
    arg1[0] = (arg0[0] * arg2[0] + arg0[1] * arg2[1]) + arg0[2] * arg2[2];
    arg1[1] = (arg0[0] * arg2[3] + arg0[1] * arg2[4]) + arg0[2] * arg2[5];
    arg1[2] = (arg0[0] * arg2[6] + arg0[1] * arg2[7]) + arg0[2] * arg2[8];
}
