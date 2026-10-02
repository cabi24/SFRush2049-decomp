/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
extern f32 D_80123888;
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
f32 func_8008B424(f32 *v) {
    f32 z = v[2];
    f32 x = v[0];
    f32 y = v[1];
    f32 squared = z*z + (x*x + y*y);
    if (squared < D_80123888) squared = D_80123888;
    return 1.0f / sqrtf(squared);
}
