/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef u32 Mtx[16];
typedef struct {
    u8 pad0[36];
    f32 pos[3];
    u8 pad1[152 - 48];
} Ent;
typedef struct {
    f32 m[9];
    f32 pos[3];
} XForm;
extern Ent D_80150B70[];
float sqrtf(float);
#pragma intrinsic (sqrtf)

/* pack two floats into the N64 fixed-point Mtx word pair */
#define HI(a, b) (((u32)(a) & 0xFFFF0000) | ((u32)(b) >> 16))
#define LO(a, b) (((u32)(a) << 16) | ((u32)(b) & 0xFFFF))

s32 func_8009D708(s32 idx, XForm *src, Mtx *out, f32 w_, s32 absolute) {
    register f32 w = w_;
    f32 x, y, z, len, sc;
    s32 a, b, c, d;
    u32 *o = (u32 *)out;

    if (absolute != 0) {
        x = src->pos[0];
        y = src->pos[1];
        z = src->pos[2];
    } else {
        x = src->pos[0] - D_80150B70[idx].pos[0];
        y = src->pos[1] - D_80150B70[idx].pos[1];
        z = src->pos[2] - D_80150B70[idx].pos[2];
    }
    len = sqrtf(x * x + y * y + z * z);
    if (len > 1600.0f) {
        sc = 1600.0f / len;
        x *= sc;
        y *= sc;
        z *= sc;
    } else {
        sc = 1.0f;
    }
    a = (s32)(x * 1048576.0f);
    b = (s32)(y * 1048576.0f);
    c = (s32)(z * 1048576.0f);
    d = (s32)(w * 65536.0f);
    o[6] = HI(a, b);
    o[14] = LO(a, b);
    o[7] = HI(c, d);
    o[15] = LO(c, d);
    sc *= 65536.0f;
    a = (s32)(src->m[0] * sc);
    b = (s32)(src->m[1] * sc);
    o[0] = HI(a, b);
    o[8] = LO(a, b);
    a = (s32)(src->m[2] * sc);
    o[1] = HI(a, 0);
    o[9] = LO(a, 0);
    a = (s32)(src->m[3] * sc);
    b = (s32)(src->m[4] * sc);
    o[2] = HI(a, b);
    o[10] = LO(a, b);
    a = (s32)(src->m[5] * sc);
    o[3] = HI(a, 0);
    o[11] = LO(a, 0);
    a = (s32)(src->m[6] * sc);
    b = (s32)(src->m[7] * sc);
    o[4] = HI(a, b);
    o[12] = LO(a, b);
    a = (s32)(src->m[8] * sc);
    o[5] = HI(a, 0);
    o[13] = LO(a, 0);
    return 1;
}
