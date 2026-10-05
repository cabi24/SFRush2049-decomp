typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef float f32;
typedef float F32;
typedef short S16;
typedef int S32;

typedef struct V3 { F32 x, y, z; } V3;
typedef struct HN { u8 p[0x2C]; s32 f2c; } HN;
typedef struct Q { u8 p[0x48]; HN **h; } Q;
typedef struct Car {
    u8 p0[0x8];
    F32 RWR[3];         /* 0x08 */
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
    s8 view2;           /* 0x35E */
    u8 p35f[0x380 - 0x35F];
    Q *q;               /* 0x380 */
} Car;

extern F32 D_80152720[];
extern F32 D_80152708[];
typedef struct { u8 other[96]; F32 matrix[3][3]; F32 position[3]; u8 tail[8]; } Slot152;
extern Slot152 D_80150B70[];
void func_8008D6FC(s16 objnum, F32 *pos, F32 *uvs);
extern F32 D_801526F8[];   /* rear_x per player */
extern F32 D_801526E0[];   /* rear_y per player */
extern F32 D_8012E690[][3]; /* cam_pos */
extern s16 D_8012E6C8[];   /* leg */
extern F32 D_8012E6E8[];   /* leg time */
extern F32 D_801108C8[];   /* leg durations */
extern volatile F32 D_8002EB94;
extern s32 state_word_a;
extern F32 D_801526A8[][3];
void func_800E9234(Car *car);
s8 func_800CDE38(HN **h);
void func_800E8D50(Car *car, F32 *pos, s32 mat, F32 *res);
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

void func_800E95DC(S16 mode, Car *car, F32 *pos, s32 uvs) {
    V3 delta[4];
    F32 res[3], pos2[3];
    s32 pl;
    F32 k, dx, dy, dz;

    pl = car->player;
    if (mode == 0) {
        D_8012E690[pl][0] = pos[0];
        D_8012E690[pl][1] = pos[1];
        D_8012E690[pl][2] = pos[2];
        D_80150B70[pl].position[0] = pos[0];
        D_80150B70[pl].position[1] = pos[1];
        D_80150B70[pl].position[2] = pos[2];
        res[0] = car->RWR[0];
        res[1] = car->RWR[1];
        res[2] = car->RWR[2];
        pos[0] = pos[0] - res[0];
        pos[1] = pos[1] - res[1];
        pos[2] = pos[2] - res[2];
        vector_normalize_length(pos, D_80150B70[pl].matrix);
        D_8012E6C8[pl] = 0;
        D_8012E6E8[pl] = 0.0f;
        return;
    }
    switch (D_8012E6C8[pl]) {
    case 0:
        func_800E92C8(car, res, D_801526F8[pl], D_801526E0[pl]);
        res[0] = res[0] + pos[0];
        res[1] = res[1] + pos[1];
        res[2] = res[2] + pos[2];
        k = .1f * D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]];
        dx = res[0] - D_8012E690[pl][0];
        delta[pl].x = k * dx;
        dy = res[1] - D_8012E690[pl][1];
        delta[pl].y = k * dy;
        dz = res[2] - D_8012E690[pl][2];
        delta[pl].z = k * dz;
        k = D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]];
        pos2[0] = car->RWR[0] - car->dr_uvs[2][0] * 40.0f;
        pos2[1] = car->RWR[1] + 5.0f;
        pos2[2] = car->RWR[2] - car->dr_uvs[2][2] * 40.0f;
        dx = pos2[0] - D_8012E690[pl][0];
        delta[pl].x = k * dx;
        dy = pos2[1] - D_8012E690[pl][1];
        delta[pl].y = k * dy;
        dz = pos2[2] - D_8012E690[pl][2];
        delta[pl].z = k * dz;
        break;
    case 1:
        func_800E92C8(car, res, D_801526F8[pl], D_801526E0[pl]);
        res[0] = res[0] + pos[0];
        res[1] = res[1] + pos[1];
        res[2] = res[2] + pos[2];
        k = D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]];
        dx = res[0] - D_8012E690[pl][0];
        delta[pl].x = k * dx;
        dy = res[1] - D_8012E690[pl][1];
        delta[pl].y = k * dy;
        dz = res[2] - D_8012E690[pl][2];
        delta[pl].z = k * dz;
        break;
    case 2:
        func_800E9234(car);
        if (!(state_word_a & 8)) {
            s8 v;
            v = 2;
            if (car->q->h[0]->f2c != 0)
                v = func_800CDE38(car->q->h);
            car->view = v;
            car->view2 = v;
            if (car->view == 1) {
                D_801526A8[car->player][0] = 0.0f;
                D_801526A8[car->player][1] = 0.0f;
                D_801526A8[car->player][2] = 0.0f;
            }
        }
        break;
    }
    if (D_8012E6C8[pl] == 0 || D_8012E6C8[pl] == 1) {
        pos2[0] = delta[pl].x + D_8012E690[pl][0];
        pos2[1] = delta[pl].y + D_8012E690[pl][1];
        pos2[2] = delta[pl].z + D_8012E690[pl][2];
        res[0] = pos2[0] - pos[0];
        res[1] = pos2[1] - pos[1];
        res[2] = pos2[2] - pos[2];
        D_80152720[pl] = 0.0f;
        func_800E8D50(car, pos, uvs, res);
        D_80150B70[pl].position[1] += 4.0f;
        D_8012E6E8[pl] += D_8002EB94;
        if (D_801108C8[D_8012E6C8[pl]] <= D_8012E6E8[pl]) {
            D_8012E6C8[pl]++;
            D_8012E6E8[pl] = 0.0f;
            D_8012E690[pl][0] = D_80150B70[pl].position[0];
            D_8012E690[pl][1] = D_80150B70[pl].position[1];
            D_8012E690[pl][2] = D_80150B70[pl].position[2];
        }
    }
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
