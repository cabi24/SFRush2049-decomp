typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;

typedef struct PathPt {
    s16 x;
    s16 y;
    s16 z;
} PathPt;

typedef struct Obj80 {
    char pad00[0x0C];
    f32 x;          /* 0x0C */
    f32 y;          /* 0x10 */
    f32 z;          /* 0x14 */
    f32 dirx;       /* 0x18 */
    f32 pad1C;
    f32 dirz;       /* 0x20 */
    s32 radius;     /* 0x24 */
    char pad28[0x28];
} Obj80;

extern Obj80 D_80151CE8[];
extern u16 D_801407F0;
extern PathPt *D_801407F4;
extern f32 D_80123E00;

s16 func_800BA61C(s16 arg0) {
    f32 prevDist;
    f32 d[2];
    f32 dir[2];
    f32 minDist;
    f32 dist;
    s16 prevSide;
    s16 best;
    s16 side;
    s16 i;
    s16 j;
    PathPt *p;

    dir[0] = D_80151CE8[arg0].dirx;
    dir[1] = D_80151CE8[arg0].dirz;
    minDist = D_80123E00;
    i = -1;
    j = 0;
    while (i < D_801407F0) {
        if (j == D_801407F0) {
            j = 0;
        }
        p = &D_801407F4[j];
        d[0] = p->x - D_80151CE8[arg0].x;
        d[1] = p->z - D_80151CE8[arg0].z;
        if (d[0] * dir[0] + d[1] * dir[1] < 0.0f) {
            side = -1;
        } else {
            side = 1;
        }
        dist = d[0] * d[0] + d[1] * d[1];
        if (dist < minDist) {
            minDist = dist;
            best = j;
        }
        if (i >= 0 && dist <= D_80151CE8[arg0].radius && prevDist <= D_80151CE8[arg0].radius && side != prevSide) {
            break;
        }
        i++;
        j++;
        prevSide = side;
        prevDist = dist;
    }
    if (i == D_801407F0) {
        return best;
    }
    return j;
}
