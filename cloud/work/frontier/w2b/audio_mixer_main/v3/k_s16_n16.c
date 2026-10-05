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
    s16 next;
    s32 done;
    s16 k;

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
