/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef int s32;
typedef unsigned int u32;

typedef struct {
    f32 rot[3][3];
    f32 pos[3];
    char pad30[0x68];
} View; /* 0x98 */

extern View D_80150B70[];

extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_8009D708(s32 idx, View *obj, u32 *m, f32 scale, s32 absolute) {
    f32 x, y, z;
    f32 dist;
    f32 s;
    s32 a, b;
    s32 xi, yi, zi, wi;

    if (absolute) {
        x = obj->pos[0];
        y = obj->pos[1];
        z = obj->pos[2];
    } else {
        x = obj->pos[0] - D_80150B70[idx].pos[0];
        y = obj->pos[1] - D_80150B70[idx].pos[1];
        z = obj->pos[2] - D_80150B70[idx].pos[2];
    }
    dist = sqrtf(x * x + y * y + z * z);
    if (dist > 1600.0f) {
        s = 1600.0f / dist;
        x *= s;
        y *= s;
        z *= s;
    } else {
        s = 1.0f;
    }
    xi = x * 1048576.0f;
    yi = y * 1048576.0f;
    zi = z * 1048576.0f;
    wi = scale * 65536.0f;
    m[6] = (xi & 0xFFFF0000) | ((u32)yi >> 16);
    m[14] = (xi << 16) | (yi & 0xFFFF);
    m[7] = (zi & 0xFFFF0000) | ((u32)wi >> 16);
    m[15] = (zi << 16) | (wi & 0xFFFF);
    s *= 65536.0f;
    a = obj->rot[0][0] * s;
    b = obj->rot[0][1] * s;
    m[0] = (a & 0xFFFF0000) | ((u32)b >> 16);
    m[8] = (a << 16) | (b & 0xFFFF);
    a = obj->rot[0][2] * s;
    m[1] = a & 0xFFFF0000;
    m[9] = a << 16;
    a = obj->rot[1][0] * s;
    b = obj->rot[1][1] * s;
    m[2] = (a & 0xFFFF0000) | ((u32)b >> 16);
    m[10] = (a << 16) | (b & 0xFFFF);
    a = obj->rot[1][2] * s;
    m[3] = a & 0xFFFF0000;
    m[11] = a << 16;
    a = obj->rot[2][0] * s;
    b = obj->rot[2][1] * s;
    m[4] = (a & 0xFFFF0000) | ((u32)b >> 16);
    m[12] = (a << 16) | (b & 0xFFFF);
    a = obj->rot[2][2] * s;
    m[5] = a & 0xFFFF0000;
    m[13] = a << 16;
}
