/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef signed int s32;

f32 cosf(f32);
f32 sinf(f32);
void func_800A61B0(f32 *in, f32 *out, f32 *mat);

void camera_collision_avoid(f32 *origin, f32 *a, f32 *b, f32 angle, f32 *mat, f32 *src, f32 *uvs, f32 *pos) {
    f32 p[3];

    p[0] = b[1] * a[2] - b[2] * a[1];
    p[1] = b[2] * a[0] - b[0] * a[2];
    p[2] = b[0] * a[1] - b[1] * a[0];
    p[0] = origin[0] + p[0];
    p[1] = origin[1] + p[1];
    p[2] = origin[2] + p[2];
    func_800A61B0(src + 3, uvs + 3, mat);
    uvs[0] = cosf(angle);
    uvs[1] = 0.0f;
    uvs[2] = -sinf(angle);
    uvs[6] = uvs[1] * uvs[5] - uvs[2] * uvs[4];
    uvs[7] = uvs[2] * uvs[3] - uvs[0] * uvs[5];
    uvs[8] = uvs[0] * uvs[4] - uvs[1] * uvs[3];
    uvs[0] = uvs[4] * uvs[8] - uvs[5] * uvs[7];
    uvs[1] = uvs[5] * uvs[6] - uvs[3] * uvs[8];
    uvs[2] = uvs[3] * uvs[7] - uvs[4] * uvs[6];
    func_800A61B0(p, pos, uvs);
}
