/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

typedef struct PathPoint {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ s16 flag;
} PathPoint;

typedef struct PathDesc {
    /* 0x0 */ u16 count;
    /* 0x2 */ u16 unk2;
    /* 0x4 */ PathPoint *points;
} PathDesc;

typedef struct Gate {
    /* 0x00 */ u8 pad0[12];
    /* 0x0C */ f32 pos[3];
    /* 0x18 */ f32 dir[3];
    /* 0x24 */ s32 radius;
    /* 0x28 */ u8 pad28[40];
} Gate; /* 0x50 */

extern Gate D_80151CE8[];
extern PathDesc D_8012E5E8[];

s16 func_800BA2B8(s16 gate, s16 path)
{
    f32 best;
    f32 lastDist;
    f32 delta[2];
    f32 dir[2];
    f32 dist;
    f32 dot;
    s16 n;
    s16 lastSign;
    s16 closest;
    s16 i;
    s32 sign;

    dir[0] = D_80151CE8[gate].dir[0];
    dir[1] = D_80151CE8[gate].dir[2];
    best = 1e10f;
    for (n = -1, i = 0; n < D_8012E5E8[path].count; i++, n++) {
        if (i == D_8012E5E8[path].count) {
            i = 0;
        }
        delta[0] = D_8012E5E8[path].points[i].x - D_80151CE8[gate].pos[0];
        delta[1] = D_8012E5E8[path].points[i].z - D_80151CE8[gate].pos[2];
        dist = delta[0] * delta[0] + delta[1] * delta[1];
        if (delta[0] * dir[0] + delta[1] * dir[1] < 0.0f) {
            sign = -1;
        } else {
            sign = 1;
        }
        if (dist < best) {
            best = dist;
            closest = i;
        }
        if (n >= 0 && dist <= D_80151CE8[gate].radius && lastDist <= D_80151CE8[gate].radius && lastSign != sign) {
            break;
        }
        lastDist = dist;
        lastSign = sign;
    }
    if (n == D_8012E5E8[path].count) {
        return closest;
    }
    return i;
}
