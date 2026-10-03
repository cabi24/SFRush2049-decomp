/* NONMATCH research: native 0x8010C448, 80 words. */
/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Offset views, not recovered complete record definitions. */
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

int func_8010C448(short *player_index, float *point, float *radius_in,
                  float *radial_error)
{
    float delta[3];
    float position[3];
    ProximityPlayer *player;
    short index;
    float radius, distance_squared;
    int result;

    index = *player_index;
    player = &player_array[index];
    radius = *radius_in;
    if (!D_8014AA3A[index].enabled) return 0;
    position[0] = point[0];
    position[1] = point[1];
    position[2] = point[2];
    delta[0] = player->position[0] - position[0];
    delta[1] = player->position[1] - position[1];
    delta[2] = player->position[2] - position[2];
    radius += 3.5f;
    distance_squared = delta[2] * delta[2] + delta[0] * delta[0];
    if (radial_error) *radial_error = distance_squared - radius * radius;
    if (distance_squared - radius * radius > 0.0f) return 0;
    result = 0;
    if (delta[1] > -2.0f && delta[1] < 18.0f) result = 1;
    return result;
}
