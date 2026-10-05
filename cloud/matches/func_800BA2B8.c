/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800BA2B8(gate, path): index of the path point where the closed point list D_8012E5E8[path]
 * crosses the gate line of D_80151CE8[gate] (0x50-byte records: pos[3] at 0x0C, direction at 0x18,
 * integer radius at 0x24). Walks the points (8 bytes: s16 x, y, z, flag) with wrap-around, tracks the
 * nearest one in the XZ plane, and stops at the first point where this and the previous point are both
 * within the gate radius (squared distance compared with the unsquared integer radius) and the side of the gate line changed.
 * Returns the nearest point when the whole list was walked without a crossing, else the crossing index.
 * No arcade ancestor proven (N64 checkpoint/path set-up; arcade init_cp_data is the closest relative).
 *
 * Frontier wave 2 (w2c). Code identical; own literal 1e20f = 0x60AD78EC equals the retail word at
 * 0x80123DFC (checked by hand; the unpatched scorer reports it as 2 unverified section-relative relocs).
 * Shaping quirks, all compile-affecting:
 *   - lastDist and lastSign are read before they are first written (guarded by n >= 0), as in retail,
 *     which loads both from their stack homes before the loop;
 *   - `f32 dot` is unused: it supplies the 4-byte slot that puts the s16 locals at sp+16..22;
 *   - the side test is the inline expression, sums written x-term first;
 *   - `sign != (s32) lastSign` (cast on the right operand) gives retail's `bne sign,lastSign`; without
 *     the cast uopt canonicalises both spellings to `bne lastSign,sign`;
 *   - the for-increment is `i++, n++`.
 */
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
    s16 sign;

    dir[0] = D_80151CE8[gate].dir[0];
    dir[1] = D_80151CE8[gate].dir[2];
    best = 1e20f;
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
        if (n >= 0 && dist <= D_80151CE8[gate].radius && lastDist <= D_80151CE8[gate].radius && sign != (s32) lastSign) {
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
