/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Historical steering_apply is a target-overlap sphere callback.
 * Algorithm/declaration/phase/return donor: targets.c:OverlapTarget in
 * rushtherock 845329d7b36f5a384c5625ed9a0aef584ab46139, lines 673-693.
 * Vector macros: game/vecmath.h:20-21 at the same pinned revision.
 *
 * N64 takes a signed selector and radius by pointer, snapshots the origin,
 * and uses a 3.5f radius margin. It returns zero for disabled models;
 * otherwise it optionally writes the squared gap and accepts gap <= 0.
 * Every local is consumed. Preserve the donor's scalar-before-vector
 * declaration order, distance-before-radius phases, and boolean return.
 * Unknown structure bytes below are observed storage offsets/strides,
 * not dummy local padding or claims of full original structure recovery.
 * See cloud/work/frontier/dot_target_sphere_20261005/README.md.
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

int steering_apply(short *player_index, float *point, float *radius_in,
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
    distance_squared = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    radius += 3.5f;
    gap = distance_squared - radius * radius;
    if (radial_error) *radial_error = gap;
    return (gap <= 0.0f);
}
