typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef float f32;
typedef float F32;
typedef short S16;
typedef int S32;

typedef struct Car {
    u8 p0[0x14];
    F32 dr_vel[3];      /* 0x14 */
    u8 p20[0xC];
    F32 dr_uvs[3][3];   /* 0x2C */
    u8 p50[0x58];
    F32 V[3];           /* 0xA8 */
    u8 pb4[0xF6 - 0xB4];
    s16 objnum;         /* 0xF6 */
    u8 pf8[0x35C - 0xF8];
    s8 player;          /* 0x35C */
    s8 view;            /* 0x35D */
} Car;

extern F32 D_80152720[];
extern F32 D_80152708[];
typedef struct { u8 other[96]; F32 matrix[3][3]; F32 position[3]; u8 tail[8]; } Slot152;
extern Slot152 D_80150B70[];
void func_8008D6FC(s16 objnum, F32 *pos, F32 *uvs);
void vector_normalize_length(F32 *v, F32 (*m)[3]);
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
    char buf[8];

    s32 pl;

    pl = car->player;
    if (car->view == 8)
        magvel = 0;
    else
        magvel = sqrtf(car->dr_vel[2]*car->dr_vel[2] + car->dr_vel[0]*car->dr_vel[0]);

    if (magvel <= MAX_VEL) {
        vec[0] = 0.0f;
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
            vec[1] = car->V[0];
            vec[2] = 0.0f;
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
        b = sqrtf(res[0]*res[0] + res[2]*res[2]);

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
        D_80152720[pl] = 0.0f;
    else if (car->view == 4)
        D_80152720[pl] = 1.0f - magvel / MAX_VEL;
    else if (magvel < MAX_VEL)
        D_80152720[pl] = 1.0f - magvel * .01f;
    else
        D_80152720[pl] = 0.0f;

    camoff[0] = res[0];
    camoff[1] = res[1];
    camoff[2] = res[2];
}

void func_800EA108(Car *car, F32 *pos, F32 *uvs) {
    S32 i, j;
    F32 rpos[3], res[3];
    s32 slot;

    slot = car->player;
    func_8008D6FC(car->objnum, pos, uvs);
    D_80152708[slot] = .13f;
    func_800E92C8(car, res, 30, 16);
    for (i = 0; i < 3; i++)
        D_80150B70[slot].position[i] = (D_80150B70[slot].position[i]*(1.0f-D_80152708[slot]) + (pos[i]+res[i])*D_80152708[slot]);
    for (i = 0; i < 3; i++)
        rpos[i] = pos[i] - D_80150B70[slot].position[i];
    vector_normalize_length(rpos, D_80150B70[slot].matrix);
}
