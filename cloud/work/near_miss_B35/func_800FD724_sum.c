/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
f32 func_800FD724(f32 *a,f32 *b) {
    f32 x=a[0]*b[3];
    f32 y=a[1]*b[4];
    f32 z=b[5]*a[2];
    return (x+y)+z;
}
