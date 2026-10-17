/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
void func_8008C544(f32 *v, f32 *out, f32 (*m)[3]) {
 out[0] = v[0]*m[0][0] - v[1]*m[1][0] - v[2]*m[2][0];
 out[1] = (-v[0]*m[0][1] + v[1]*m[1][1]) + v[2]*m[2][1];
 out[2] = (-v[0]*m[0][2] + v[1]*m[1][2]) + v[2]*m[2][2];
}
