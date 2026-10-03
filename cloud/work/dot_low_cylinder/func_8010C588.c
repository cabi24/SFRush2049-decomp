/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* NONMATCH research. Types describe observed strides, not proven whole records. */
typedef struct EnabledView2056 {
    signed char enabled;
    unsigned char unknown[2055];
} EnabledView2056;
typedef float Vec3[3];
typedef struct PlayerView952 {
    unsigned char unknown0[8];
    Vec3 position;
    unsigned char unknown20[932];
} PlayerView952;
extern EnabledView2056 D_8014AA3A[];
extern PlayerView952 D_80152818[];
int func_8010C588(short *selector, const float *origin,
                  const float *base_radius, float *radial_error)
{
    Vec3 difference, position;
    PlayerView952 *player;
    short index;
    int result;
    float radius, square, error;
    index = *selector;
    radius = *base_radius;
    player = &D_80152818[index];
    if (D_8014AA3A[index].enabled == 0) return 0;
    position[0] = origin[0];
    position[1] = origin[1];
    position[2] = origin[2];
    difference[0] = player->position[0] - position[0];
    difference[1] = player->position[1] - position[1];
    difference[2] = player->position[2] - position[2];
    radius += 3.5f;
    square = difference[2] * difference[2] + difference[0] * difference[0];
    error = square - radius * radius;
    if (radial_error != 0) *radial_error = error;
    if (error > 0.0f) return 0;
    result = 0;
    if (difference[1] > -2.0f && difference[1] < 4.0f) result = 1;
    return result;
}
