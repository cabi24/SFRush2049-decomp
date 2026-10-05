/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * stat_lap_split (historical label): HUD indicator for a world position relative to a player's car.
 * Returns -1 unless the feature byte D_8010FFC0 is set and the player's slot state
 * (D_80153E88[player], 8-byte records, byte +7) is 6. Otherwise the vector from the car's position
 * (car +0x08) to `pos` is rotated into the car frame by func_800A61B0 (out = M * v with the car
 * matrix at +0x2C). If the horizontal length^2 exceeds 1 the direction is classified with signed
 * squares against cos^2 of the half-sector (0.924f * 0.924f, folded to 0x3F5A9111): bit 4/8 for
 * +x/-x, bit 1/2 for +z/-z. high_scores_display (historical label) is then called with
 * (mode, player, 1, arg3, 1.0f, x, y), where x/y come from the s16 tables D_8011F020/D_8011F040
 * indexed by the direction bits (0,0 when mode == 6). No arcade ancestor found (N64 addition).
 * Shaping: `car = D_80152818; car += player;` (not `&D_80152818[player]`): the latter lets ugen
 * emit `.noalias car,sp`, and as1 then hoists the car loads above the delta stores. `x2 *= sign`
 * (compound) gives retail's mul.s operand order. -O3 only.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;

typedef struct {
    u8 pad0[8];
    f32 pos[3];
    u8 pad14[0x2C - 0x14];
    f32 matrix[9];
    u8 pad50[952 - 0x50];
} Car;
typedef struct {
    u8 pad0[7];
    u8 state;
} Slot;

extern s8 D_8010FFC0;
extern Slot D_80153E88[];
extern Car D_80152818[];
extern s16 D_8011F020[];
extern s16 D_8011F040[];
void func_800A61B0(f32 *in, f32 *out, f32 *m);
s32 high_scores_display(s32, s32, s32, u8, f32, f32, f32);

s32 stat_lap_split(s32 arg0, s32 player, f32 *pos, u8 arg3) {
    Car *car;
    f32 out[3];
    f32 delta[3];
    f32 lim;
    f32 sum;
    f32 x2;
    f32 z2;
    s32 dir;

    if (D_8010FFC0 == 0)
        return -1;
    if (D_80153E88[player].state != 6)
        return -1;
    car = D_80152818;
    car += player;
    delta[0] = pos[0] - car->pos[0];
    delta[1] = pos[1] - car->pos[1];
    delta[2] = pos[2] - car->pos[2];
    func_800A61B0(delta, out, car->matrix);
    z2 = out[2] * out[2];
    x2 = out[0] * out[0];
    dir = 0;
    sum = z2 + x2;
    if (sum > 1.0f) {
        z2 *= (f32)(out[2] >= 0.0f ? 1 : -1);
        x2 *= (f32)(out[0] >= 0.0f ? 1 : -1);
        lim = 0.924f * 0.924f * sum;
        if (lim < x2)
            dir = 4;
        else if (x2 < -lim)
            dir = 8;
        if (lim < z2)
            dir |= 1;
        else if (z2 < -lim)
            dir |= 2;
    }
    if (arg0 == 6)
        return high_scores_display(arg0, player, 1, arg3, 1.0f, 0.0f, 0.0f);
    return high_scores_display(arg0, player, 1, arg3, 1.0f, D_8011F020[dir], D_8011F040[dir]);
}
