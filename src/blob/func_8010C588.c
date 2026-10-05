/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010C588: N64 OverlapTarget (arcade game/targets.c OverlapTarget(slot, pos, radius, *dist)),
 * reached through a function pointer (no direct callers). Sibling of
 * func_8010C448/func_8010C588 (identical bodies except the upper height limit: 18.0f / 4.0f).
 *   returns 0 unless model[slot].collidable (D_8014A250, 0x808-byte MODELDAT, +0x7EA);
 *   copies pos into a local array (veccopy), vec = game_car[slot].RWR - pos (vecsub);
 *   N64 change: the distance test is horizontal only (x, z); radius += CAR_RADIUS (3.5f);
 *   gap = dsq - radius^2, stored through dist when non-null; returns 0 when gap > 0;
 *   N64 addition: then requires -2.0f < vec[1] < 4.0f.
 * slot and radius arrive by pointer (first s16 of the record / f32 *).
 * Shaping quirks: `gc = player_array; gc += slot;` (not &player_array[slot]) -- the typed
 *   &array[i] form gives ugen a .noalias for the car pointer and as1 then hoists the RWR loads
 *   above the local-array stores; vec is declared before p (p at sp+0, vec at sp+12); the dot is
 *   written x*x + z*z.  Also MATCH at -O2.  Whole-program unit: EQUAL.
 */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32; typedef float f32;

typedef struct GameCar {
    u8 pad0[8];
    f32 RWR[3];             /* 0x08 */
    u8 pad14[932];
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
    f32 vec[3];
    f32 p[3];
    s16 slot = *slotp;
    f32 radius = *radiusp;
    GameCar *gc;

    if (D_8014A250[slot].collidable == 0) {
        return 0;
    }
    p[0] = pos[0];
    p[1] = pos[1];
    p[2] = pos[2];
    gc = player_array;
    gc += slot;
    vec[0] = gc->RWR[0] - p[0];
    vec[1] = gc->RWR[1] - p[1];
    vec[2] = gc->RWR[2] - p[2];
    dsq = vec[0] * vec[0] + vec[2] * vec[2];
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
