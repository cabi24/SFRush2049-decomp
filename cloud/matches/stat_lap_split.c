/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Directional positional-sound request, 0x800FEA00..0x800FEC60 (608 bytes).
 * Historical name does not describe its sound-direction behavior.
 * Algorithm ancestor: historicalsource/rushtherock@845329d7,
 * game/carsnd.c target_sound; vector subtraction: game/vecmath.h mvecsub.
 * N64 uses x/z, a binary32 .924f*.924f threshold, two signed-half pan
 * tables, a special centered sound 6, and its normal seven-argument API.
 * Initializing car before the guards and using both compound signed-square
 * multiplications reproduces IDO's alias schedule and FP operand order.
 * No artificial locals, extra formals, volatile accesses or dead operations.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct GameCar { s32 header[2]; f32 position[3]; s32 middle[6]; f32 matrix[9]; unsigned char rest[872]; } GameCar;
extern s8 D_8010FFC0;
extern u8 D_80153E8F[];
extern GameCar player_array[];
extern s16 D_8011F020[], D_8011F040[];

void func_800A61B0(void *, void *, void *);
u32 high_scores_display(u32, u32, u32, u8, f32, f32, f32);

#define vecsub(a,b,r) {r[0]=a[0]-b[0]; r[1]=a[1]-b[1]; r[2]=a[2]-b[2];}
s32 stat_lap_split(s32 sound, s32 slot, f32 *position, u8 mode) {
    GameCar *car = &player_array[slot];
    f32 output[3];
    f32 delta[3];
    f32 threshold;
    f32 magnitude_squared;
    f32 side_squared;
    f32 forward_squared;
    s32 direction;

    if (D_8010FFC0 == 0) {
        return -1;
    }
    if (D_80153E8F[slot * 8] != 6) {
        return -1;
    }
    vecsub(position, car->position, delta);
    func_800A61B0(delta, output, car->matrix);
    forward_squared = output[2] * output[2];
    side_squared = output[0] * output[0];
    direction = 0;
    magnitude_squared = forward_squared + side_squared;
    if (magnitude_squared > 1.0f) {
        forward_squared *= (f32)(output[2] >= 0.0f ? 1 : -1);
        side_squared *= (f32)(output[0] >= 0.0f ? 1 : -1);
        threshold = (.924f * .924f) * magnitude_squared;
        if (threshold < side_squared) {
            direction = 4;
        } else if (side_squared < -threshold) {
            direction = 8;
        }
        if (threshold < forward_squared) {
            direction |= 1;
        } else if (forward_squared < -threshold) {
            direction |= 2;
        }
    }
    if (sound == 6) {
        return high_scores_display(sound, slot, 1, mode, 1.0f, 0.0f, 0.0f);
    }
    return high_scores_display(sound, slot, 1, mode, 1.0f, (f32) D_8011F020[direction], (f32) D_8011F040[direction]);
}
