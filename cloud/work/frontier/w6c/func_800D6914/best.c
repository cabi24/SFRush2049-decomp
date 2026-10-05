/* flags: -g0 -O3 -mips2 -G 0 -non_shared -- PROVISIONAL LANE, first draft: 282/315 words, 47 normalised rows
 * with the stand-in caller below (blob_unit --internal func_800D6914 --keep standin_caller --block func_800D6914).
 * Main-menu 3D buttons; the fourteen filler words and x0/x1 only reproduce the frame layout. See ../RESULTS.md. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct TexDef {
    char name[16];
    u16 width, height;
    u8 pad[16];
} TexDef;

typedef struct Button {
    /* 0x00 */ s32 pad0;
    /* 0x04 */ s32 handle;
    /* 0x08 */ f32 angle;
    /* 0x0C */ f32 mtx[3][3];
    /* 0x30 */ f32 pos[3];
    /* 0x3C */ u8 pad3C[4];
} Button; /* 0x40 */

extern Button D_801102B4[9];
extern s32 D_801105B4;
extern s32 D_801105B8;
extern f32 D_801105BC;
extern s32 D_80110638;
extern volatile f32 D_8002EB94;
extern f32 D_8011418C[3][3];
extern f32 D_801543D0;
extern volatile u8 D_80140BDC;

void *memcpy(void *dst, const void *src, u32 n);
TexDef *func_800B24EC(char *name, s16 *index, s8 lo, s8 hi, s32 err);
void func_800B5898(f32 angle, f32 mtx[3][3]);
void func_800B5940(f32 angle, f32 mtx[3][3]);
void func_8008B32C(f32 src[3][3], f32 dst[3][3], f32 scale);
void func_8008D870(s16 handle, TexDef *tex, s32 arg2);
void model_transform_setup(s32 handle, s32 arg1, s32 arg2);

f32 fabsf(f32);
#pragma intrinsic(fabsf)

void func_800D6914(void) {
    s32 i;
    f32 y;
    TexDef *tex;
    f32 diff;
    f32 speed;
    f32 mtx[3][3];
    f32 pos[3];
    f32 old;
    f32 sel_y;
    s32 unused;
    s16 index;
    s32 x0;
    s32 x1;

    if (D_801105B4 == 0) {
        return;
    }
    y = 100.0f;
    for (i = 0; i < 4; i++) {
        if (i == D_80110638) {
            sel_y = y;
            if (D_801102B4[i].angle < 3.1415927f) {
                D_801102B4[i].angle += 12.566371f * D_8002EB94;
            }
            if (D_801102B4[i].angle > 3.1415927f || D_801105B8 == 1) {
                D_801102B4[i].angle = 3.1415927f;
            }
        } else {
            if (D_801102B4[i].angle > 0.0f) {
                D_801102B4[i].angle -= 12.566371f * D_8002EB94;
            }
            if (D_801102B4[i].angle < 0.0f || D_801105B8 == 1) {
                D_801102B4[i].angle = 0.0f;
            }
        }
        memcpy(D_801102B4[i].mtx, D_8011418C, 36);
        memcpy(pos, D_801102B4[i].pos, 12);
        D_801102B4[i].pos[0] = -90.0f;
        D_801102B4[i].pos[1] = y;
        D_801102B4[i].pos[2] = 300.0f;
        if (D_801105B8 == 0) {
            if (pos[1] + 180.0f * D_8002EB94 < D_801102B4[i].pos[1]) {
                D_801102B4[i].pos[1] = pos[1] + 180.0f * D_8002EB94;
            }
            if (D_801102B4[i].pos[1] < pos[1] - 180.0f * D_8002EB94) {
                D_801102B4[i].pos[1] = pos[1] - 180.0f * D_8002EB94;
            }
        }
        D_801102B4[i + 4].angle = D_801102B4[i].angle;
        D_801102B4[i + 4].pos[0] = 150.0f;
        D_801102B4[i + 4].pos[1] = D_801102B4[i].pos[1];
        D_801102B4[i + 4].pos[2] = D_801102B4[i].pos[2];
        func_800B5898(D_801102B4[i].angle - 1.5707964f, D_801102B4[i].mtx);
        memcpy(D_801102B4[i + 4].mtx, D_801102B4[i].mtx, 36);
        if (D_801102B4[i].angle > 1.5707964f) {
            tex = func_800B24EC("BUTTON_SELECT", &index, 0, D_80140BDC - 1, 1);
        } else {
            tex = func_800B24EC("BUTTON", &index, 0, D_80140BDC - 1, 1);
        }
        func_8008D870(D_801102B4[i].handle, tex, -1);
        func_8008D870(D_801102B4[i + 4].handle, tex, -1);
        model_transform_setup(D_801102B4[i].handle, 0, 15);
        model_transform_setup(D_801102B4[i + 4].handle, 0, 15);
        y -= 40.0f;
    }
    memcpy(pos, D_801102B4[8].pos, 12);
    D_801102B4[8].angle += 6.2831855f * D_8002EB94;
    if (D_801102B4[8].angle > 6.2831855f) {
        D_801102B4[8].angle -= 6.2831855f;
    }
    memcpy(D_801102B4[8].mtx, D_8011418C, 36);
    D_801102B4[8].pos[0] = -160.0f;
    D_801102B4[8].pos[1] = sel_y * 225.0f / 300.0f;
    D_801102B4[8].pos[2] = 225.0f;
    if (D_801105B8 == 0) {
        diff = fabsf(D_801102B4[8].pos[1] - pos[1]);
        if (D_801543D0 < diff * 60.0f / 10.0f) {
            D_801543D0 = diff * 60.0f / 10.0f;
            if (D_801543D0 < 540.0f * D_801105BC) {
                D_801543D0 = D_801543D0;
            } else {
                D_801543D0 = 540.0f * D_801105BC;
            }
            if (180.0f * D_801105BC < D_801543D0) {
                D_801543D0 = D_801543D0;
            } else {
                D_801543D0 = 180.0f * D_801105BC;
            }
        }
        if (pos[1] + D_801543D0 * D_8002EB94 < D_801102B4[8].pos[1]) {
            D_801102B4[8].pos[1] = pos[1] + D_801543D0 * D_8002EB94;
        } else if (D_801102B4[8].pos[1] < pos[1] - D_801543D0 * D_8002EB94) {
            D_801102B4[8].pos[1] = pos[1] - D_801543D0 * D_8002EB94;
        } else {
            D_801543D0 = 180.0f * D_801105BC;
        }
    }
    func_800B5940(D_801102B4[8].angle, D_801102B4[8].mtx);
    func_800B5898(1.5707964f, D_801102B4[8].mtx);
    func_8008B32C(D_801102B4[8].mtx, D_801102B4[8].mtx, 0.4f);
    D_801105B8 = 0;
}

void standin_caller(void) {
    func_800D6914();
}
