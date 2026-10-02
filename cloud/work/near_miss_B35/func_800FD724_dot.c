/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
f32 func_800FD724(f32 *a,f32 *b) {
    return b[5]*a[2]+(a[0]*b[3]+a[1]*b[4]);
}
