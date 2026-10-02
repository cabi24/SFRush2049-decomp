/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef float f32;
typedef int s32;
typedef struct BoundsView {
    unsigned char prefix[244];
    f32 upper_x;
    f32 unused_y;
    f32 upper_z;
    unsigned char gap[24];
    f32 lower_x;
    f32 unused_lower_y;
    f32 lower_z;
} BoundsView;
s32 func_800CEC8C(BoundsView *bounds, f32 *position, f32 upper_y)
{
    if (bounds->upper_x < position[0] || position[0] < bounds->lower_x) return 0;
    if (upper_y < position[1] || position[1] < -1.0f) return 0;
    if (bounds->upper_z < position[2] || position[2] < bounds->lower_z) return 0;
    return 1;
}
