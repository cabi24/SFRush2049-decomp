typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u8 r, g, b, a;
} Color;

typedef struct {
    s32 type;            /* 0x00 */
    s32 handle;          /* 0x04 */
    f32 ang;             /* 0x08 */
    f32 mat[3][3];       /* 0x0C */
    f32 pos[3];          /* 0x30 */
    u8 alpha;            /* 0x3C */
    u8 pad3D[3];
} Button; /* 0x40 */

typedef struct {
    u8 pad0[60];
    Color color;         /* 0x3C */
    u8 pad40[4];
} Model68;

extern s32 D_80116D0C;
extern s32 D_80116D10;
extern s32 D_80116D14[];
extern s16 D_80116D9C;
extern Color D_80116DB0;
extern Button D_8011650C[];
extern s16 D_80149D9C;
extern f32 D_8015273C;
extern f32 D_80152740;
extern f32 D_8011418C[];
extern volatile f32 D_8002EB94;
extern volatile u8 D_80140BDC;
extern Model68 D_8012E700[];

f32 fabsf(f32);
#pragma intrinsic(fabsf)
void *memcpy(void *, const void *, u32);
s32 func_800B24EC();
void func_8008D870(s16 handle, s32 tex, s32 c);
void func_800B5898(f32 angle, f32 uv[][3]);
void func_800B5940(f32 angle, f32 uv[][3]);
void func_8008B32C(f32 (*dst)[3], f32 (*src)[3], f32 s);
void model_data_load();
void model_transform_setup();

void func_800DA2C0(void) {
    f32 pad0[4];
    f32 old[3];
    s8 flag;
    f32 pad1[3];
    s16 tex;
    Color color;
    f32 pad2[1];
    s32 i;
    s32 n;
    s32 name;
    s32 yi;
    f32 y;
    f32 sel_y;
    f32 speed;

    flag = 1;
    color = D_80116DB0;
    if (D_80116D0C == 0)
        return;
    n = 0;
    for (i = 0; i < D_80116D9C; i++) {
        if (D_80116D14[i] != 0)
            n++;
    }
    memcpy(old, D_8011650C[28].pos, 12);
    D_8011650C[28].ang += 6.2831855f * D_8002EB94;
    if (6.2831855f < D_8011650C[28].ang)
        D_8011650C[28].ang -= 6.2831855f;
    memcpy(D_8011650C[28].mat, D_8011418C, 36);
    D_8011650C[28].pos[0] = -160.0f;
    D_8011650C[28].pos[2] = 225.0f;
    sel_y = 75.0f - n * 165.0f / (D_80149D9C - 1);
    D_8011650C[28].pos[1] = sel_y;
    if (D_80116D10 == 0) {
        y = D_8011650C[28].pos[1];
        speed = D_8015273C;
        if (speed < fabsf(y - old[1]) * 60.0f / 10.0f) {
            speed = fabsf(y - old[1]) * 60.0f / 10.0f;
            if (!(speed < 540.0f))
                speed = 540.0f;
            if (!(180.0f < speed))
                speed = 180.0f;
        }
        if (old[1] + speed * D_8002EB94 < y) {
            D_8011650C[28].pos[1] = old[1] + speed * D_8002EB94;
            D_8015273C = speed;
        } else if (y < old[1] - speed * D_8002EB94) {
            D_8011650C[28].pos[1] = old[1] - speed * D_8002EB94;
            D_8015273C = speed;
        } else {
            D_8015273C = 180.0f;
        }
    }
    func_800B5940(D_8011650C[28].ang, D_8011650C[28].mat);
    func_800B5898(1.5707964f, D_8011650C[28].mat);
    func_8008B32C(D_8011650C[28].mat, D_8011650C[28].mat, 0.4f);
    yi = sel_y * 1.33f + n * 40;
    for (i = 0; i < 14; i++) {
        y = yi;
        if (i == D_80116D9C) {
            if (D_8011650C[i].ang < 3.1415927f)
                D_8011650C[i].ang += 12.566371f * D_8002EB94;
            if (3.1415927f < D_8011650C[i].ang)
                D_8011650C[i].ang = 3.1415927f;
            if (D_80116D10 != 0)
                D_8011650C[i].ang = 3.1415927f;
        } else {
            if (0.0f < D_8011650C[i].ang)
                D_8011650C[i].ang -= 12.566371f * D_8002EB94;
            if (D_8011650C[i].ang < 0.0f)
                D_8011650C[i].ang = 0.0f;
            if (D_80116D10 != 0)
                D_8011650C[i].ang = 0.0f;
        }
        D_8011650C[i + 14].ang = D_8011650C[i].ang;
        memcpy(D_8011650C[i].mat, D_8011418C, 36);
        memcpy(D_8011650C[i + 14].mat, D_8011418C, 36);
        memcpy(old, D_8011650C[i].pos, 12);
        D_8011650C[i].pos[0] = -90.0f;
        D_8011650C[i].pos[1] = y;
        D_8011650C[i].pos[2] = 300.0f;
        if (D_80116D10 == 0) {
            speed = D_80152740;
            if (speed < fabsf(D_8011650C[i].pos[1] - old[1]) * 60.0f / 10.0f) {
                speed = fabsf(D_8011650C[i].pos[1] - old[1]) * 60.0f / 10.0f;
                if (!(speed < 540.0f))
                    speed = 540.0f;
            }
            if (old[1] + speed * D_8002EB94 < D_8011650C[i].pos[1]) {
                flag = 0;
                D_8011650C[i].pos[1] = old[1] + speed * D_8002EB94;
            } else if (D_8011650C[i].pos[1] < old[1] - speed * D_8002EB94) {
                flag = 0;
                D_8011650C[i].pos[1] = old[1] - speed * D_8002EB94;
            }
            D_80152740 = speed;
        }
        D_8011650C[i + 14].pos[0] = 150.0f;
        D_8011650C[i + 14].pos[1] = D_8011650C[i].pos[1] * 31.0f / 30.0f;
        D_8011650C[i + 14].pos[2] = 310.0f;
        func_800B5898(D_8011650C[i].ang - 1.5707964f, D_8011650C[i].mat);
        func_800B5898(D_8011650C[i + 14].ang - 1.5707964f, D_8011650C[i + 14].mat);
        if (1.5707964f < D_8011650C[i].ang)
            name = func_800B24EC("BUTTON_SELECT", &tex, 0, (s8)(D_80140BDC - 1), 1);
        else
            name = func_800B24EC("BUTTON", &tex, 0, (s8)(D_80140BDC - 1), 1);
        func_8008D870(D_8011650C[i].handle, name, -1);
        func_8008D870(D_8011650C[i + 14].handle, name, -1);
        if (140.0f < D_8011650C[i].pos[1] || D_8011650C[i].pos[1] < -160.0f)
            D_8011650C[i].alpha = 0;
        else if (100.0f < D_8011650C[i].pos[1])
            D_8011650C[i].alpha = 255.0f - (D_8011650C[i].pos[1] - 100.0f) * 255.0f / 40.0f;
        else if (D_8011650C[i].pos[1] < -120.0f)
            D_8011650C[i].alpha = 255.0f - (-120.0f - D_8011650C[i].pos[1]) * 255.0f / 40.0f;
        else
            D_8011650C[i].alpha = 255;
        color.a = D_8011650C[i].alpha;
        D_8011650C[i + 14].alpha = color.a;
        D_8012E700[(s16)D_8011650C[i].handle].color = color;
        D_8012E700[(s16)D_8011650C[i + 14].handle].color = color;
        if (D_80116D14[i] == 0) {
            model_data_load(D_8011650C[i].handle, 0, 15);
            model_data_load(D_8011650C[i + 14].handle, 0, 15);
            D_8011650C[i].alpha = 0;
        } else {
            if (color.a == 0) {
                model_data_load(D_8011650C[i].handle, 0, 15);
                model_data_load(D_8011650C[i + 14].handle, 0, 15);
            } else {
                model_transform_setup(D_8011650C[i].handle, 0, 15);
                model_transform_setup(D_8011650C[i + 14].handle, 0, 15);
            }
            yi = y - 40.0f;
        }
    }
    if (flag == 1)
        D_80152740 = 0.0f;
    D_80116D10 = 0;
}

void zz_caller(void) {
    func_800DA2C0();
}
void zz_caller2(void) {
    func_800DA2C0();
}
