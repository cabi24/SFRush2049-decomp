/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
extern f32 D_80123888;
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
f32 func_8008B424(f32 *v) {
    f32 squared = (v[0]*v[0] + v[1]*v[1]) + v[2]*v[2];
    if (squared < D_80123888) squared = D_80123888;
    return 1.0f / sqrtf(squared);
}
