/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
extern f32 sqrtf(f32);
#pragma intrinsic (sqrtf)
f32 func_8008B3C8(f32 *v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

