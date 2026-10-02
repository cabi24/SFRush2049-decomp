/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
void func_8008C544(f32 *v, f32 *out, f32 *m) {
 out[0] = v[0]*m[0] - v[1]*m[3] - v[2]*m[6];
 out[1] = (-v[0]*m[1] + v[1]*m[4]) + v[2]*m[7];
 out[2] = (-v[0]*m[2] + v[1]*m[5]) + v[2]*m[8];
}
