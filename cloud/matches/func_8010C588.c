/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * N64 target-overlap callback, derived from arcade targets.c OverlapTarget
 * and vecmath.h mveccopy/mvecsub at rushtherock 845329d7b36f5a384c5625ed9a0aef584ab46139.
 * The native callback takes pointers to the signed selector and radius,
 * snapshots the origin, uses X/Z distance with radius + 3.5, and accepts
 * vertical delta strictly between -2 and 4. Optional radial error
 * is written even when the overlap test fails. Disabled models return zero.
 *
 * Preserve the donor's dsq/gap-before-vector declaration order and its
 * distance-then-radius operation order: both affect stock IDO code generation.
 * All locals are used. No dummy storage, reads, volatile, or helper stand-ins.
 * Types below are observed offset views, not full recovered record layouts.
 * See cloud/work/frontier/dot_target_overlap_20261005/README.md.
 */
#define mvecsub(a,b,r) {r[0]=a[0]-b[0]; r[1]=a[1]-b[1]; r[2]=a[2]-b[2];}
#define mveccopy(a,r) {r[0]=a[0]; r[1]=a[1]; r[2]=a[2];}
typedef struct ProximityEnabled {
    signed char enabled;
    unsigned char unknown[0x807];
} ProximityEnabled;
typedef struct ProximityPlayer {
    unsigned char unknown0[8];
    float position[3];
    unsigned char unknown14[0x3B8 - 0x14];
} ProximityPlayer;
extern ProximityEnabled D_8014AA3A[];
extern ProximityPlayer player_array[];

int func_8010C588(short *player_index, float *point, float *radius_in,
                  float *radial_error)
{
    float distance_squared, gap;
    float delta[3], position[3];
    ProximityPlayer *player;
    short index;
    float radius;

    index = *player_index;
    player = &player_array[index];
    radius = *radius_in;
    if (!D_8014AA3A[index].enabled) return 0;
    mveccopy(point,position);
    mvecsub(player->position,position,delta);
    distance_squared = delta[0] * delta[0] + delta[2] * delta[2];
    radius += 3.5f;
    gap = distance_squared - radius * radius;
    if (radial_error) *radial_error = gap;
    if (gap > 0.0f) return 0;
    if (delta[1] > -2.0f && delta[1] < 4.0f) return 1;
    return 0;
}
