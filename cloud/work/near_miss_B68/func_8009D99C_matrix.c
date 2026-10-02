/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
extern Ent D_80150B70[];

/* pack two floats into the N64 fixed-point Mtx word pair */
#define HI(a, b) (((u32)(a) & 0xFFFF0000) | ((u32)(b) >> 16))
#define LO(a, b) (((u32)(a) << 16) | ((u32)(b) & 0xFFFF))

s32 func_8009D99C(s32 idx, f32 src[][4], f32 *pos, Mtx *out, f32 w, s32 absolute) {
    f32 x, y, z;
    s32 a, b, c, d;
    u32 *o = (u32 *)out;
    f32 (*s)[4] = src;

    if (absolute != 0) {
        x = pos[0];
        y = pos[1];
        z = pos[2];
    } else {
        x = pos[0] - D_80150B70[idx].pos[0];
        y = pos[1] - D_80150B70[idx].pos[1];
        z = pos[2] - D_80150B70[idx].pos[2];
    }
    if (x <= -2048.0f || x >= 2048.0f) {
        return 0;
    }
    if (y <= -2048.0f || y >= 2048.0f) {
        return 0;
    }
    if (z <= -2048.0f || z >= 2048.0f) {
        return 0;
    }
    a = (s32)(x * 1048576.0f);
    b = (s32)(y * 1048576.0f);
    c = (s32)(z * 1048576.0f);
    d = (s32)(w * 65536.0f);
    o[6] = HI(a, b);
    o[14] = LO(a, b);
    o[7] = HI(c, d);
    o[15] = LO(c, d);
    a = (s32)(s[0][0] * 65536.0f);
    b = (s32)(s[1][0] * 65536.0f);
    o[0] = HI(a, b);
    o[8] = LO(a, b);
    a = (s32)(s[2][0] * 65536.0f);
    o[1] = HI(a, 0);
    o[9] = LO(a, 0);
    a = (s32)(s[0][1] * 65536.0f);
    b = (s32)(s[1][1] * 65536.0f);
    o[2] = HI(a, b);
    o[10] = LO(a, b);
    a = (s32)(s[2][1] * 65536.0f);
    o[3] = HI(a, 0);
    o[11] = LO(a, 0);
    a = (s32)(s[0][2] * 65536.0f);
    b = (s32)(s[1][2] * 65536.0f);
    o[4] = HI(a, b);
    o[12] = LO(a, b);
    a = (s32)(s[2][2] * 65536.0f);
    o[5] = HI(a, 0);
    o[13] = LO(a, 0);
    return 1;
}
