extern f32 D_80123B0C;
extern f32 D_80123B10;
f32 sinf(f32);
f32 cosf(f32);
typedef struct { f32 x, y, z; } V3;
void gfx_setup_fc(f32 ang, V3 *v) {
    f32 s, c;
    s32 i;
    if ((ang < D_80123B0C) | (D_80123B10 < ang)) {
        s = sinf(ang);
        c = cosf(ang);
        for (i = 0; i < 3; i++) {
            f32 x = v->x;
            f32 z = v->z;
            v->x = z * s + x * c;
            v->z = z * c - x * s;
            v++;
        }
    }
}
