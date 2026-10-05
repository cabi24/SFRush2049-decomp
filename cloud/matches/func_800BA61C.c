/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* same words at -O2 */
/*
 * func_800BA61C @ 0x800BA61C, 424 bytes: path point for a checkpoint-like record.
 * Walks the track path points (PathPt D_801407F4[], count u16 D_801407F0, wrapping) and returns
 * the first point where the record D_80151CE8[arg0] (0x50 bytes: position +0x0C/+0x14, heading
 * +0x18/+0x20, s32 radius +0x24) is passed: this and the previous point are both within `radius`
 * (squared distances are compared with the radius as written) and the heading side changes sign.
 * If no such point exists it returns the nearest point. N64 code; the arcade equivalent is the
 * linear track-centre search in InitCPS (checkpoint.c), not a direct ancestor.
 *
 * State: code identical (106/106, EQUAL in the whole-program unit); the one own literal 1e20f
 * (0x60AD78EC at 0x80123E00) is unverified by the unpatched scorer, checked by hand.
 * With `extern f32 D_80123E00` instead of the literal the scorer prints strict MATCH.
 *
 * Shaping facts (each needed):
 * - two never-used locals: a 4-byte one declared before prevDist (without it 14 words) and an
 *   s16 before prevSide (without it 4 words). They only place the frame slots (56-byte frame,
 *   prevDist at sp+48, prevSide at sp+20); probably leftovers of removed debug code;
 * - the record is indexed directly (no local pointer); d and dir are two-element arrays;
 * - `for (i = -1, j = 0; ...; j++, i++)`, distance computed before the side test.
 * Earlier work: cloud/work/s20261004/C (found the body and the two slots).
 */
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

s16 func_800BA61C(s16 arg0) {
    f32 unused0;
    f32 prevDist;
    f32 d[2];
    f32 dir[2];
    f32 minDist;
    f32 dist;
    s16 unused1;
    s16 prevSide;
    s16 best;
    s16 side;
    s16 i;
    s16 j;

    dir[0] = D_80151CE8[arg0].dirx;
    dir[1] = D_80151CE8[arg0].dirz;
    minDist = 1e20f;
    for (i = -1, j = 0; i < D_801407F0; j++, i++) {
        if (j == D_801407F0) {
            j = 0;
        }
        d[0] = D_801407F4[j].x - D_80151CE8[arg0].x;
        d[1] = D_801407F4[j].z - D_80151CE8[arg0].z;
        dist = d[0] * d[0] + d[1] * d[1];
        if (d[0] * dir[0] + d[1] * dir[1] < 0.0f) {
            side = -1;
        } else {
            side = 1;
        }
        if (dist < minDist) {
            minDist = dist;
            best = j;
        }
        if (i >= 0 && dist <= D_80151CE8[arg0].radius && prevDist <= D_80151CE8[arg0].radius && side != prevSide) {
            break;
        }
        prevDist = dist;
        prevSide = side;
    }
    if (i == D_801407F0) {
        return best;
    }
    return j;
}
