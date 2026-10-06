typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    s32 type;            /* 0x00 */
    s32 handle;          /* 0x04 */
    f32 ang;             /* 0x08 */
    f32 mat[3][3];       /* 0x0C */
    f32 pos[3];          /* 0x30 */
    u8 pad3C[4];
} Button; /* 0x40 */

extern s32 D_801105B4;
extern s32 D_801105B8;
extern f32 D_801105BC;
extern Button D_801102B4[];
extern s32 D_80110638;
extern f32 D_8011418C[];
extern volatile f32 D_8002EB94;
extern volatile u8 D_80140BDC;
extern f32 D_801543D0;

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

void func_800D6914(void) {
    s32 i;
    s32 name;
    f32 y;
    f32 cur[12];
    f32 old[3];
    f32 d;
    f32 sel_y;
    f32 k;
    s16 tex;

    if (D_801105B4 == 0)
        return;
    y = 100.0f;
    for (i = 0; i < 4; i++) {
        if (i == D_80110638) {
            sel_y = y;
            if (D_801102B4[i].ang < 3.1415927f)
                D_801102B4[i].ang += 12.566371f * D_8002EB94;
            if (3.1415927f < D_801102B4[i].ang || D_801105B8 == 1)
                D_801102B4[i].ang = 3.1415927f;
        } else {
            if (0.0f < D_801102B4[i].ang)
                D_801102B4[i].ang -= 12.566371f * D_8002EB94;
            if (D_801102B4[i].ang < 0.0f || D_801105B8 == 1)
                D_801102B4[i].ang = 0.0f;
        }
        memcpy(D_801102B4[i].mat, D_8011418C, 36);
        memcpy(old, D_801102B4[i].pos, 12);
        D_801102B4[i].pos[0] = -90.0f;
        D_801102B4[i].pos[1] = y;
        D_801102B4[i].pos[2] = 300.0f;
        if (D_801105B8 == 0) {
            if (old[1] + 180.0f * D_8002EB94 < D_801102B4[i].pos[1])
                D_801102B4[i].pos[1] = old[1] + 180.0f * D_8002EB94;
            if (D_801102B4[i].pos[1] < old[1] - 180.0f * D_8002EB94)
                D_801102B4[i].pos[1] = old[1] - 180.0f * D_8002EB94;
        }
        D_801102B4[i + 4].ang = D_801102B4[i].ang;
        D_801102B4[i + 4].pos[0] = 150.0f;
        D_801102B4[i + 4].pos[1] = D_801102B4[i].pos[1];
        D_801102B4[i + 4].pos[2] = D_801102B4[i].pos[2];
        func_800B5898(D_801102B4[i].ang - 1.5707964f, D_801102B4[i].mat);
        memcpy(D_801102B4[i + 4].mat, D_801102B4[i].mat, 36);
        if (1.5707964f < D_801102B4[i].ang)
            name = func_800B24EC("BUTTON_SELECT", &tex, 0, (s8)(D_80140BDC - 1), 1);
        else
            name = func_800B24EC("BUTTON", &tex, 0, (s8)(D_80140BDC - 1), 1);
        func_8008D870(D_801102B4[i].handle, name, -1);
        func_8008D870(D_801102B4[i + 4].handle, name, -1);
        model_transform_setup(D_801102B4[i].handle, 0, 15);
        model_transform_setup(D_801102B4[i + 4].handle, 0, 15);
        y -= 40.0f;
    }
    memcpy(old, D_801102B4[8].pos, 12);
    D_801102B4[8].ang += 6.2831855f * D_8002EB94;
    if (6.2831855f < D_801102B4[8].ang)
        D_801102B4[8].ang -= 6.2831855f;
    memcpy(D_801102B4[8].mat, D_8011418C, 36);
    D_801102B4[8].pos[0] = -160.0f;
    D_801102B4[8].pos[2] = 225.0f;
    D_801102B4[8].pos[1] = sel_y * 225.0f / 300.0f;
    if (D_801105B8 == 0) {
        y = D_801102B4[8].pos[1];
        if (D_801543D0 < fabsf(y - old[1]) * 60.0f / 10.0f) {
            D_801543D0 = fabsf(y - old[1]) * 60.0f / 10.0f;
            D_801543D0 = (D_801543D0 < 540.0f * D_801105BC) ? D_801543D0 : 540.0f * D_801105BC;
            D_801543D0 = (180.0f * D_801105BC < D_801543D0) ? D_801543D0 : 180.0f * D_801105BC;
        }
        if (old[1] + D_801543D0 * D_8002EB94 < y)
            D_801102B4[8].pos[1] = old[1] + D_801543D0 * D_8002EB94;
        else if (y < old[1] - D_801543D0 * D_8002EB94)
            D_801102B4[8].pos[1] = old[1] - D_801543D0 * D_8002EB94;
        else
            D_801543D0 = 180.0f * D_801105BC;
    }
    func_800B5940(D_801102B4[8].ang, D_801102B4[8].mat);
    func_800B5898(1.5707964f, D_801102B4[8].mat);
    func_8008B32C(D_801102B4[8].mat, D_801102B4[8].mat, 0.4f);
    D_801105B8 = 0;
}

void zz_caller(void) {
    func_800D6914();
}
void zz_caller2(void) {
    func_800D6914();
}
