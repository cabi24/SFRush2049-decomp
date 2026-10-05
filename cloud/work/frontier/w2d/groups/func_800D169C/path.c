/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800D169C: find the path point of the track graph D_801407F0 (u16 count, 6-byte s16[3] points) nearest
 * to car 0, step back to the even point that starts its segment (one back for an odd index; two back when the
 * car is behind an even point, by the sign of the dot product with the segment direction), and raise the
 * progress high-water mark D_8014AA14 to that index.  Arcade relative: the closest-path-point scan in
 * reference/repos/rushtherock/game/resurrect.c (c_dist = 1e20; dx*dx + dy*dy + dz*dz; close_index), not a
 * line-for-line ancestor.
 * Whole-program structure this depends on:
 *   - func_800D18C8 (the caller-less `jr ra; nop` stub right after this function) is the high-water update,
 *     inlined at both call sites.  Written out inline the four accesses to D_8014AA14 get a base register;
 *     through the inlined helper they stay `lui; lh/sh %lo` as in retail;
 *   - D_80152818 (the 6-entry car array) is defined in this unit, not extern: as1 then shares one `lui at`
 *     between the first two of the three position loads (precedent: D_801551E8 in codex_func_800B7438).
 * Quirks: close_index is read uninitialised when no point beats 1e20 (retail loads its stack slot before
 * the loop); `dx` is reused for the dot product (same register as the loop's dx); the second half reads the
 * position through `car` while the first uses the array, which keeps the two sets of loads distinct;
 * `p` is an s16 pointer to the point's coordinates.
 * Own rodata: the float 1e20 (0x60AD78EC at 0x80124174).
 */
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
Car D_80152818[6];
extern s16 D_8014AA14;

void func_800D18C8(s32 index);

void func_800D169C(void) {
    s32 i;
    s32 close_index;
    f32 x, y, z;
    f32 dx, dy, dz;
    f32 dist;
    f32 c_dist;
    f32 delta1[3];
    f32 delta2[3];
    s16 *p;
    Car *car = D_80152818;

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
        func_800D18C8(close_index);
        return;
    }
    p = D_801407F0.points[close_index].pos;
    delta1[0] = (f32) p[3] - (f32) p[0];
    delta1[1] = (f32) p[4] - (f32) p[1];
    delta1[2] = (f32) p[5] - (f32) p[2];
    delta2[0] = car->pos[0] - p[0];
    delta2[1] = car->pos[1] - p[1];
    delta2[2] = car->pos[2] - p[2];
    dx = delta1[0] * delta2[0] + delta1[1] * delta2[1] + delta1[2] * delta2[2];
    if (dx < 0.0f && close_index > 0) {
        close_index -= 2;
    }
    func_800D18C8(close_index);
}

void func_800D18C8(s32 index) {
    if (D_8014AA14 < index) {
        D_8014AA14 = index;
    }
}
