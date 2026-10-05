typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef float f32;
typedef float F32;
typedef short S16;

typedef struct Car {
    u8 p0[0x14];
    F32 dr_vel[3];      /* 0x14 */
    u8 p20[0xC];
    F32 dr_uvs[3][3];   /* 0x2C */
    u8 p50[0x58];
    F32 V[3];           /* 0xA8 */
    u8 pb4[0x35C - 0xB4];
    s8 player;          /* 0x35C */
    s8 view;            /* 0x35D */
} Car;

extern F32 D_80152720[];
void func_8009E820(F32 *in, F32 *out, F32 *uvs);
void func_800A61B0(F32 *in, F32 *out, F32 *uvs);
void func_800CFDEC(F32 *a, F32 *b, s16 n, F32 lo, F32 hi, F32 t, F32 *out);
float sqrtf(float);
float fabsf(float);
#pragma intrinsic(sqrtf)
#pragma intrinsic(fabsf)

#define MAX_VEL 100.0f

void func_800E92C8(Car *car, F32 *camoff, F32 ab, F32 AB) {
    F32 vec[3], res[3], temp[3], A, a, B, b, fact, magvel, uvs[3][3];
    S16 fixed_cam, flag;
    char buf[60];
    s32 pl;

    pl = car->player;
    if (car->view == 8)
        magvel = 0;
    else
        magvel = sqrtf(car->dr_vel[0]*car->dr_vel[0] + car->dr_vel[2]*car->dr_vel[2]);

    if (magvel <= MAX_VEL) {
        vec[0] = 0;
        vec[1] = AB;
        vec[2] = -ab;
        if (car->view == 8) {
            temp[0] = vec[0];
            temp[1] = vec[1];
            temp[2] = vec[2];
        } else
            func_8009E820(vec, temp, (F32 *)car->dr_uvs);
        temp[1] = fabsf(temp[1]);
    }

    if (magvel > 1) {
        if (car->view == 4) {
            vec[0] = car->V[1];
            vec[2] = 0;
            vec[1] = car->V[0];
            func_8009E820(vec, res, (F32 *)car->dr_uvs);
            res[0] = -res[0];
            res[2] = -res[2];
        } else {
            res[0] = car->dr_vel[0];
            res[1] = car->dr_vel[1];
            res[2] = car->dr_vel[2];
            if (ab < 0)
                res[1] = -res[1];
            func_800A61B0(res, vec, (F32 *)car->dr_uvs);
            vec[2] = fabsf(vec[2]);
            func_8009E820(vec, res, (F32 *)car->dr_uvs);
            res[0] = -res[0];
            res[2] = -res[2];
        }

        fact = ab / magvel;
        res[0] *= fact;
        res[1] *= fact;
        res[2] *= fact;

        a = res[1];
        b = sqrtf(res[2]*res[2] + res[0]*res[0]);

        if (b > .001f) {
            A = -a * AB / ab;
            fact = (b - A) / b;
            res[0] *= fact;
            res[2] *= fact;
        }

        B = b * AB / ab;
        res[1] = fabsf(B - res[1]);

        if (magvel < MAX_VEL)
            func_800CFDEC(temp, res, 3, 1, MAX_VEL, magvel, res);
    } else {
        res[0] = temp[0];
        res[1] = temp[1];
        res[2] = temp[2];
    }

    if (car->view == 8)
        D_80152720[pl] = 0;
    else if (car->view == 4)
        D_80152720[pl] = 1 - magvel / MAX_VEL;
    else if (magvel < MAX_VEL)
        D_80152720[pl] = 1 - magvel * .01f;
    else
        D_80152720[pl] = 0;

    camoff[0] = res[0];
    camoff[1] = res[1];
    camoff[2] = res[2];
}
