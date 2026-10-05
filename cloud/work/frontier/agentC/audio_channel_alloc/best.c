/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * Historical label audio_channel_alloc is misleading: this returns the
 * horizontal path length between two path-point indices of the track graph
 * D_801407F0 (u16 count, 6-byte s16[3] points), walking successors with
 * func_800B9F60. When `to` is behind `from` the walk runs to the end of the
 * path and continues from the loop point D_80151CE8[D_80151CE8[0].unk2].unk2E;
 * it returns -1 when `to` lies before that loop point. Arcade relative: the
 * path_dist accumulation in reference/repos/rushtherock/game/scp.c.
 *
 * The y delta is computed and then overwritten with 0 (retail stores both).
 * No compile-shaping quirks: only the declaration order of the locals
 * (which fixes the stack offsets) was chosen to fit the frame.
 */
typedef float f32;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef signed char s8;

typedef struct {
    s16 pos[3];
} PathPoint;

typedef struct {
    u16 count;
    PathPoint *points;
} PathGraph;

typedef struct {
    s16 unk0;
    s16 unk2;
    char pad4[0x2A];
    s16 unk2E;
    char pad30[0x20];
} Track; /* 0x50 */

extern PathGraph D_801407F0;
extern Track D_80151CE8[];

extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)
void func_800B9F60(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3);

f32 audio_channel_alloc(s32 from, s32 to) {
    s32 i;
    s32 next;
    s32 end;
    f32 dist;
    f32 d[3];

    if (to < from && to < D_80151CE8[D_80151CE8[0].unk2].unk2E) {
        return -1.0f;
    }
    if (to < from) {
        end = D_801407F0.count;
    } else {
        end = to;
    }
    dist = 0.0f;
    for (i = from; i < end; i++) {
        func_800B9F60(-1, i, 0, &next);
        d[0] = D_801407F0.points[next].pos[0] - D_801407F0.points[i].pos[0];
        d[1] = D_801407F0.points[next].pos[1] - D_801407F0.points[i].pos[1];
        d[2] = D_801407F0.points[next].pos[2] - D_801407F0.points[i].pos[2];
        d[1] = 0.0f;
        dist += sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
    }
    if (end != to) {
        for (i = D_80151CE8[D_80151CE8[0].unk2].unk2E; i < to; i++) {
            func_800B9F60(-1, i, 0, &next);
            d[0] = D_801407F0.points[next].pos[0] - D_801407F0.points[i].pos[0];
            d[1] = D_801407F0.points[next].pos[1] - D_801407F0.points[i].pos[1];
            d[2] = D_801407F0.points[next].pos[2] - D_801407F0.points[i].pos[2];
            d[1] = 0.0f;
            dist += sqrtf(d[0] * d[0] + d[1] * d[1] + d[2] * d[2]);
        }
    }
    return dist;
}
