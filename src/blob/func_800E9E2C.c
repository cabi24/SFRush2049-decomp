/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800E9E2C - circling camera for one car slot. Arcade ancestor:
 * game/camera.c circle_camera_around_car(): the camera offset rotates about
 * the car once per 6 s (IRQTIME % 6000 ms), res = {cos*15, 10, sin*15}, then
 * the slot elastic factor is cleared and the follow update (func_800E8D50,
 * N64 UpdateCarObj) places the camera. N64 additions: per-car state byte
 * D_80110680[car]; state 1 latches the clock (D_80110668[car]) after 10 s and
 * switches to state 2, which freezes the angle at the latched time.
 * IRQTIME is the float clock D_801543CC in ms converted to u32 (the
 * cvt/unsigned sequences). Own literals (1/6000, 2*pi, twice) are own .rodata.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef f32 Mat3[3][3];
typedef struct { u8 other[859]; s8 car,slot; } Object;
extern f32 D_80152720[];
extern f32 D_801543CC;
extern s8 D_80110680[];
extern f32 D_80110668[];
extern f32 sinf(f32);
extern f32 cosf(f32);
extern void func_800E8D50(Object *object, f32 *carpos, Mat3 *matrix, f32 *res);

#define IRQTIME(t) ((u32)((t) * 1000.0f))

void func_800E9E2C(Object *object, f32 *carpos, Mat3 *matrix) {
    f32 res[3], ang;
    s32 slot = object->slot;

    if (D_80110680[object->car] == 1 && D_80110668[object->car] + 10.0f < D_801543CC) {
        D_80110668[object->car] = D_801543CC;
        D_80110680[object->car] = 2;
    }
    if (D_80110680[object->car] == 2)
        ang = (f32)(IRQTIME(D_80110668[object->car]) % 6000) * (1.0f / 6000) * 6.2831855f;
    else
        ang = (f32)(IRQTIME(D_801543CC) % 6000) * (1.0f / 6000) * 6.2831855f;

    res[0] = cosf(ang) * 15.0f;
    res[1] = 10.0f;
    res[2] = sinf(ang) * 15.0f;
    D_80152720[slot] = 0.0f;
    func_800E8D50(object, carpos, matrix, res);
}
