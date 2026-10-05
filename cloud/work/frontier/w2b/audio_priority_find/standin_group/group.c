/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * PROVISIONAL (stand-in caller, not spliceable): audio_priority_find
 * (historical label, misleading) is a route-crossing test.
 *   (who, route): returns -1 when route is out of range; for a type-2 route
 *   returns 0 for tracker 0 and -1 otherwise; else walks the route's s16 points
 *   (6 bytes each) and, among points within sqrt(range) of the tracker's
 *   (x, z), returns the index of the first point whose side of the tracker's
 *   direction line (sign of the 2D dot product) differs from the first such
 *   point's side; -1 when none.
 *
 * Whole-program facts this depends on:
 *  - It is an internal (non-kept) -O3 function with two call sites in its only
 *    caller audio_mixer_main. IPA passes the SECOND source parameter (route)
 *    in a0 and the first (who) in a1; the entry stores `sw a0,28(sp)` /
 *    `sw a1,24(sp)` (homes in source order) are the evidence. Compiled as a
 *    kept function it cannot match (homes un-swapped, wider temp ring).
 *  - standin_caller below is NOT real code: audio_mixer_main is unmatched.
 *
 * Shaping notes: tracker fields are read through the global array (so uopt
 * hoists them into the loop preheader), route fields through a pointer
 * (re-read every iteration); the route record is addressed with explicit byte
 * arithmetic, (Route *)((u8 *)routes + route * sizeof(Route)), which loads the
 * base before the shift (&routes[route], routes + route and a u8 * field all
 * give the reverse temp order, 2 words off); `prev` is read
 * uninitialised by uopt's register load before the loop, as in retail.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
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
    char pad0[0xC];
    f32 x;
    char pad10[4];
    f32 z;
    f32 dx;
    char pad1C[4];
    f32 dz;
    s32 range;
    char pad28[0x50 - 0x28];
} Tracker;

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

extern s32 D_standin[];
void standin_caller(s16 a, s16 b) {
    D_standin[0] = audio_priority_find(b, a - 5);
    D_standin[1] = audio_priority_find(b, a);
}
