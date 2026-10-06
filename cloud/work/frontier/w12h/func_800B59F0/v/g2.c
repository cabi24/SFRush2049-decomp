/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w10e PROVISIONAL near-miss: 2/343 words in the unit with stand-in callers zz_caller/zz_caller2 (real caller physics_sym is unmatched). Pause-menu 3D buttons. Residual: the D_8011AD40[i] test is coloured v0, retail v1. See ../RESULTS.md. */
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

extern s32 D_8011AC94;
extern s32 D_8011AC98;
extern f32 D_8011AC9C;
extern Button D_8011A994[];
extern s8 D_8011AD40[];
extern s32 D_8011AD44;
extern f32 D_8011418C[];
extern volatile f32 D_8002EB94;
extern volatile u8 D_80140BDC;
extern f32 D_80152678;

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

static s32 getvis(s32 i) {
    return D_8011AD40[i];
}
void func_800B59F0(void) {
    f32 cur[2];
    f32 old[3];
    f32 d;
    f32 sel_y;
    s32 vis;
    s16 tex;
    s32 i;
    s32 name;
    f32 y;

    if (D_8011AC94 == 0)
        return;
    y = 100.0f;
    for (i = 0; i < 4; i++) {
        if (i == D_8011AD44) {
            sel_y = y;
            if (D_8011A994[i].ang < 3.1415927f)
                D_8011A994[i].ang += 12.566371f * D_8002EB94;
            if (3.1415927f < D_8011A994[i].ang || D_8011AC98 == 1)
                D_8011A994[i].ang = 3.1415927f;
        } else {
            if (0.0f < D_8011A994[i].ang)
                D_8011A994[i].ang -= 12.566371f * D_8002EB94;
            if (D_8011A994[i].ang < 0.0f || D_8011AC98 == 1)
                D_8011A994[i].ang = 0.0f;
        }
        memcpy(D_8011A994[i].mat, D_8011418C, 36);
        memcpy(old, D_8011A994[i].pos, 12);
        D_8011A994[i].pos[0] = -90.0f;
        D_8011A994[i].pos[1] = y;
        D_8011A994[i].pos[2] = 300.0f;
        if (D_8011AC98 == 0) {
            if (old[1] + 180.0f * D_8002EB94 < D_8011A994[i].pos[1])
                D_8011A994[i].pos[1] = old[1] + 180.0f * D_8002EB94;
            if (D_8011A994[i].pos[1] < old[1] - 180.0f * D_8002EB94)
                D_8011A994[i].pos[1] = old[1] - 180.0f * D_8002EB94;
        }
        D_8011A994[i + 4].ang = D_8011A994[i].ang;
        D_8011A994[i + 4].pos[0] = 150.0f;
        D_8011A994[i + 4].pos[1] = D_8011A994[i].pos[1];
        D_8011A994[i + 4].pos[2] = D_8011A994[i].pos[2];
        func_800B5898(D_8011A994[i].ang - 1.5707964f, D_8011A994[i].mat);
        memcpy(D_8011A994[i + 4].mat, D_8011A994[i].mat, 36);
        if (1.5707964f < D_8011A994[i].ang)
            name = func_800B24EC("BUTTON_SELECT", &tex, 0, (s8)(D_80140BDC - 1), 1);
        else
            name = func_800B24EC("BUTTON", &tex, 0, (s8)(D_80140BDC - 1), 1);
        func_8008D870(D_8011A994[i].handle, name, -1);
        func_8008D870(D_8011A994[i + 4].handle, name, -1);
        if ((vis = getvis(i)) == 0) {
            model_data_load(D_8011A994[i].handle, 1, 15);
            model_data_load(D_8011A994[i + 4].handle, 1, 15);
        } else {
            model_transform_setup(D_8011A994[i].handle, 0, 15);
            if (i < 3)
                model_transform_setup(D_8011A994[i + 4].handle, 0, 15);
            else
                model_data_load(D_8011A994[i + 4].handle, 1, 15);
            y -= 40.0f;
        }
    }
    memcpy(old, D_8011A994[8].pos, 12);
    D_8011A994[8].ang += 6.2831855f * D_8002EB94;
    if (6.2831855f < D_8011A994[8].ang)
        D_8011A994[8].ang -= 6.2831855f;
    memcpy(D_8011A994[8].mat, D_8011418C, 36);
    D_8011A994[8].pos[0] = -160.0f;
    D_8011A994[8].pos[2] = 225.0f;
    D_8011A994[8].pos[1] = sel_y * 225.0f / 300.0f;
    if (D_8011AC98 == 0) {
        y = D_8011A994[8].pos[1];
        if (D_80152678 < fabsf(y - old[1]) * 60.0f / 10.0f) {
            D_80152678 = fabsf(y - old[1]) * 60.0f / 10.0f;
            D_80152678 = (D_80152678 < 540.0f * D_8011AC9C) ? D_80152678 : 540.0f * D_8011AC9C;
            D_80152678 = (180.0f * D_8011AC9C < D_80152678) ? D_80152678 : 180.0f * D_8011AC9C;
        }
        if (old[1] + D_80152678 * D_8002EB94 < y)
            D_8011A994[8].pos[1] = old[1] + D_80152678 * D_8002EB94;
        else if (y < old[1] - D_80152678 * D_8002EB94)
            D_8011A994[8].pos[1] = old[1] - D_80152678 * D_8002EB94;
        else
            D_80152678 = 180.0f * D_8011AC9C;
    }
    func_800B5940(D_8011A994[8].ang, D_8011A994[8].mat);
    func_800B5898(1.5707964f, D_8011A994[8].mat);
    func_8008B32C(D_8011A994[8].mat, D_8011A994[8].mat, 0.4f);
    D_8011AC98 = 0;
}

void zz_caller(void) {
    func_800B59F0();
}
void zz_caller2(void) {
    func_800B59F0();
}
