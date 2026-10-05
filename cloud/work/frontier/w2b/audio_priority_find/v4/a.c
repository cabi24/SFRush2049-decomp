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
    r = D_801407F0.routes;
    r += route;
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
