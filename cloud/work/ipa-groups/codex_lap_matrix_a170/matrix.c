/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
void func_800A61B0(f32 *arg0, f32 *arg1, f32 *arg2) {
    arg1[0] = (arg0[0] * arg2[0] + arg0[1] * arg2[1]) + arg0[2] * arg2[2];
    arg1[1] = (arg0[0] * arg2[3] + arg0[1] * arg2[4]) + arg0[2] * arg2[5];
    arg1[2] = (arg0[0] * arg2[6] + arg0[1] * arg2[7]) + arg0[2] * arg2[8];
}
