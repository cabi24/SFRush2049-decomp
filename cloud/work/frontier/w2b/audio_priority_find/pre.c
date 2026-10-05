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

