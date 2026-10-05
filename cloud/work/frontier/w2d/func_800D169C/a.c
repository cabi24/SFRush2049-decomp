/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned char u8;
typedef float f32;
typedef struct Point {
    s16 pos[3];
} Point;
typedef struct Route {
    u16 count;
    Point *points;
} Route;
typedef struct Car {
    u8 opaque0[8];
    f32 pos[3];
    u8 opaque14[0x3A4];
} Car; /* 0x3B8 */
extern Route D_801407F0;
extern Car D_80152818[];
extern s16 D_8014AA14;

void func_800D169C(void) {
    s32 i;
    s32 close_index;
    f32 x, y, z;
    f32 dx, dy, dz;
    f32 dist;
    f32 c_dist;
    f32 delta1[3];
    f32 delta2[3];
    f32 dot;
    Point *p;

    x = D_80152818[0].pos[0];
    y = D_80152818[0].pos[1];
    z = D_80152818[0].pos[2];
    c_dist = 1e20;
    for (i = 0; i < D_801407F0.count; i++) {
        dx = D_801407F0.points[i].pos[0] - x;
        dy = D_801407F0.points[i].pos[1] - y;
        dz = D_801407F0.points[i].pos[2] - z;
        dist = dx * dx + dy * dy + dz * dz;
        if (dist < c_dist) {
            c_dist = dist;
            close_index = i;
        }
    }
    if (close_index & 1) {
        close_index--;
        if (D_8014AA14 < close_index) {
            D_8014AA14 = close_index;
        }
        return;
    }
    p = &D_801407F0.points[close_index];
    delta1[0] = p[1].pos[0] - p[0].pos[0];
    delta1[1] = p[1].pos[1] - p[0].pos[1];
    delta1[2] = p[1].pos[2] - p[0].pos[2];
    delta2[0] = D_80152818[0].pos[0] - p[0].pos[0];
    delta2[1] = D_80152818[0].pos[1] - p[0].pos[1];
    delta2[2] = D_80152818[0].pos[2] - p[0].pos[2];
    dot = delta1[0] * delta2[0] + delta1[1] * delta2[1] + delta2[2] * delta1[2];
    if (dot < 0.0f && close_index > 0) {
        close_index -= 2;
    }
    if (D_8014AA14 < close_index) {
        D_8014AA14 = close_index;
    }
}
