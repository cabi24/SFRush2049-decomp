/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef float f32;
typedef signed int s32;
extern f32 D_80123B04;
extern f32 D_80123B08;
f32 sinf(f32);
f32 cosf(f32);

void func_8009EA68(f32 ang, f32 *v) {
    f32 c, s, x, y;
    s32 i;

    if (ang < D_80123B04 || ang > D_80123B08) {
        s = sinf(ang);
        c = cosf(ang);
        for (i = 0; i < 3; i++) {
            x = v[i];
            y = v[i + 3];
            v[i] = x * c - y * s;
            v[i + 3] = x * s + y * c;
        }
    }
}
