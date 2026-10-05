/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
typedef struct Car952 { u8 pad0[8]; f32 pos[3]; u8 pad14[952 - 20]; } Car952;
typedef struct Model2056 { u8 pad0[2026]; s8 active; u8 pad7EB[2056 - 2027]; } Model2056;
extern Car952 D_80152818[];
extern Model2056 D_8014A250[];

s32 func_8010C448(s16 *id, f32 *pos, f32 *radius, f32 *out)
{
    f32 pad[2];
    f32 d[3];
    f32 p[3];
    f32 r;
    Car952 *car;

    r = *radius;
    if (!D_8014A250[*id].active) {
        return 0;
    }
    car = &D_80152818[*id];
    p[0] = pos[0];
    p[1] = pos[1];
    p[2] = pos[2];
    d[0] = car->pos[0] - p[0];
    d[1] = car->pos[1] - p[1];
    d[2] = car->pos[2] - p[2];
    r += 3.5f;
    if (out) {
        *out = d[0] * d[0] + d[2] * d[2] - r * r;
    }
    if (d[0] * d[0] + d[2] * d[2] - r * r > 0.0f) {
        return 0;
    }
    if (d[1] > -2.0f && d[1] < 18.0f) {
        return 1;
    }
    return 0;
}
