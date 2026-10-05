typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;

typedef struct GameCar {
    u8 pad0[8];
    f32 RWR[3];             /* 0x08 */
    u8 pad14[940];
} GameCar;                  /* 0x3B8 */

typedef struct ModelDat {
    u8 pad0[0x7EA];
    s8 collidable;          /* 0x7EA */
    u8 pad7EB[0x808 - 0x7EB];
} ModelDat;                 /* 0x808 */

extern GameCar player_array[];
extern ModelDat D_8014A250[];

s32 func_8010C588(s16 *slotp, f32 *pos, f32 *radiusp, f32 *dist)
{
    f32 dsq, gap;
    f32 p[3];
    f32 vec[3];
    s16 slot = *slotp;
    f32 radius = *radiusp;
    GameCar *gc;

    if (D_8014A250[slot].collidable == 0) {
        return 0;
    }
    p[0] = pos[0];
    p[1] = pos[1];
    p[2] = pos[2];
    gc = &player_array[slot];
    vec[0] = gc->RWR[0] - p[0];
    vec[1] = gc->RWR[1] - p[1];
    vec[2] = gc->RWR[2] - p[2];
    dsq = vec[2] * vec[2] + vec[0] * vec[0];
    radius += 3.5f;
    gap = dsq - radius * radius;
    if (dist) {
        *dist = gap;
    }
    if (gap > 0.0f) {
        return 0;
    }
    if (vec[1] > -2.0f && vec[1] < 4.0f) {
        return 1;
    }
    return 0;
}
