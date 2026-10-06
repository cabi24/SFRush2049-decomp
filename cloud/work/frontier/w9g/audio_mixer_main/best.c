/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * Real whole-program group (no stand-ins): audio_priority_find with its only
 * caller audio_mixer_main. Both labels are historical and misleading.
 *
 * audio_priority_find(who, route) -- CLAIMED, strict MATCH in this group.
 *   Route-crossing test: -1 when route is out of range; for a type-2 route 0
 *   for tracker 0, else -1; otherwise walks the route's s16 points (6 bytes
 *   each) and, among points within the tracker's range (squared distance
 *   against the unsquared integer, as in func_800BA2B8), returns the index of
 *   the first point whose side of the tracker's direction line differs from
 *   the first such point's side; -1 when none.
 *   It is an internal (non-kept) function with two call sites. uopt, not the
 *   ABI position, picks its argument registers: the second source parameter
 *   (route) arrives in a0 and the first (who) in a1, while the entry stores
 *   keep source order (`sw a0,28(sp)`, `sw a1,24(sp)`). Compiled kept it
 *   cannot match.
 *   Shaping: tracker fields are read through the global array (hoisted into
 *   the loop preheader), route fields through a pointer (re-read every
 *   iteration); the route record is addressed with explicit byte arithmetic,
 *   (Route *)((u8 *)routes + route * sizeof(Route)) -- &routes[route] and
 *   routes + route give the reverse temp order (2 words); `prev` is loaded
 *   before its first write, as retail does (`lh a1,20(sp)`).
 *
 * audio_mixer_main(mode) -- CONTEXT ONLY, real reconstruction, NOT matched
 *   (31/183 words; see cloud/work/frontier/w2b/RESULTS.md). For every tracker
 *   it fills the 20-entry crossing-index table (entry 0 from func_800BA61C,
 *   1..4 from func_800BA2B8(who, n-1), 5.. from audio_priority_find(who,
 *   n-5); mode < 0 = all, 0 = entry 0 plus every route, n > 0 = entry n only),
 *   then recomputes each tracker's segment length with audio_channel_alloc
 *   and the two running totals D_80152800 / D_801543AC.
 *   The trackers are 0x50-byte records that start 12 bytes into D_80151CE8
 *   (s16 header: first at +2, last at +4, count at +8); the Tracker view below
 *   keeps that 12-byte prefix so record fields carry their retail offsets.
 * No arcade ancestor identified for either (N64 checkpoint/path set-up).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} RoutePoint;

typedef struct {
    u8 type;
    char pad1[9];
    u16 num;
    RoutePoint *pts;
} Route;

typedef struct {
    char pad0[8];
    u8 count;
    char pad9[3];
    Route *routes;
} RouteSet;

typedef struct {
    s16 unk0;
    s16 first;
    s16 last;
    s16 unk6;
    s16 count;
    s16 unkA;
    f32 x;
    f32 y;
    f32 z;
    f32 dx;
    char pad1C[4];
    f32 dz;
    s32 range;
    char pad28[6];
    s16 idx[17];
} Tracker;

typedef struct {
    char pad0[0x58];
    f32 len;
} TrackerLen;

extern RouteSet D_801407F0;
extern Tracker D_80151CE8[];

s32 audio_priority_find(s16 who, s16 route) {
    s16 i;
    s16 prev;
    s16 side;
    s16 first;
    f32 d[2];
    f32 dir[2];
    Route *r;

    first = 1;
    if (route >= D_801407F0.count) {
        return -1;
    }
    r = (Route *)((u8 *)D_801407F0.routes + route * sizeof(Route));
    if (r->type == 2) {
        if (who == 0) {
            return 0;
        }
        return -1;
    }
    dir[0] = D_80151CE8[who].dx;
    dir[1] = D_80151CE8[who].dz;
    for (i = 0; i < r->num; i++) {
        d[0] = r->pts[i].x - D_80151CE8[who].x;
        d[1] = r->pts[i].z - D_80151CE8[who].z;
        if (d[0] * d[0] + d[1] * d[1] <= D_80151CE8[who].range) {
            if (d[0] * dir[0] + d[1] * dir[1] < 0.0f) {
                side = -1;
            } else {
                side = 1;
            }
            if (first) {
                prev = side;
                first = 0;
            } else if (prev != side) {
                return i;
            }
        }
    }
    return -1;
}

extern f32 D_80152800;
extern f32 D_801543AC;
s16 func_800BA61C(s16 who);
s16 func_800BA2B8(s16 who, s16 path);
f32 audio_channel_alloc(s32 from, s32 to);

#define LEN(n) (((TrackerLen *)&D_80151CE8[n])->len)

void audio_mixer_main(s16 mode) {
    s32 pad[2];
    s16 pos[3];
    s32 i;
    s32 j;
    s32 next;
    s32 done;
    s32 k;

    for (i = 0; i < D_80151CE8[0].count; i++) {
        pos[0] = D_80151CE8[i].x;
        pos[1] = D_80151CE8[i].y;
        pos[2] = D_80151CE8[i].z;
        if (mode < 0) {
            for (j = 0; j < 20; j++) {
                if (j == 0) {
                    D_80151CE8[i].idx[j] = func_800BA61C(i);
                } else if (j >= 5) {
                    D_80151CE8[i].idx[j] = audio_priority_find(i, j - 5);
                } else {
                    D_80151CE8[i].idx[j] = func_800BA2B8(i, j - 1);
                }
            }
        } else if (mode == 0) {
            D_80151CE8[i].idx[mode] = func_800BA61C(i);
            for (j = 0; j < D_801407F0.count; j++) {
                D_80151CE8[i].idx[j + 5] = audio_priority_find(i, j);
            }
        } else {
            D_80151CE8[i].idx[mode] = func_800BA2B8(i, mode - 1);
        }
    }
    D_80152800 = 0.0f;
    D_801543AC = D_80152800;
    done = 0;
    for (k = 0; k < D_80151CE8[0].count; k++) {
        if (k + 1 == D_80151CE8[0].count) {
            next = D_80151CE8[0].first;
        } else {
            next = k + 1;
        }
        LEN(k) = audio_channel_alloc(D_80151CE8[k].idx[0], D_80151CE8[next].idx[0]);
        if (k > 0 && k == D_80151CE8[0].last) {
            done = 1;
        }
        if (!done) {
            D_80152800 += LEN(k);
        }
        if (k >= D_80151CE8[0].first) {
            D_801543AC += LEN(k);
        }
    }
}
