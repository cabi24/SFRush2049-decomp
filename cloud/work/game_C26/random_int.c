/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef signed char s8;
typedef unsigned char u8;
extern f32 D_801247FC, D_80124800, D_80124804;
extern f32 sqrtf(f32);
#pragma intrinsic (sqrtf)
extern void func_800A61B0(f32 *, f32 *, void *);
void random_int(void *car, f32 *direction) {
    f32 output[3];
    f32 vector[3];
    f32 scale, sum, factor;
    f32 *dst, *src;
    if (*(s8 *)((u8 *)car + 0x640) == 0) scale = D_801247FC;
    else scale = D_80124800;
    sum = direction[2]*direction[2] + (direction[0]*direction[0] + direction[1]*direction[1]);
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
    func_800A61B0(vector, output, (u8 *)car + 0x7A0);
    *(f32 *)((u8 *)car + 0x124) += output[0];
    *(f32 *)((u8 *)car + 0x128) += output[1];
    *(f32 *)((u8 *)car + 0x12C) += output[2];
}
